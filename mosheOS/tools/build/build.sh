#!/bin/bash/

# $1 = System files (where the kernel is)
# $2 = Build files (where the batch and shell files are, including the linker)
# $3 = Root folder (where all folders are accessible)

# COMPILE KERNEL:
	i586-elf-gcc -Werror -Wall -O -fstrength-reduce -fomit-frame-pointer -nostdinc -fno-builtin -I"$1\kernel\include" -c -o "$3\bin\main.o" "$1\kernel\main.c" -fno-strict-aliasing -fno-common -fno-stack-protector;

# ASSEMBLE KERNEL LOADER:
	nasm -f elf "$1\kernel\loader.asm" -o "$3\bin\loader.o";

# LINK THE TWO TOGETHER:
	i586-elf-ld -o "$2\moshe-kernel.bin" -T "$2\linker.ld" "$3\bin\main.o" "$3\bin\loader.o";