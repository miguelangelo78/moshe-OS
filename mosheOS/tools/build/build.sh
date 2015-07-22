#!/bin/bash/

# $1 = System files (where the kernel is)
# $2 = Build files (where the batch and shell files are, including the linker)
# $3 = Root folder (where all folders are accessible)

# SET VARIABLES:
	# KERNEL AND KERNEL LOADER FILENAMES (also linker):
		kernel_out_name=moshe-kernel
		kernel_name=moshe-main
		kernel_ldr_name=kernel_loader
		linker_name=linker

	# COMPILATION, ASSEMBLING AND LINKING
		crosscompiler=i586-elf-gcc
		crosslinker=i586-elf-ld
		assembler=nasm
		
	# COMPILER, ASSEMBLER AND LINKER ARGUMENTS
		crosscompiler_args='-Werror -Wall -O -fstrength-reduce -fomit-frame-pointer -nostdinc -fno-builtin -I"$1\kernel\include" -fno-strict-aliasing -fno-common -fno-stack-protector -c'
		assembler_args="-f elf"
	
# COMPILE KERNEL:
	$crosscompiler $crosscompiler_args "$1\kernel\\"$kernel_name".c" -o "$3\bin\\"$kernel_name".o";

# ASSEMBLE KERNEL LOADER:
	$assembler $assembler_args "$1\kernel\\"$kernel_ldr_name".asm" -o "$3\bin\\"$kernel_ldr_name".o";

# LINK THE TWO TOGETHER:
	$crosslinker -o "$2\\"$kernel_out_name".bin" -T "$2\\"$linker_name".ld" "$3\bin\\"$kernel_name".o" "$3\bin\\"$kernel_ldr_name".o";