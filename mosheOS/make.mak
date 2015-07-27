#PREFIXES:
	# P = PATH
	# N = VARIABLE/FILE NAME
	# F = FILE FORMAT
	# E = EXECUTABLE
	# A = ARGUMENTS/FLAGS
	
# SET VARIABLES:
	#OS-PATHS
		P_ROOT=$(CURDIR)
		P_SYSTEM=moshe-system
		P_LIBS=$(P_SYSTEM)/libraries
		P_KRNL=$(P_SYSTEM)/kernel
		P_BIN=bin
		
	# FILE FORMATS
		F_CC=cpp
		F_ASM=asm
		F_OBJ=o
		F_BIN=bin
	
	#SOURCE FILES AND OTHER VARS:
		#NAME OF THE KERNEL BINARY FILE:
			N_KERNEL_OUT=moshe-kernel
		SOURCES= $(filter %.$(F_CC) , $(shell find ./$(P_SYSTEM) -type f))
		OBJECTS=$(notdir $(SOURCES:.$(F_CC)=.$(F_CC).$(F_OBJ)))
		ASMSRC=$(filter %.$(F_ASM) , $(shell find ./$(P_SYSTEM) -type f))
		ASMOBJS=$(notdir $(ASMSRC:.$(F_ASM)=.$(F_OBJ)))
			
	# COMPILER, ASSEMBLER AND LINKER
		E_CC=i586-elf-g++
		E_LD=i586-elf-ld
		E_ASM=nasm
	
	# COMPILER, ASSEMBLER AND LINKER ARGS/FLAGS
		A_CFLAGS=-Wno-unknown-pragmas -Wall -O -fstrength-reduce -fomit-frame-pointer -nostdinc -fno-builtin -I$(P_LIBS)/include -fno-strict-aliasing -fno-common -fno-stack-protector -c
		A_ASMFLAGS=-f elf
clean:
	rm *.o bin

launch: setgrub 
	@printf "\n===========\n LAUNCHING VIRTUALBOX...\n===========\n\n"
	@"/cygdrive/C/Program Files/Oracle/VirtualBox/VBoxManage.exe" startvm mosheOS
	
setgrub:
	@printf "===========\n LOADING KERNEL\n===========\n\n"
	@cd "$(CURDIR)/moshe-boot" && \
		./moshe-set-grub.bat
	@cp "$(CURDIR)/bin/moshe-kernel.bin" /cygdrive/a/moshe-kernel.bin
	@cp "$(CURDIR)/moshe-boot/grub/initrd.img" /cygdrive/a/initrd.img

all: $(ASMSRC:.$(F_ASM)=.$(F_ASM).o) $(SOURCES:.$(F_CC)=.$(F_CC).o)
	cd $(P_BIN) && \
		$(E_LD) -T "$(CURDIR)/link.ld" -o $(N_KERNEL_OUT).$(F_BIN) $(ASMOBJS) $(OBJECTS) 

%.$(F_ASM).o:
	$(E_ASM) $(A_ASMFLAGS) -o $(P_BIN)/$(notdir $(@:.$(F_ASM).o=.o)) $(@:.o=)
	
%.o:
	$(E_CC) $(A_CFLAGS) -o $(P_BIN)/$(notdir $@) $(@:.o=)