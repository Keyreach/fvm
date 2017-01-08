CC = gcc

all : vm asm

vm : fvm.o
	$(CC) -o fvm fvm.o

asm : fvm_asm.o chars.o
	$(CC) -o fvm_asm fvm_asm.o chars.o
	
fvm.o : fvm.c
	$(CC) -c fvm.c

fvm_asm.o : fvm_asm.c chars.h
	$(CC) -c fvm_asm.c
	
chars.o : chars.c chars.h
	$(CC) -c chars.c
	
clean:
	rm fvm fvm_asm fvm.o fvm_asm.o chars.o
