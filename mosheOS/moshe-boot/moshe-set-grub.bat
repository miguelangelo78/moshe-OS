:: COPY GRUB BOOTLOADER TO FLOPPY:
	@dd_ if=./grub/grub_disk.img of=\\.\a:
	@cp 	./grub/menu.cfg a:/boot/menu.cfg