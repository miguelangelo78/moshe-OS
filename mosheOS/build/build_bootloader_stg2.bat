:: GO TO ROOT PROJECT DIRECTOY
	cd ../

:: ASSEMBLE BOOTLOADER STAGE 2:
	nasm ./moshe-boot/moshe-bootloader-stg2.asm -f bin -o ./bin/moshe-boot/KRNLDR.SYS

:: GO BACK TO BUILD FOLDER TO CONTINUE BUILD PROCESS
	cd build