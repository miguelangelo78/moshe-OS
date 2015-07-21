:: ASSEMBLE BOOTLOADER:
	cd ../
	nasm ./moshe-boot/moshe-bootloader.asm -f bin -o ./bin/moshe-boot/moshe-bootloader.bin
	dd if=./bin/moshe-boot/moshe-bootloader.bin of=./os-tools/bochs-config/moshe-disk.img


:: LAUNCH BOCHS:


	cd os-tools/bochs-config/
	start moshe-boch.bxrc

:: RETURN TO ORIGINAL PATH:
	cd ../../build