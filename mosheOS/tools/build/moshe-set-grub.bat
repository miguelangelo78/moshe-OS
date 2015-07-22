:: COPY GRUB BOOTLOADER TO FLOPPY:
	cd ../../
	dd_ if=./moshe-boot/grub/grub_disk.img of=\\.\a:
	cp 	./moshe-boot/grub/menu.cfg a:/boot/menu.cfg
	cd tools/build