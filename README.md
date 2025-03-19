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
        `st <rd>,<rs1>,<rs2/imm>` 
  
* Spaces should be avoided between register and comas in instruction.
* Currently, our assembler does not support placing an instruction on the same line as a label.
    If an instruction is written on the same line as a label (e.g., 'start: add r1, r2, r3'), the assembler treats it as a label definition only and ignores the instruction.
This happens because the first pass extracts the label and skips the rest of the line.

## 🛠️ Issues Faced and Resolutions

### 1️⃣ **Incorrect Operand Count**
- **Problem:**  
    When assembling instructions, the program would sometimes throw errors due to incorrect operand counts.  
    - Example:  
    ```
    mul r1,r2,r3,r4     ; Error: Invalid number of Operands for mul (expected 3, got 4)
    add r1,r2              ; Error: Invalid number of Operands for add (expected 3, got 2)
    ```
- **Root Cause:**  
    - The assembler logic expected a fixed number of operands for each instruction but failed when it encountered too many or too few operands.
- **Solution:**  
    - Added **operand count validation** to the assembler.  
    - Implemented error handling to log the issue in the output:
    ```
    Error: Invalid number of operands for mul (expected 3, got 4)
    ```
### 2️⃣ **Label Handling Issues**
- **Problem:**  
    When using labels in the assembly code, the assembler would sometimes misinterpret them or fail to resolve the jump address correctly.  
    - Example:  
    ```
    start:                  ; Label definition
    add r1,r2,r3            ; addition
    b start                 ; Branch to the label
    ```
- **Root Cause:**  
    - Labels were not properly mapped to their memory addresses during the first pass.
    - Jump instructions failed to resolve the label reference during the second pass.
- **Solution:**  
    - Implemented **two-pass assembly**:
        - **First Pass:** Map labels to their respective memory addresses.
        - **Second Pass:** Replace label references with corresponding addresses.
    - Added error logging for unresolved labels:
    ```
    Error: Undefined label 'loop' 
    ```
  
