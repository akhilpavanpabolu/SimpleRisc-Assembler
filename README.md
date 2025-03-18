# Assembler for Simple RISC
This project is a custom assembler that translates assembly code into binary and hexadecimal machine code.
## 🚀 Project Overview
It includes:
### Assembler in C :
The core program (**ass.c**) parses and encodes assembly instructions into binary and hexadecimal format.
### GUI in Python  :
A graphical interface (**gui.py**) to simplify the assembly process with a file selection system and real-time output display.
### Input & Output Files:
**input.asm** : The assembly source code.

**output.bin** : The assembled binary output.

**output.hex** : The assembled hexadecimal output.

**binary_output.txt** : A readable binary representation of the machine code, with errors logged.

## 🔍 Features :
### Instruction Set Support: 
Implements various instructions such as add, sub, mul, div, mod, cmp, and, or, not, mov, lsl, lsr, asr, nop, ld, st, beq, bgt, b, call, and ret.
### Label Support : 
Supports labels and resolves them during assembly.
### Comment Removal: 
Automatically removes single-line and inline comments (e.g., ;, # ) before processing the instructions.
### Error Handling: 
Displays errors in the output (e.g., incorrect operand count) in both binary and hexadecimal formats.
### Output in Multiple Formats: 
Generates both binary and hexadecimal outputs.

## How to USE? 
The assembler can still be executed separately through the terminal.

### Compile the code :
```
gcc ass.c -o assembler
```
### Run The Assembler :
```
./assembler input.asm output.bin
```
### Launch the GUI :
```
python3 gui.py
```
### In the GUI :
* Select the input and output files.
* Click Assemble.
* View the binary and hexadecimal output with error messages displayed in real-time.
  
## Basic look of our GUI looks like :
<img width="800" alt="Interface" src="https://github.com/user-attachments/assets/12855ff2-b5dc-42cb-b887-5c48dfd69585" />

---
## Limitations of our project :
* we should run GUI in terminal for execution.
* for load and store instructions the syntax would be :
  load :
      ld <rd>,<rs1>,<rs2/imm>
  store :
      st <rd> ,<rs1>,<rs2/imm>
* Spaces should be avoided between register and comas in instruction.
