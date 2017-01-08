#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "chars.h"

struct cs_vector {
    char** array;
    unsigned int length;
};

char argument_type(char* s){
    if(s[0] == 'r')
        return 1;
    else
        return 0;
}

int argument_trim(char* s){
    strtol(chars_substr(s, 1, strlen(s)), NULL, 10);
}

char* assembler(char* source, int *len){
    int i = 0, opnum;
    char opcode, op1[6], op2[6], op3[6];
    struct cs_vector tokens;
    char* out;
    out = (char*)malloc(256 * sizeof(char));
    tokens.array = chars_split(source, ' ', &tokens.length);
    printf(">> %d\n", tokens.length);
    while(i < tokens.length){
        printf("> %s\n", tokens.array[i]);
        if(strcmp(tokens.array[i], "mov") == 0){
            opcode = 0;
            opnum  = 2;
        } else if(strcmp(tokens.array[i], "add") == 0){
            opcode = 1 << 3;
            opnum  = 3;
        } else if(strcmp(tokens.array[i], "sub") == 0){
            opcode = 2 << 3;
            opnum  = 3;
        } else if(strcmp(tokens.array[i], "ldr") == 0){
            opcode = 3 << 3;
            opnum  = 2;
        } else if(strcmp(tokens.array[i], "cmp") == 0){
            opcode = 4 << 3;
            opnum  = 2;
        } else if(strcmp(tokens.array[i], "str") == 0){
            opcode = 7 << 3;
            opnum  = 2;
        } else if(strcmp(tokens.array[i], "jmp") == 0){
            opcode = 0x40;
            opnum  = 1;
        } else if(strcmp(tokens.array[i], "jeq") == 0){
            opcode = 0x41;
            opnum  = 1;
        } else if(strcmp(tokens.array[i], "jne") == 0){
            opcode = 0x42;
            opnum  = 1;
        } else if(strcmp(tokens.array[i], "jlt") == 0){
            opcode = 0x43;
            opnum  = 1;
        } else if(strcmp(tokens.array[i], "jle") == 0){
            opcode = 0x44;
            opnum  = 1;
        } else if(strcmp(tokens.array[i], "jgt") == 0){
            opcode = 0x45;
            opnum  = 1;
        } else if(strcmp(tokens.array[i], "jge") == 0){
            opcode = 0x46;
            opnum  = 1;
        } else {
            opcode = 0;
            opnum  = 0;
        }
        /** operands + suffixes **/
        switch(opnum){
        case 0:
            out[i] = opcode;
            break;
        case 1:
            strcpy(op1, tokens.array[++i]);
            out[i - 1] = opcode;
            out[i] = argument_trim(op1) & 0xff;
            break;
        case 2:
            strcpy(op1, tokens.array[++i]);
            strcpy(op2, tokens.array[++i]);
            opcode |= argument_type(op1) << 1 | argument_type(op2);
            out[i - 2] = opcode;
            out[i - 1] = argument_trim(op1) & 0xff;
            out[i] = argument_trim(op2) & 0xff;
            break;
        case 3:
            strcpy(op1, tokens.array[++i]);
            strcpy(op2, tokens.array[++i]);
            strcpy(op3, tokens.array[++i]);
            opcode |= argument_type(op1) << 2 | argument_type(op2) << 1 | argument_type(op3);
            out[i - 3] = opcode;
            out[i - 2] = argument_trim(op1) & 0xff;
            out[i - 1] = argument_trim(op2) & 0xff;
            out[i] = argument_trim(op3) & 0xff;
            break;
        }
        ++i;
    }
    printf(">> end\n");
    *(len) = tokens.length;
    return out;
}

int main(int argc, char *argv[])
{
    int codelen, bytelen;
    FILE *outfile, *infile;
    char *bytecode, *code;
    if(argc > 2){
        printf("Loading from %s\n", argv[1]);
        infile  = fopen(argv[1], "r");
        outfile = fopen(argv[2], "wb");
    } else {
        printf("Falling back to defaults\n");
        infile  = fopen("code.asm", "r");
        outfile = fopen("data.bin", "wb");
    }
    fseek(infile, 0, SEEK_END);
    codelen = ftell(infile);
    rewind(infile);
    code = malloc(codelen + 1);
    fread(code, codelen, 1, infile);
    code[codelen] = 0;
    fclose(infile);
    bytecode = assembler(code, &bytelen);
    fwrite(bytecode, 1, bytelen, outfile);
    fclose(outfile);
    puts("- written");
    return 0;
}
