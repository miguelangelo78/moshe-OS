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
		SOURCES= $(filter %.$(F_CC) , $(shell find ./$(P_SYSTEM) -type f))
		OBJECTS=$(notdir $(SOURCES:.$(F_CC)=.$(F_CC).$(F_OBJ)))
		N_KERNEL_OUT=moshe-kernel
		N_KERNEL_LDR=kernel_loader
			
	# COMPILER, ASSEMBLER AND LINKER
		E_CC=i586-elf-g++
		E_LD=i586-elf-ld
		E_ASM=nasm
	
	# COMPILER, ASSEMBLER AND LINKER ARGS
		A_CFLAGS=-Wall -O -fstrength-reduce -fomit-frame-pointer -nostdinc -fno-builtin -I$(P_LIBS)/include -fno-strict-aliasing -fno-common -fno-stack-protector -c
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

all: $(N_KERNEL_LDR).o $(SOURCES:.cpp=.cpp.o)
	@printf "\n\n"
	@echo $(SOURCES)
	@printf "\n\n"
	
	cd $(P_BIN) && \
		$(E_LD) -o $(N_KERNEL_OUT).$(F_BIN) $(N_KERNEL_LDR).$(F_OBJ) $(OBJECTS) 
	
$(N_KERNEL_LDR).o:
	$(E_ASM) $(A_ASMFLAGS) -o $(P_BIN)/$(N_KERNEL_LDR).$(F_OBJ) $(P_KRNL)/$(N_KERNEL_LDR).$(F_ASM)

%.o:
	$(E_CC) $(A_CFLAGS) -o $(P_BIN)/$(notdir $@) $(@:.o=)