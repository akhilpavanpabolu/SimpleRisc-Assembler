#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LABELS 100
#define MAX_INSTRUCTIONS 1000

typedef struct{
    char mnemonic[10];
    int opcode;
} Instruction;

Instruction instructions[] = {
    {"add", 0b00000}, {"sub", 0b00001}, {"mul", 0b00010}, {"div", 0b00011}, {"mod", 0b00100},
    {"cmp", 0b00101}, {"and", 0b00110}, {"or",  0b00111}, {"not", 0b01000}, {"mov", 0b01001},
    {"lsl", 0b01010}, {"lsr", 0b01011}, {"asr", 0b01100}, {"nop", 0b01101}, {"ld",  0b01110},
    {"st",  0b01111}, {"beq", 0b10000}, {"bgt", 0b10001}, {"b",  0b10010}, {"call", 0b10011},
    {"ret", 0b10100}
};

typedef struct {
    char mnemonic[10];
    int operand_count;
} InstructionOperands;

InstructionOperands operand_table[] = {
    {"add", 3}, {"sub", 3}, {"mul", 3}, {"div", 3}, {"mod", 3}, {"cmp", 2}, {"and", 3}, {"or", 3},
    {"not", 2}, {"mov", 2}, {"lsl", 3}, {"lsr", 3}, {"asr", 3}, {"nop", 0}, {"ld", 3}, {"st", 3},
    {"beq", 1}, {"bgt", 1}, {"b", 1}, {"call", 1}, {"ret", 0}
};

typedef struct {
    char label[20];
    int address;
} Label;

Label labels[MAX_LABELS];
int label_count = 0;

int getOpcode(char *mnemonic){
    for(int i=0; i< sizeof(instructions)/sizeof(Instruction);i++){
        if(strcmp(mnemonic ,instructions[i].mnemonic)==0){
            return instructions[i].opcode;
        }
    }
    return -1;      //invalid instruction
}
int find_label_address(char *label) {
    
    for (int i = 0; i < label_count; i++) {
        if (strcmp(labels[i].label, label) == 0) {
            return labels[i].address;
        }
    }
    return -1;
}


void remove_comment(char *line) {
    char *comment_start = strchr(line, '#');  // For '#' comments
    if (!comment_start) {
        comment_start = strchr(line, ';');  // For ';' comments (optional)
    }
    if (comment_start) {
        *comment_start = '\0';  // Truncate the line at the comment character
    }
}


void first_pass(FILE *input) {
    char line[100], label[20];
    int address = 0;
    
    while (fgets(line, sizeof(line), input)) {
        remove_comment(line);
        char *colon = strchr(line, ':'); // Find colon (label declaration)
        if (colon) {
            *colon = '\0'; // Remove the colon to isolate the label name
            sscanf(line, " %s", label); // Extract the label
            if (label_count < MAX_LABELS) {
                strcpy(labels[label_count].label, label);
                labels[label_count].address = address;
                label_count++;
            } else {
                printf("Error: Too many labels defined (max: %d)\n", MAX_LABELS);
            }
        } else {
            address++; // Count only valid instructions
        }
    }

    rewind(input); // Reset file pointer to beginning
}


unsigned int encode_instruction(char *mnemonic, int type,int rd,int rs1,int rs2,int imm){
    int opcode = getOpcode(mnemonic);

    if (opcode == -1) {
        printf("Error: Invalid instruction %s\n", mnemonic);
        return 0;
    }

    if (strcmp(mnemonic, "nop") == 0 || strcmp(mnemonic, "ret") == 0) {
        return (opcode << 27);
    } 
    else if (strcmp(mnemonic, "add") == 0 || strcmp(mnemonic, "sub") == 0 ||
               strcmp(mnemonic, "mul") == 0 || strcmp(mnemonic, "div") == 0 ||
               strcmp(mnemonic, "mod") == 0 || strcmp(mnemonic, "and") == 0 ||
               strcmp(mnemonic, "or") == 0 || strcmp(mnemonic, "lsl") == 0 ||
               strcmp(mnemonic, "lsr") == 0 || strcmp(mnemonic, "asr") == 0) {
        return (opcode << 27) | (type << 26) | (rd << 22) | (rs1 << 18) | (rs2 << 14) | (imm & 0x3FFFF);
    } else if (strcmp(mnemonic, "not") == 0 || strcmp(mnemonic, "mov") == 0) {
        return (opcode << 27) | (type << 26) | (rd << 22) | (rs2 << 14)| (imm & 0x3FFFF);
    } else if (strcmp(mnemonic, "cmp") == 0) {
        return (opcode << 27) | (type << 26) | (rs1 << 18) | (rs2 << 14) |(imm & 0x3FFFF);
    } else if (strcmp(mnemonic, "ld") == 0 || strcmp(mnemonic, "st") == 0) {
        return (opcode << 27) | (type << 26) | (rd << 22) | (rs1 << 18) | (rs2 << 14) | (imm & 0x3FFFF);
    }
    else if(strcmp(mnemonic, "b") == 0 || strcmp(mnemonic, "beq") == 0 || strcmp(mnemonic, "bgt") == 0 || 
            strcmp(mnemonic, "call") == 0){
        return (opcode << 27) | (imm & 0x07FFFFFF);
    }

    
    return 0;
}

