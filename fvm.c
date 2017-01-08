#include <stdio.h>


#define OP_MOVRI 0x2
#define OP_MOVRR 0x3

#define OP_ADRII 0x4
#define OP_ADRRI 0x6
#define OP_ADRRR 0x7

#define OP_SBRII 0xc
#define OP_SBRRI 0xe
#define OP_SBRRR 0xf

#define OP_SLRII 0x14
#define OP_SLRRI 0x16
#define OP_SLRRR 0x17

#define OP_SRRII 0x1c
#define OP_SRRRI 0x1e
#define OP_SRRRR 0x1f

#define OP_ORRII 0x24
#define OP_ORRRI 0x26
#define OP_ORRRR 0x27

#define OP_ANRII 0x2c
#define OP_ANRRI 0x2e
#define OP_ANRRR 0x2f

#define OP_XORII 0x34
#define OP_XORRI 0x36
#define OP_XORRR 0x37

// 011xxx
#define OP_LDRI  0x1a
#define OP_LDRR  0x1b
// 111xxx
#define OP_STRI  0x3a
#define OP_STRR  0x3b
// 100xxx
#define OP_CMPRI 0x22
#define OP_CMPRR 0x23
// 1000xxx
#define OP_JMPI 0x40
#define OP_JMPR 0x41
#define OP_JLTI 0x42
#define OP_JLTR 0x43
#define OP_JEQI 0x44
#define OP_JEQR 0x45
#define OP_JGTI 0x46
#define OP_JGTR 0x47
#define OP_JGEI 0x4a
#define OP_JGER 0x4b
#define OP_JNEI 0x4c
#define OP_JNER 0x4d
#define OP_JLEI 0x4e
#define OP_JLER 0x4f
// Stack
#define OP_PUSHI 0x8
#define OP_PUSHR 0x9
#define OP_POPI 0x10
#define OP_POPR 0x11

#define OP_STOP 0x0

#define REG 16
#define MEM 256

/*
char data[MEM] = {
	OP_MOVRI, 0, 2,
	OP_ADRRI, 0, 0, 4,
	OP_MOVRR, 1, 0,
	OP_ADRRR, 2, 1, 0,
	OP_MOVRR, 3, 2,
	OP_SBRRI, 4, 3, 4,
	OP_MOVRI, 5, 0xa0,
	OP_STRR, 4, 5,
	OP_LDRR, 7, 5,
	OP_STOP
};
*/

char data[MEM];
/* = {
	OP_MOVRI, 0, 10,
	OP_STRI, 0, 100,
	OP_SBRRI, 0, 0, 1,
	OP_LDRI, 1, 100,
	OP_ADRRR, 1, 1, 0,
	OP_STRI, 1, 100,
	OP_CMPRI, 0, 0,
	OP_JNE, 6,
	OP_STOP
};*/

char registers[REG];

struct {
    unsigned int eq: 1;
    unsigned int lt: 1;
    unsigned int gt: 1;
    unsigned int reserved: 9;
} flags;

void dump_registers(){
    int i;
    puts("Registers dump");
	for(i = 0; i < REG; i++){
		printf("R%02d ", i);
	}
	printf("\n");
	for(i = 0; i < REG; i++){
		printf("%3d ", registers[i] & 0xff);
	}
	printf("\n");
}

