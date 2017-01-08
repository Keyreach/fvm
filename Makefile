CC = gcc
VM_O = fvm.o
ASM_O = fvm_asm.o chars.o

all : vm asm

vm : $(VM_O)
	$(CC) -o fvm $(VM_O)

asm : $(ASM_O)
	$(CC) -o fvm_asm $(ASM_O)
	
fvm.o : fvm.c
	$(CC) -c fvm.c

fvm_asm.o : fvm_asm.c chars.h
	$(CC) -c fvm_asm.c
	
chars.o : chars.c chars.h
	$(CC) -c chars.c
	
clean:
	rm fvm fvm_asm *.o
