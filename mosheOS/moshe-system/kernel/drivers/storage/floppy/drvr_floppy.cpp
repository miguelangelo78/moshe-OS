//****************************************************************************
//**
//**    drvr_floppy.cpp
//**		-Floppy Disk driver
//**   
//**    SEE MORE: http://forum.osdev.org/viewtopic.php?t=13538
//****************************************************************************

#include "hal\hal.h"
#include "drvr_floppy.h"
#include "task\thread.h"
#include "error\panic.h"
#include "string\string.h"
#include "memory\heap\kheap.h"
						  
const char * drive_types[8] = {
	"none",
	"360kB 5.25\"",
	"1.2MB 5.25\"",
	"720kB 3.5\"",

	"1.44MB 3.5\"",
	"2.88MB 3.5\"",
	"unknown type",
	"unknown type"
};

bool is_floppy_irq = false;
uint8_t current_drive = 0;

//============================================================================
//    FLOPPY ENUMS AND CONSTANTS
//============================================================================
char floppy_dmabuf[FLOPPY_DMA_BUFFER_SIZE] __attribute__((aligned(0x8000)));
// standard base address of the primary floppy controller
static const int floppy_base = 0x03f0;
// standard IRQ number for floppy controllers
static const int floppy_irq = 6;

// The registers of interest. There are more, but we only use these here.
enum floppy_registers {
	FLOPPY_DOR = 2,  // digital output register
	FLOPPY_MSR = 4,  // master status register, read only
	FLOPPY_FIFO = 5,  // data FIFO, in DMA operation for commands
	FLOPPY_CCR = 7   // configuration control register, write only
};

// The commands of interest. There are more, but we only use these here.
enum floppy_commands {
	CMD_SPECIFY = 3,            // SPECIFY
	CMD_WRITE_DATA = 5,         // WRITE DATA
	CMD_READ_DATA = 6,          // READ DATA
	CMD_RECALIBRATE = 7,        // RECALIBRATE
	CMD_SENSE_INTERRUPT = 8,    // SENSE INTERRUPT
	CMD_SEEK = 15,              // SEEK
};

enum { floppy_motor_off = 0, floppy_motor_on, floppy_motor_wait };
static volatile int floppy_motor_ticks = 0;
static volatile int floppy_motor_state = 0;

// Used by floppy_dma_init and floppy_do_track to specify direction
typedef enum {
	floppy_dir_read = 1,
	floppy_dir_write = 2
} floppy_dir;

//============================================================================
//    DRIVER FUNCTIONS
//============================================================================

void irq_wait() {
	while (!is_floppy_irq);
	is_floppy_irq = false; 
}

static void floppy_dma_init(floppy_dir dir) {
	union {
		unsigned char b[4]; // 4 bytes
		unsigned long l;    // 1 long = 32-bit
	} a, c; // address and count

	a.l = (unsigned)&floppy_dmabuf;
	c.l = (unsigned)FLOPPY_DMA_BUFFER_SIZE - 1; // -1 because of DMA counting

	// check that address is at most 24-bits (under 16MB)
	// check that count is at most 16-bits (DMA limit)
	// check that if we add count and address we don't get a carry
	// (DMA can't deal with such a carry, this is the 64k boundary limit)
	if ((a.l >> 24) || (c.l >> 16) || (((a.l & 0xffff) + c.l) >> 16))
		kernel_panic(EXC_DRIVER, (char*)"floppy_dma_init: static buffer problem\n");

	unsigned char mode;
	switch (dir) {
		// 01:0:0:01:10 = single/inc/no-auto/to-mem/chan2
		case floppy_dir_read:  mode = 0x46; break;
		// 01:0:0:10:10 = single/inc/no-auto/from-mem/chan2
		case floppy_dir_write: mode = 0x4a; break;
		default: kernel_panic(EXC_DRIVER, (char*)"floppy_dma_init: invalid direction"); return;
	}

	outb(0x0a, 0x06);   // mask chan 2

	outb(0x0c, 0xff);   // reset flip-flop
	outb(0x04, a.b[0]); //  - address low byte
	outb(0x04, a.b[1]); //  - address high byte

	outb(0x81, a.b[2]); // external page register

	outb(0x0c, 0xff);   // reset flip-flop
	outb(0x05, c.b[0]); //  - count low byte
	outb(0x05, c.b[1]); //  - count high byte

	outb(0x0b, mode);   // set mode (see above)

	outb(0x0a, 0x02);   // unmask chan 2
}

void floppy_write_cmd(int base, char cmd) {
	for (int i = 0; i < 600; i++) {	// do timeout, 60 seconds
		sleep(1); // sleep 10 ms
		if (0x80 & inb(base + FLOPPY_MSR))
			return (void)outb(base + FLOPPY_FIFO, cmd);
	}
	kernel_panic(EXC_DRIVER, (char*)"floppy_write_cmd: timeout");
}

unsigned char floppy_read_data(int base) {
	for (int i = 0; i < 600; i++) {	// do timeout, 60 seconds
		sleep(1); // sleep 10 ms
		if (0x80 & inb(base + FLOPPY_MSR))
			return inb(base + FLOPPY_FIFO);
		
	}
	kernel_panic(EXC_DRIVER, (char*)"floppy_read_data: timeout");
	return 0; // not reached
}