void vm_loop(){
    int overload = 10000, pc = 0;
    char op[3] = {0, 0, 0};
    while((data[pc] != OP_STOP) && overload){
        printf(".");
        switch(data[pc]){
// mov
            case OP_MOVRI:
                op[0] = data[++pc];
                op[1] = data[++pc];
                registers[op[0]] = op[1];
                break;
            case OP_MOVRR:
                op[0] = data[++pc];
                op[1] = data[++pc];
                registers[op[0]] = registers[op[1]];
                break;
// add
            case OP_ADRRI:
                op[0] = data[++pc];
                op[1] = data[++pc];
                op[2] = data[++pc];
                registers[op[0]] = registers[op[1]] + op[2];
                break;
            case OP_ADRRR:
                op[0] = data[++pc];
                op[1] = data[++pc];
                op[2] = data[++pc];
                registers[op[0]] = registers[op[1]] + registers[op[2]];
                break;
            case OP_ADRII:
                op[0] = data[++pc];
                op[1] = data[++pc];
                op[2] = data[++pc];
                registers[op[0]] = op[1] + op[2];
                break;
// sub
            case OP_SBRRI:
                op[0] = data[++pc];
                op[1] = data[++pc];
                op[2] = data[++pc];
                registers[op[0]] = registers[op[1]] - op[2];
                break;
            case OP_SBRRR:
                op[0] = data[++pc];
                op[1] = data[++pc];
                op[2] = data[++pc];
                registers[op[0]] = registers[op[1]] - registers[op[2]];
                break;
            case OP_SBRII:
                op[0] = data[++pc];
                op[1] = data[++pc];
                op[2] = data[++pc];
                registers[op[0]] = op[1] - op[2];
                break;
// shift left
            case OP_SLRII:
                op[0] = data[++pc];
                op[1] = data[++pc];
                op[2] = data[++pc];
                registers[op[0]] = op[1] << op[2];
                break;
            case OP_SLRRI:
                op[0] = data[++pc];
                op[1] = data[++pc];
                op[2] = data[++pc];
                registers[op[0]] = registers[op[1]] << op[2];
                break;
            case OP_SLRRR:
                op[0] = data[++pc];
                op[1] = data[++pc];
                op[2] = data[++pc];
                registers[op[0]] = registers[op[1]] << registers[op[2]];
                break;
// shift right
            case OP_SRRII:
                op[0] = data[++pc];
                op[1] = data[++pc];
                op[2] = data[++pc];
                registers[op[0]] = op[1] >> op[2];
                break;
            case OP_SRRRI:
                op[0] = data[++pc];
                op[1] = data[++pc];
                op[2] = data[++pc];
                registers[op[0]] = registers[op[1]] >> op[2];
                break;
            case OP_SRRRR:
                op[0] = data[++pc];
                op[1] = data[++pc];
                op[2] = data[++pc];
                registers[op[0]] = registers[op[1]] >> registers[op[2]];
                break;
// or
            case OP_ORRII:
                op[0] = data[++pc];
                op[1] = data[++pc];
                op[2] = data[++pc];
                registers[op[0]] = op[1] | op[2];
                break;
            case OP_ORRRI:
                op[0] = data[++pc];
                op[1] = data[++pc];
                op[2] = data[++pc];
                registers[op[0]] = registers[op[1]] | op[2];
                break;
            case OP_ORRRR:
                op[0] = data[++pc];
                op[1] = data[++pc];
                op[2] = data[++pc];
                registers[op[0]] = registers[op[1]] | registers[op[2]];
                break;
// and
            case OP_ANRII:
                op[0] = data[++pc];
                op[1] = data[++pc];
                op[2] = data[++pc];
                registers[op[0]] = op[1] & op[2];
                break;
            case OP_ANRRI:
                op[0] = data[++pc];
                op[1] = data[++pc];
                op[2] = data[++pc];
                registers[op[0]] = registers[op[1]] & op[2];
                break;
            case OP_ANRRR:
                op[0] = data[++pc];
                op[1] = data[++pc];
                op[2] = data[++pc];
                registers[op[0]] = registers[op[1]] & registers[op[2]];
                break;
// xor
            case OP_XORII:
                op[0] = data[++pc];
                op[1] = data[++pc];
                op[2] = data[++pc];
                registers[op[0]] = op[1] ^ op[2];
                break;
            case OP_XORRI:
                op[0] = data[++pc];
                op[1] = data[++pc];
                op[2] = data[++pc];
                registers[op[0]] = registers[op[1]] ^ op[2];
                break;
            case OP_XORRR:
                op[0] = data[++pc];
                op[1] = data[++pc];
                op[2] = data[++pc];
                registers[op[0]] = registers[op[1]] ^ registers[op[2]];
                break;
// load
            case OP_LDRI:
                op[0] = data[++pc];
                op[1] = data[++pc];
                registers[op[0]] = data[op[1]];
                break;
            case OP_LDRR:
                op[0] = data[++pc];
                op[1] = data[++pc];
                registers[op[0]] = data[registers[op[1]]];
                break;
// store
            case OP_STRI:
                op[0] = data[++pc];
                op[1] = data[++pc];
                data[op[1]] = registers[op[0]];
                break;
            case OP_STRR:
                op[0] = data[++pc];
                op[1] = data[++pc];
                data[registers[op[1]]] = registers[op[0]];
                break;
// push to stack
            case OP_PUSHI:
                op[0] = data[++pc];
                data[registers[15]] = op[0];
                --registers[15];
                break;
            case OP_PUSHR:
                op[0] = data[++pc];
                data[registers[15]] = registers[op[0]];
                --registers[15];
                break;
// pop from stack
            case OP_POPR:
                op[0] = data[++pc];
                registers[op[0]] = data[++registers[15]];
                break;
// compare
            case OP_CMPRI:
                op[0] = data[++pc];
                op[1] = data[++pc];
                if(registers[op[0]] > op[1])
                    flags.gt = 1;
                else
                    flags.gt = 0;
                if(registers[op[0]] == op[1])
                    flags.eq = 1;
                else
                    flags.eq = 0;
                if(registers[op[0]] < op[1])
                    flags.lt = 1;
                else
                    flags.lt = 0;
                break;
            case OP_CMPRR:
                op[0] = data[++pc];
                op[1] = data[++pc];
                if(registers[op[0]] > registers[op[1]])
                    flags.gt = 1;
                else
                    flags.gt = 0;
                if(registers[op[0]] == registers[op[1]])
                    flags.eq = 1;
                else
                    flags.eq = 0;
                if(registers[op[0]] < registers[op[1]])
                    flags.lt = 1;
                else
                    flags.lt = 0;
                break;
// goto
            case OP_JMPI:
                op[0] = data[++pc];
                pc = op[0] - 1;
                break;
            case OP_JEQI:
                op[0] = data[++pc];
                if(flags.eq == 1)
                    pc = op[0] - 1;
                break;
            case OP_JNEI:
                op[0] = data[++pc];
                if(flags.eq == 0)
                    pc = op[0] - 1;
                break;
            case OP_JLTI:
                op[0] = data[++pc];
                if(flags.lt == 1)
                    pc = op[0] - 1;
                break;
            case OP_JGTI:
                op[0] = data[++pc];
                if(flags.gt == 1)
                    pc = op[0] - 1;
                break;
            case OP_JLEI:
                op[0] = data[++pc];
                if(flags.gt == 0)
                    pc = op[0] - 1;
                break;
            case OP_JGEI:
                op[0] = data[++pc];
                if(flags.lt == 0)
                    pc = op[0] - 1;
                break;
            default:
                printf("!");
        }
        ++pc;
        --overload;
	}
	if(overload)
		printf("\nEnded at %d\n", pc);
	else
		printf("\nEmergency exit\n");
}

int main(int argc, char *argv[]){
	int i;
	long filelen;
	FILE *infile;
	printf("faekvm\n%d registers\n%d bytes of memory\n", REG, MEM);
	/* cleaning registers */
	for(i = 0; i < REG; i++){
		registers[i] = 0;
	}
    registers[REG-1] = MEM-1;
	/* loading flash */
	if(argc > 1){
        printf("Loading from %s\n", argv[1]);
        infile = fopen(argv[1], "rb");
    } else {
        printf("Falling back to defaults\n");
        infile = fopen("data.bin", "rb");
    }
	fseek(infile, 0, SEEK_END);
	filelen = ftell(infile);
	rewind(infile);
	fread(data, (filelen > MEM ? MEM : filelen), 1, infile);
	fclose(infile);
	printf("Read %d bytes\n", filelen);
    vm_loop();
	dump_registers();
	return 0;
}