void print_binary(unsigned int value) {
    for (int i = 31; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
    printf("\n");
}
int getOperandCount(char *mnemonic) {
    for (int i = 0; i < sizeof(operand_table) / sizeof(InstructionOperands); i++) {
        if (strcmp(mnemonic, operand_table[i].mnemonic) == 0) {
            return operand_table[i].operand_count;
        }
    }
    return -1; // Invalid instruction
}


void assemble_file(const char *input_file , const char *output_file){
    FILE *input = fopen(input_file, "r");
    FILE *output = fopen(output_file, "wb");
    FILE *binary_file = fopen("binary_output.txt", "w");
    FILE *hex_file = fopen("output.hex", "w");

    if(!input || !output){
        printf("Error in opening File...");
    }
    first_pass(input);
    char line[100],mnemonic[10],label[20];
    int rd,rs1,rs2;
    int address;
    int imm;

    while (fgets(line, sizeof(line), input)) {
        unsigned int instruction = 0;
        
        if (strchr(line, ':')) {
            continue;
        }
        // Extract the mnemonic from the line
        if (sscanf(line, "%s", mnemonic) != 1) {
            continue;
        }
        // Make a copy of the line to avoid modifying the original
        char line_copy[100];
        strcpy(line_copy, line);
        // Remove comments from the copied line before tokenizing
        remove_comment(line_copy);

        // === Add Operand Count Validation Here ===
        int expected_operands = getOperandCount(mnemonic);
        if (expected_operands == -1) {
            printf("Error: Invalid instruction %s\n", mnemonic);
            fprintf(binary_file,"Error: Invalid instruction %s\n", mnemonic);
            fprintf(hex_file,"Error: Invalid instruction %s\n", mnemonic);
            continue;
        }

        int actual_operands = 0;
        char *token = strtok(line_copy, " ,\t\n");  // Work on the copied line
        if (token) token = strtok(NULL, " ,\t\n");  // Skip mnemonic
        while (token) {
            actual_operands++;
            token = strtok(NULL, " ,\t\n");
        }
        // Validate operand count
        if (actual_operands != expected_operands) {
            printf("Error: Invalid number of operands for %s (expected %d, got %d)\n",
                   mnemonic, expected_operands, actual_operands);
            fprintf(binary_file,"Error: Invalid number of operands for %s (expected %d, got %d)\n",
                    mnemonic, expected_operands, actual_operands);  
            fprintf(hex_file,"Error: Invalid number of operands for %s (expected %d, got %d)\n",
                    mnemonic, expected_operands, actual_operands);
            continue;
        }
        // === End of Operand Count Validation ===

        if (sscanf(line, "%s r%d,r%d,r%d", mnemonic, &rd, &rs1, &rs2) == 4) {
            instruction = encode_instruction(mnemonic, 0, rd, rs1, rs2, 0);

        } else if (sscanf(line, "%s r%d,r%d,%d", mnemonic, &rd, &rs1, &imm) == 4) {
            instruction = encode_instruction(mnemonic, 1, rd, rs1, 0, imm);

        } else if (sscanf(line, "%s %d", mnemonic, &imm) == 2) {
            instruction = encode_instruction(mnemonic, 0, 0, 0, 0, imm);
        } 
        
        else if(sscanf(line, "%s r%d,r%d,", mnemonic, &rd, &rs2) == 3){
            if(strcmp(mnemonic, "cmp") == 0){
                instruction = encode_instruction(mnemonic, 0, 0, rd, rs2, 0);
            }
            else {
                instruction = encode_instruction(mnemonic, 0, rd, 0, rs2, 0);
            }
        }
        else if(sscanf(line, "%s r%d,%d,", mnemonic, &rd, &imm) == 3){
            if(strcmp(mnemonic, "cmp") == 0){
                instruction = encode_instruction(mnemonic, 1, 0, rd, 0, imm);
            }
            else {
                instruction = encode_instruction(mnemonic, 1, rd, 0, 0, imm);
            }
        }
        else if (sscanf(line, "%s %s", mnemonic, label) == 2) {
            imm = find_label_address(label);
            if (imm == -1) {
                printf("Error: Undefined label %s\n", label);
                fprintf(binary_file,"Error: Undefined label %s\n", label);  
                fprintf(hex_file,"Error: Undefined label %s\n", label);
                continue;
                
            }
            instruction = encode_instruction(mnemonic, 0, 0, 0, 0, imm - address  );
        }
        else if(sscanf(line, "%s",mnemonic)==1){
            instruction = encode_instruction(mnemonic,0,0,0,0,0);
        }
        else {
            printf("Invalid instruction: %s", line);
            continue;
        }
        fwrite(&instruction, sizeof(instruction), 1, output);
        address++;
        for (int i = 31; i >= 0; i--) {
            fprintf(binary_file, "%d", (instruction >> i) & 1);
        }
        fprintf(binary_file, "\n");
        // Write Hexadecimal Output (.hex file)
        fprintf(hex_file, "%08X\n", instruction);
        // printf("0b%032b\n", instruction);h
        print_binary(instruction);

    }
    fclose(input);
    fclose(output);
    fclose(binary_file);
    fclose(hex_file);

    
}
int main(int argc, char *argv[]){
    if (argc != 3) {
        printf("Usage: %s <input.asm> <output.bin>\n", argv[0]);
        return 1;
    }
    assemble_file(argv[1], argv[2]);
    return 0;
}