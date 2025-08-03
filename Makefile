# Set the C compiler and debugger
CC = clang
GDB = gdb 

LD = ld.lld
ASM = nasm

INC = -I kernel/inc/
LIBS = $(shell find kernel/src/**/ -type f -name "*.c")
ASM_SRC = $(shell find . -type f -name "*.asm")

KERNEL = $(shell find kernel/src/ -type f -name "kernel.c")

ASM_BOOT = bootloader/boot.asm
BOOT_O = bootloader/boot.o
ASM_ENTRY = $(shell find kernel/ -type f -name "entry.asm")

LINKER = $(shell find . -type f -name "linker.ld")

ASM_KERNEL = $(shell find kernel/src/ -type f -name "*.asm")

# CRITICAL: assembly must be linked first
OBJ = ${ASM_BOOT:.asm=.o} ${ASM_ENTRY:.asm=.o} ${KERNEL:.c=.o} ${ASM_KERNEL:.c=.o} ${LIBS:.c=.o} 

CCFLAGS = -ggdb -c -ffreestanding -target x86_64-none-elf
LDFLAGS = -Ttext 0x10000
LDFLAGS_BIN = --oformat binary
ASFLAGS = -f elf64 -i bootloader/

all: moveboot kernelasm.o ${OBJ} tinyos.img

tinyos.img: ${ASM_ENTRY:.asm=.o} ${KERNEL:.c=.o} ${ASM_KERNEL:.asm=.o} ${LIBS:.c=.o} 
	${LD} -o $@ ${LDFLAGS_BIN} ${LINKER} boot $^

%.o : %.c
	${CC} ${CCFLAGS} ${INC} -o $@ $^

%.o: %.asm
	${ASM} $< ${ASFLAGS} -o $@

moveboot: ${BOOT_O}
	mv $< boot

kernelasm.o: ${ASM_KERNEL}
	${ASM} $< -f elf64 -o $@

clean:
	rm -f boot *.o **/*.o **/**/*.o **/**/**/*.o

