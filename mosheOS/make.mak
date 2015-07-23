
# SET VARIABLES:
	#KERNEL LOADER, KERNEL AND OTHER CPP Files
		KERNEL_OUT=moshe-kernel
		KERNEL_MAIN=moshe-main
		KERNEL_LDR=kernel_loader
	
	# COMPILER, ASSEMBLER AND LINKER
		CC=i586-elf-g++
		LD=i586-elf-ld
		ASM=nasm
	
	# COMPILER, ASSEMBLER AND LINKER ARGS
		CFLAGS=-Wall -O -fstrength-reduce -fomit-frame-pointer -nostdinc -fno-builtin -I/libraries/include -fno-strict-aliasing -fno-common -fno-stack-protector -c
		ASMFLAGS=-f elf
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

build: $(KERNEL_LDR).o $(KERNEL_MAIN).o foo.o
	cd bin && \
		$(LD) -o "$(CURDIR)/bin/$(KERNEL_OUT).bin" $(KERNEL_LDR).o $(KERNEL_MAIN).o foo.o 
	
$(KERNEL_LDR).o:
	$(ASM) $(ASMFLAGS) -o "$(CURDIR)/bin/$(KERNEL_LDR).o" "$(CURDIR)/moshe-system/kernel/$(KERNEL_LDR).asm"
	
$(KERNEL_MAIN).o:
	$(CC) $(CFLAGS) -o bin/$(KERNEL_MAIN).o moshe-system/kernel/$(KERNEL_MAIN).cpp
	
foo.o:	
	$(CC) $(CFLAGS) -o "$(CURDIR)/bin/foo.o" "$(CURDIR)/moshe-system/kernel/foo.cpp"

	