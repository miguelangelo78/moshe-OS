:: GO TO ROOT PROJECT DIRECTOY
	cd ../

:: ASSEMBLE BOOTLOADER STAGE 1:
	nasm ./moshe-boot/moshe-bootloader-stg1.asm -f bin -o ./bin/moshe-boot/moshe-bootloader-stg1.bin

:: GO BACK TO BUILD FOLDER TO CONTINUE BUILD PROCESS
	cd build