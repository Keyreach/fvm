#include <stdio.h>

// 000xxx
#define OP_MOVRI 0x2
#define OP_MOVRR 0x3
// 001xxx
#define OP_ADRII 0xc
#define OP_ADRRI 0xe
#define OP_ADRRR 0xf
// 010xxx
#define OP_SBRII 0x14
#define OP_SBRRI 0x16
#define OP_SBRRR 0x17
// 101xxx

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
#define OP_JMP 0x40
#define OP_JEQ 0x41
#define OP_JNE 0x42
#define OP_JLT 0x43
#define OP_JLE 0x44
#define OP_JGT 0x45
#define OP_JGE 0x46

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

char data[MEM] = {
	OP_MOVRI, 0, 10,
	OP_STRI, 0, 100,
	OP_SBRRI, 0, 0, 1,
	OP_LDRI, 1, 100,
	OP_ADRRR, 1, 1, 0,
	OP_STRI, 1, 100,
	OP_CMPRI, 0, 0,
	OP_JNE, 6,
	OP_STOP
};

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
            case OP_JMP:
                op[0] = data[++pc];
                pc = op[0] - 1;
                break;
            case OP_JEQ:
                op[0] = data[++pc];
                if(flags.eq == 1)
                    pc = op[0] - 1;
                break;
            case OP_JNE:
                op[0] = data[++pc];
                if(flags.eq == 0)
                    pc = op[0] - 1;
                break;
            case OP_JLT:
                op[0] = data[++pc];
                if(flags.lt == 1)
                    pc = op[0] - 1;
                break;
            case OP_JGT:
                op[0] = data[++pc];
                if(flags.gt == 1)
                    pc = op[0] - 1;
                break;
            case OP_JLE:
                op[0] = data[++pc];
                if(flags.gt == 0)
                    pc = op[0] - 1;
                break;
            case OP_JGE:
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