void floppy_check_interrupt(int base, int *st0, int *cyl) {
	floppy_write_cmd(base, CMD_SENSE_INTERRUPT);

	*st0 = floppy_read_data(base);
	*cyl = floppy_read_data(base);
}

void floppy_motor(int base, int onoff) {
	if (onoff) {
		if (!floppy_motor_state) {
			// need to turn on
			outb(base + FLOPPY_DOR, 0x1c);
			sleep(50); // wait 500 ms = hopefully enough for modern drives
		}
		floppy_motor_state = floppy_motor_on;
	} else {
		if (floppy_motor_state == floppy_motor_wait)
			kernel_panic(EXC_DRIVER, (char*)"floppy_motor: strange, fd motor-state already waiting..\n");
		
		floppy_motor_ticks = 300; // 3 seconds, see floppy_timer() below
		floppy_motor_state = floppy_motor_wait;
	}
}

void floppy_motor_kill(int base) {
	outb(base + FLOPPY_DOR, 0x0c);
	floppy_motor_state = floppy_motor_off;
}

// Move to cylinder 0, which calibrates the drive..
void floppy_calibrate(int base) {
	int st0, cyl = -1; // set to bogus cylinder

	floppy_motor(base, floppy_motor_on);

	for (int i = 0; i < 10; i++) {
		// Attempt to positions head to cylinder 0
		floppy_write_cmd(base, CMD_RECALIBRATE);
		floppy_write_cmd(base, 0); // argument is drive, we only support 0

		irq_wait();
		floppy_check_interrupt(base, &st0, &cyl);

		if (st0 & 0xC0) {
			static const char * status[] = { 0, "error", "invalid", "drive" };
			char buff[50] = "floppy_calibrate: status = ";
			strcat(buff, status[st0 >> 6]);
			kernel_panic(EXC_DRIVER, buff);
			continue;
		}

		if (!cyl) { // found cylinder 0 ?
			floppy_motor(base, floppy_motor_off);
			return;
		}
	}

	floppy_motor(base, floppy_motor_off);
	kernel_panic(EXC_DRIVER, (char*)"floppy_calibrate: 10 retries exhausted\n");
}

void floppy_reset(int base) {

	outb(base + FLOPPY_DOR, 0x00); // disable controller
	outb(base + FLOPPY_DOR, 0x0C); // enable controller

	irq_wait(); // sleep until interrupt occurs

	int st0, cyl; // ignore these here..
	floppy_check_interrupt(base, &st0, &cyl);

	// set transfer speed 500kb/s
	outb(base + FLOPPY_CCR, 0x00);

	//  - 1st byte is: bits[7:4] = steprate, bits[3:0] = head unload time
	//  - 2nd byte is: bits[7:1] = head load time, bit[0] = no-DMA
	//
	//  steprate    = (8.0ms - entry*0.5ms)*(1MB/s / xfer_rate)
	//  head_unload = 8ms * entry * (1MB/s / xfer_rate), where entry 0 -> 16
	//  head_load   = 1ms * entry * (1MB/s / xfer_rate), where entry 0 -> 128
	//
	floppy_write_cmd(base, CMD_SPECIFY);
	floppy_write_cmd(base, 0xdf); /* steprate = 3ms, unload time = 240ms */
	floppy_write_cmd(base, 0x02); /* load time = 16ms, no-DMA = 0 */

	// it could fail...
	floppy_calibrate(base);
}

// Seek for a given cylinder, with a given head
void floppy_seek(int base, unsigned cyli, int head) {
	int st0, cyl = -1; // set to bogus cylinder

	floppy_motor(base, floppy_motor_on);

	for (int i = 0; i < 10; i++) {
		// Attempt to position to given cylinder
		// 1st byte bit[1:0] = drive, bit[2] = head
		// 2nd byte is cylinder number
		floppy_write_cmd(base, CMD_SEEK);
		floppy_write_cmd(base, head << 2);
		floppy_write_cmd(base, cyli);

		irq_wait();
		floppy_check_interrupt(base, &st0, &cyl);

		if (st0 & 0xC0) continue;

		if ((unsigned)cyl == cyli) {
			floppy_motor(base, floppy_motor_off);
			return;
		}
	}

	floppy_motor(base, floppy_motor_off);
	kernel_panic(EXC_DRIVER, (char*)"floppy_seek: 10 retries exhausted\n");
}

