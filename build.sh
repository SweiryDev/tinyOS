#!/bin/bash

cd bootloader
nasm -f elf64 boot.asm -o boot
mv boot ../boot

cd ../kernel
nasm -f elf64 entry.asm -o entry.o
clang -ggdb -c -ffreestanding -mno-red-zone -target x86_64-none-elf -Iinc src/kernel.c src/cpu/ports.c src/driver/vga.c
# make # FIX IT!
mv entry.o ../entry.o
mv kernel.o ../kernel.o
mv ports.o ../ports.o
mv vga.o ../vga.o

cd ..
# Add the kernel.o 
ld.lld -n --oformat binary -T linker.ld boot entry.o kernel.o ports.o vga.o -o tinyos.img

rm boot entry.o kernel.o ports.o vga.o




