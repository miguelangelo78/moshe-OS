:: ASSEMBLE STAGE1:
	call build_bootloader_stg1.bat

:: ASSEMBLE STAGE2:
   	call build_bootloader_stg2.bat

:: CONVERT BINARY FILES INTO .IMG DISK FILE
	:: BACK TO PROJECT ROOT DIRECTORY
		cd ../ 

	:: COPY STAGE 1 TO BOOTSECTOR:
		dd_ if=./bin/moshe-boot/moshe-bootloader-stg1.bin of=\\.\a: count=1 bs=512
	:: COPY STAGE 2 TO FLOPPY:
		cp "./bin/moshe-boot/KRNLDR.SYS" A:\KRNLDR.SYS
	
:: LET PROGRAMMER SEE THE OUTPUT OF THE ASSEMBLING, AND WAIT FOR HIM TO PRESS ENTER
	PAUSE

:: LAUNCH BOCHS:
	cd os-tools/bochs-config/
	start moshe-boch.bxrc

:: RETURN TO ORIGINAL PATH:
	cd ../../build