void floppy_do_track(int base, unsigned cyl, floppy_dir dir) {
	// transfer command, set below
	unsigned char cmd = 0;

	// Read is MT:MF:SK:0:0:1:1:0, write MT:MF:0:0:1:0:1
	// where MT = multitrack, MF = MFM mode, SK = skip deleted
	// Specify multitrack and MFM mode
	static const int flags = 0xC0;
	switch (dir) {
		case floppy_dir_read: cmd = CMD_READ_DATA | flags; break;
		case floppy_dir_write: cmd = CMD_WRITE_DATA | flags; break;
		default: kernel_panic(EXC_DRIVER, (char*)"floppy_do_track: invalid direction");
	}

	// seek both heads
	floppy_seek(base, cyl, 0);
	floppy_seek(base, cyl, 1);

	floppy_motor(base, floppy_motor_on);

	// init dma..
	floppy_dma_init(dir);

	sleep(10); // give some time (100ms) to settle after the seeks

	floppy_write_cmd(base, cmd);  // set above for current direction
	floppy_write_cmd(base, 0);    // 0:0:0:0:0:HD:US1:US0 = head and drive
	floppy_write_cmd(base, cyl);  // cylinder
	floppy_write_cmd(base, 0);    // first head (should match with above)
	floppy_write_cmd(base, 1);    // first sector, strangely counts from 1
	floppy_write_cmd(base, 2);    // bytes/sector, 128*2^x (x=2 -> 512)
	floppy_write_cmd(base, 18);   // number of tracks to operate on
	floppy_write_cmd(base, 0x1b); // GAP3 length, 27 is default for 3.5"
	floppy_write_cmd(base, 0xff); // data length (0xff if B/S != 0)

	irq_wait(); // don't SENSE_INTERRUPT here!

	unsigned char st0, st1, st2, bps; //, rcy, rhe, rse;
	st0 = floppy_read_data(base);
	st1 = floppy_read_data(base);
	st2 = floppy_read_data(base);
		
	/*
	* These are cylinder/head/sector values, updated with some
	* rather bizarre logic, that I would like to understand.
	*
	*
	rcy = floppy_read_data(base);
	rhe = floppy_read_data(base);
	rse = floppy_read_data(base);
	*/

	// bytes per sector, should be what we programmed in
	bps = floppy_read_data(base);
	
	char status_msg[30];
	int error = 0;

	if (st0 & 0xC0)
		strcat(status_msg, "floppy_do_sector: status = "), error = 1;
	if (st1 & 0x80) 
		strcat(status_msg, "floppy_do_sector: end of cylinder\n"), error = 1;
	if (st0 & 0x08) 
		strcat(status_msg, "floppy_do_sector: drive not ready\n"), error = 1;
	if (st1 & 0x20) 
		strcat(status_msg, "floppy_do_sector: CRC error\n"), error = 1;
	if (st1 & 0x10) 
		strcat(status_msg, "floppy_do_sector: controller timeout\n"), error = 1;
	if (st1 & 0x04) 
		strcat(status_msg, "floppy_do_sector: no data found\n"), error = 1;
	if ((st1 | st2) & 0x01)
		strcat(status_msg, "floppy_do_sector: no address mark found\n"), error = 1;
	if (st2 & 0x40) 
		strcat(status_msg, "floppy_do_sector: deleted address mark\n"), error = 1;
	if (st2 & 0x20) 
		strcat(status_msg, "floppy_do_sector: CRC error in data\n"), error = 1;
	if (st2 & 0x10) 
		strcat(status_msg, "floppy_do_sector: wrong cylinder\n"), error = 1;
	if (st2 & 0x04)
		strcat(status_msg, "floppy_do_sector: uPD765 sector not found\n"), error = 1;
	if (st2 & 0x02) 
		strcat(status_msg, "floppy_do_sector: bad cylinder\n"), error = 1;
	//if (bps != 0x2) 
	//	strcat(status_msg, "floppy_do_sector: wanted 512B/sector"), error = 1;
	if (st1 & 0x02) 
		strcat(status_msg, "floppy_do_sector: not writable\n"), error = 1;
	
	floppy_motor(base, floppy_motor_off);

	if (!error) return;
	else kernel_panic(EXC_DRIVER, status_msg);
}

void floppy_irq_callback(registers_t regs) {
	is_floppy_irq = 1;
}

//============================================================================
//    INTERFACE FUNCTIONS
//============================================================================

char* floppy_read_track(int base, unsigned cyl) {
	floppy_reset(base);
	floppy_do_track(base, cyl, floppy_dir_read);
	return floppy_dmabuf;
}

void floppy_deep_read(char* buffer) {
	for (unsigned i = 0; i < 1; i++) {
		floppy_read_track(floppy_base, i);
		memcpy(buffer + (i*FLOPPY_DMA_BUFFER_SIZE), floppy_dmabuf, FLOPPY_DMA_BUFFER_SIZE);
	}
}

void floppy_write_track(int base, unsigned cyl) {
	floppy_do_track(base, cyl, floppy_dir_write);
}

const char * dr_floppy_detect_drives() {
	outb(0x70, 0x10);
	return drive_types[inb(0x71) >> 4];
}

void dr_flpy_initialize(int dma_addr) {
	flpydsk_set_working_drive(0);
	flpydsk_install();
	floppy_reset(floppy_base);
}

//! install floppy driver
void flpydsk_install() {
	install_interrupt_handler(PIC_IRQ_PARALLEL1, &floppy_irq_callback);	
}

//! set current working drive
void flpydsk_set_working_drive(uint8_t drive) {
	if (drive < 4 && drive>=0) current_drive = drive;
}

//! get current working drive
uint8_t flpydsk_get_working_drive() {
	return current_drive;
}