#include "drvr_floppy.h"
#include "error\panic.h"
#include "task\thread.h"

volatile static uint32_t _FloppyDiskIRQ = 0;
static uint8_t	_CurrentDrive = 0;
const int FLPY_SECTORS_PER_TRACK = 18;
const int FDC_DMA_CHANNEL = 2;
uint32_t DMA_BUFFER = 0x1000;

/********** DRIVER FUNCTIONS **********/
//****************************************************************************
//**
//**    FLPY.cpp
//**		-Floppy Disk driver
//**
//****************************************************************************
//============================================================================
//    IMPLEMENTATION HEADERS
//============================================================================

//============================================================================
//    IMPLEMENTATION PRIVATE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//============================================================================

/*
**	Controller I/O Ports. Please see chapter for additional ports
*/

enum FLPY_IO {

	FLPY_DOR = 0x3f2,
	FLPY_MSR = 0x3f4,
	FLPY_FIFO = 0x3f5,
	FLPY_CTRL = 0x3f7
};

/**
*	Bits 0-4 of command byte. Please see chapter for additional commands
*/

enum FLPY_CMD {

	FDC_CMD_READ_TRACK = 2,
	FDC_CMD_SPECIFY = 3,
	FDC_CMD_CHECK_STAT = 4,
	FDC_CMD_WRITE_SECT = 5,
	FDC_CMD_READ_SECT = 6,
	FDC_CMD_CALIBRATE = 7,
	FDC_CMD_CHECK_INT = 8,
	FDC_CMD_FORMAT_TRACK = 0xd,
	FDC_CMD_SEEK = 0xf
};

/**
*	Additional command masks. Can be masked with above commands
*/

enum FLPY_CMD_EXT {

	FDC_CMD_EXT_SKIP = 0x20,	//00100000
	FDC_CMD_EXT_DENSITY = 0x40,	//01000000
	FDC_CMD_EXT_MULTITRACK = 0x80	//10000000
};

/*
**	Digital Output Register
*/

enum FLPY_DOR_MASK {

	FLPY_DOR_MASK_DRIVE0 = 0,	//00000000	= here for completeness sake
	FLPY_DOR_MASK_DRIVE1 = 1,	//00000001
	FLPY_DOR_MASK_DRIVE2 = 2,	//00000010
	FLPY_DOR_MASK_DRIVE3 = 3,	//00000011
	FLPY_DOR_MASK_RESET = 4,	//00000100
	FLPY_DOR_MASK_DMA = 8,	//00001000
	FLPY_DOR_MASK_DRIVE0_MOTOR = 16,	//00010000
	FLPY_DOR_MASK_DRIVE1_MOTOR = 32,	//00100000
	FLPY_DOR_MASK_DRIVE2_MOTOR = 64,	//01000000
	FLPY_DOR_MASK_DRIVE3_MOTOR = 128	//10000000
};

/**
*	Main Status Register
*/

enum FLPY_MSR_MASK {

	FLPY_MSR_MASK_DRIVE1_POS_MODE = 1,	//00000001
	FLPY_MSR_MASK_DRIVE2_POS_MODE = 2,	//00000010
	FLPY_MSR_MASK_DRIVE3_POS_MODE = 4,	//00000100
	FLPY_MSR_MASK_DRIVE4_POS_MODE = 8,	//00001000
	FLPY_MSR_MASK_BUSY = 16,	//00010000
	FLPY_MSR_MASK_DMA = 32,	//00100000
	FLPY_MSR_MASK_DATAIO = 64, //01000000
	FLPY_MSR_MASK_DATAREG = 128	//10000000
};

/**
*	Controller Status Port 0
*/

enum FLPY_ST0_MASK {

	FLPY_ST0_MASK_DRIVE0 = 0,		//00000000	=	for completness sake
	FLPY_ST0_MASK_DRIVE1 = 1,		//00000001
	FLPY_ST0_MASK_DRIVE2 = 2,		//00000010
	FLPY_ST0_MASK_DRIVE3 = 3,		//00000011
	FLPY_ST0_MASK_HEADACTIVE = 4,		//00000100
	FLPY_ST0_MASK_NOTREADY = 8,		//00001000
	FLPY_ST0_MASK_UNITCHECK = 16,		//00010000
	FLPY_ST0_MASK_SEEKEND = 32,		//00100000
	FLPY_ST0_MASK_INTCODE = 64		//11000000
};

/*
** LPYDSK_ST0_MASK_INTCODE types
*/

enum FLPY_ST0_INTCODE_TYP {

	FLPY_ST0_TYP_NORMAL = 0,
	FLPY_ST0_TYP_ABNORMAL_ERR = 1,
	FLPY_ST0_TYP_INVALID_ERR = 2,
	FLPY_ST0_TYP_NOTREADY = 3
};

/**
*	GAP 3 sizes
*/

enum FLPY_GAP3_LENGTH {

