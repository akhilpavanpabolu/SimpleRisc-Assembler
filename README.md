# Assembler for Simple RISC
This project is a custom assembler that translates assembly code into binary and hexadecimal machine code.
## 🚀 Project Overview
It includes:
### Assembler in C :
The core program (`ass.c`) parses and encodes assembly instructions into binary and hexadecimal format.
### GUI in Python  :
A graphical interface (`gui.py`) to simplify the assembly process with a file selection system and real-time output display.
### Input & Output Files:
**input.asm** : The assembly source code.

**output.bin** : The assembled binary output.

**output.hex** : The assembled hexadecimal output.

**binary_output.txt** : A readable binary representation of the machine code, with errors logged.

## 🔍 Features :
### Instruction Set Support: 
Implements various instructions such as `add`, `sub`, `mul`, `div`, `mod`, `cmp`, `and`, `or`, `not`, `mov`, `lsl`, `lsr`, `asr`, `nop`, `ld`, `st`, `beq`, `bgt`, `b`, `call`, and `ret`.
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
<img width="730" alt="Interface" src="https://github.com/user-attachments/assets/12855ff2-b5dc-42cb-b887-5c48dfd69585" />

---
## Limitations of our project :
* we should run GUI in terminal for execution.
* for load and store instructions the syntax would be:
     * Load :
       `ld <rd>,<rs1>,<rs2/imm>`
    * Store :
        `st <rd> ,<rs1>,<rs2/imm>` 
  
* Spaces should be avoided between register and comas in instruction.

## 🛠️ Issues Faced and Resolutions

### 1️⃣ **Incorrect Operand Count**
- **Problem:**  
    When assembling instructions, the program would sometimes throw errors due to incorrect operand counts.  
    - Example:  
    ```asm
    mul r1, r2, r3, r4     ; Error: Too many operands (expected 3, got 4)
    add r1, r2              ; Error: Too few operands (expected 3, got 2)
    ```
- **Root Cause:**  
    - The assembler logic expected a fixed number of operands for each instruction but failed when it encountered too many or too few operands.
- **Solution:**  
    - Added **operand count validation** to the assembler.  
    - Implemented error handling to log the issue in the output:
    ```
    Error: Invalid number of operands for mul (expected 3, got 4)
    ```