	FLPY_GAP3_LENGTH_STD = 42,
	FLPY_GAP3_LENGTH_5_14 = 32,
	FLPY_GAP3_LENGTH_3_5 = 27
};

/*
**	Formula: 2^sector_number * 128, where ^ denotes "to the power of"
*/

enum FLPY_SECTOR_DTL {

	FLPY_SECTOR_DTL_128 = 0,
	FLPY_SECTOR_DTL_256 = 1,
	FLPY_SECTOR_DTL_512 = 2,
	FLPY_SECTOR_DTL_1024 = 4
};

bool dma_initialize_floppy(uint8_t *buffer, unsigned length) {
	union {
		uint8_t byte[4];
		unsigned long l;
	}a, c;

	a.l = (unsigned)buffer;
	c.l = (unsigned)length - 1;

	//Check for buffer issues
	if ((a.l >> 24) || (c.l >> 16) || (((a.l & 0xffff) + c.l) >> 16)) return false;

	dma_reset(1);
	dma_mask_channel(FDC_DMA_CHANNEL);
	dma_reset_flipflop(1);

	dma_set_address(FDC_DMA_CHANNEL, a.byte[0], a.byte[1]);
	dma_reset_flipflop(1);

	dma_set_count(FDC_DMA_CHANNEL, c.byte[0], c.byte[1]);
	dma_set_read(FDC_DMA_CHANNEL);

	dma_unmask_all(1);

	return true;
}

void flpy_write_dor(uint8_t val) {
	outb(FLPY_DOR, val);
}

uint8_t flpy_read_status() {
	return inb(FLPY_MSR);
}

void flpy_send_command(uint8_t cmd) {
	//! wait until data register is ready. We send commands to the data register
	for (int i = 0; i < 500; i++)
		if (flpy_read_status() & FLPY_MSR_MASK_DATAREG)
			return outb(FLPY_FIFO, cmd);
}

uint8_t flpy_read_data() {
	//! same as above function but returns data register for reading
	for (int i = 0; i < 500; i++)
		if (flpy_read_status() & FLPY_MSR_MASK_DATAREG)
			return inb(FLPY_FIFO);
	return 0;
}

void flpy_write_ccr(uint8_t val) {
	//! write the configuation control
	outb(FLPY_CTRL, val);
}

static void dr_floppy_callback(registers_t regs) {  
	_FloppyDiskIRQ = 1;	// Interrupt has been triggered
}

inline void flpy_wait_irq() {
	//! wait for irq to fire
	while (!_FloppyDiskIRQ);
	_FloppyDiskIRQ = 0;
}

/**
*	Controller Command Routines
*/

//! check interrupt status command
void flpy_check_int(uint32_t* st0, uint32_t* cyl) {

	flpy_send_command(FDC_CMD_CHECK_INT);

	*st0 = flpy_read_data();
	*cyl = flpy_read_data();
}

//! turns the current floppy drives motor on/off
void flpy_control_motor(bool b) {

	//! sanity check: invalid drive
	if (_CurrentDrive > 3) return;

	uint32_t motor = 0;

	//! select the correct mask based on current drive
	switch (_CurrentDrive) {
		case 0:
			motor = FLPY_DOR_MASK_DRIVE0_MOTOR;
			break;
		case 1:
			motor = FLPY_DOR_MASK_DRIVE1_MOTOR;
			break;
		case 2:
			motor = FLPY_DOR_MASK_DRIVE2_MOTOR;
			break;
		case 3:
			motor = FLPY_DOR_MASK_DRIVE3_MOTOR;
			break;
	}

	//! turn on or off the motor of that drive
	if (b)
		flpy_write_dor(_CurrentDrive | motor | FLPY_DOR_MASK_RESET | FLPY_DOR_MASK_DMA);
	else
		flpy_write_dor(FLPY_DOR_MASK_RESET);

	//! in all cases; wait a little bit for the motor to spin up/turn off
	sleep(20);
}

//! configure drive
void flpy_drive_data(uint32_t stepr, uint32_t loadt, uint32_t unloadt, bool dma) {
	uint32_t data = 0;

	//! send command
	flpy_send_command(FDC_CMD_SPECIFY);
	data = ((stepr & 0xf) << 4) | (unloadt & 0xf);
	flpy_send_command(data);
	data = (loadt) << 1 | (dma == false) ? 0 : 1;
	flpy_send_command(data);
}

//! calibrates the drive
int flpy_calibrate(uint32_t drive) {

	uint32_t st0, cyl;

	if (drive >= 4)
		return -2;

	//! turn on the motor
	flpy_control_motor(true);

	for (int i = 0; i < 10; i++) {

		//! send command
		flpy_send_command(FDC_CMD_CALIBRATE);
		flpy_send_command(drive);
		flpy_wait_irq();
		flpy_check_int(&st0, &cyl);

		//! did we fine cylinder 0? if so, we are done
		if (!cyl) {
			flpy_control_motor(false);
			return 0;
		}
	}

	flpy_control_motor(false);
	return -1;
}

//! disable controller
void flpy_disable_controller() {
	flpy_write_dor(0);
}

//! enable controller
void flpy_enable_controller() {
	flpy_write_dor(FLPY_DOR_MASK_RESET | FLPY_DOR_MASK_DMA);
}

//! reset controller
void flpy_reset() {
	uint32_t st0, cyl;

	//! reset the controller	
	flpy_disable_controller();
	flpy_enable_controller();
	flpy_wait_irq();

	//! send CHECK_INT/SENSE INTERRUPT command to all drives
	for (int i = 0; i<4; i++)
		flpy_check_int(&st0, &cyl);

	//! transfer speed 500kb/s
	flpy_write_ccr(0);

	//! pass mechanical drive info. steprate=3ms, unload time=240ms, load time=16ms
	flpy_drive_data(3, 16, 240, true);

	//! calibrate the disk
	flpy_calibrate(_CurrentDrive);
}

void flpy_read_sector_imp(uint8_t head, uint8_t track, uint8_t sector) {
	uint32_t st0, cyl;

	//! set the DMA for read transfer
	if (!dma_initialize_floppy((uint8_t*)DMA_BUFFER, 512))
		kernel_panic(EXC_DMA_BADBUFF, (char*)"");

	// set dma for read transfer
	dma_set_read(FDC_DMA_CHANNEL);

	//! read in a sector
	flpy_send_command(FDC_CMD_READ_SECT | FDC_CMD_EXT_MULTITRACK | FDC_CMD_EXT_SKIP | FDC_CMD_EXT_DENSITY);
	flpy_send_command(head << 2 | _CurrentDrive);
	flpy_send_command(track);
	flpy_send_command(head);
	flpy_send_command(sector);
	flpy_send_command(FLPY_SECTOR_DTL_512);
	flpy_send_command(((sector + 1) >= FLPY_SECTORS_PER_TRACK) ? FLPY_SECTORS_PER_TRACK : sector + 1);
	flpy_send_command(FLPY_GAP3_LENGTH_3_5);
	flpy_send_command(0xff);

	//! wait for irq
	flpy_wait_irq();

	//! read status info
	for (int j = 0; j<7; j++)
		flpy_read_data();

	//! let FDC know we handled interrupt
	flpy_check_int(&st0, &cyl);
}

//! seek to given track/cylinder
int flpy_seek(uint32_t cyl, uint32_t head) {

	uint32_t st0, cyl0;

	if (_CurrentDrive >= 4)
		return -1;

	for (int i = 0; i < 10; i++) {

		//! send the command
		flpy_send_command(FDC_CMD_SEEK);
		flpy_send_command((head) << 2 | _CurrentDrive);
		flpy_send_command(cyl);

		//! wait for the results phase IRQ
		flpy_wait_irq();
		flpy_check_int(&st0, &cyl0);

		//! found the cylinder?
		if (cyl0 == cyl)
			return 0;
	}

	return -1;
}

/********** INTERFACE FUNCTIONS **********/

void dr_floppy_dma_set_buffer_loc(uint32_t addr) {
	DMA_BUFFER = addr;
}

void dr_floppy_initialize(uint32_t dma_buffer_loc) {
	dr_floppy_set_working_drive(0);
	dr_floppy_dma_set_buffer_loc(dma_buffer_loc);
	dr_floppy_install();
}

void dr_floppy_install() {
	install_interrupt_handler(PIC_IRQ_PARALLEL1, &dr_floppy_callback);
	flpy_reset();
	flpy_drive_data(13, 1, 0xF, true);
}

void dr_floppy_set_working_drive(uint8_t drive) {
	if (drive < 4 && drive>=0) _CurrentDrive = drive;
}

uint8_t dr_floppy_get_working_drive() {
	return _CurrentDrive;
}

void dr_floppy_lba_to_chs(int lba, int * head, int *track, int * sector) {
	*head = (lba % (FLPY_SECTORS_PER_TRACK * 2)) / (FLPY_SECTORS_PER_TRACK);
	*track = lba / (FLPY_SECTORS_PER_TRACK * 2);
	*sector = lba % FLPY_SECTORS_PER_TRACK + 1;
}

uint8_t * dr_floppy_read_sector(int sectorLBA) {
	if (_CurrentDrive >= 4) return 0;

	//! convert LBA sector to CHS
	int head = 0, track = 0, sector = 1;
	dr_floppy_lba_to_chs(sectorLBA, &head, &track, &sector);

	//! turn motor on and seek to track
	flpy_control_motor(true);
	
	if (flpy_seek(track, head) != 0) return 0;
	
	//! read sector and turn motor off
	flpy_read_sector_imp(head, track, sector);
	flpy_control_motor(false);

	//! warning: this is a bit hackish
	return (uint8_t*)DMA_BUFFER;
}