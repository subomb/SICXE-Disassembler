# SIC/XE Disassembler Project

## Project Overview
This SIC/XE Disassembler is a software tool designed to reverse-engineer SIC/XE machine language into a more understandable assembly language format. The core functionality of this tool is to parse .obj files containing SIC/XE object code, systematically identify the formats of instructions, and output detailed characteristics of each instruction, including mnemonics, operand addressing types, and target address addressing modes. It is important to note that this project specifically targets the parsing and interpretation of text records within the SIC/XE object files.

## Features
- Instruction Format Support: Handles SIC/XE instruction formats 2, 3, and 4, making it versatile for various types of assembly language instructions.
- Detailed Instruction Information: For each instruction within the object code, the disassembler outputs:
    - Instruction mnemonic (e.g., LDA, ADD)
    - Instruction format (2, 3, or 4)
    - Operand addressing type (simple, immediate, indirect)
    - Target address addressing mode (pc, base, absolute, or indexed variations)
    - Original object code for reference
- Custom Output Filenames: Produces output files for the disassembled code, naming each according to its corresponding input file but with a "_struct.txt" suffix to signify the structured disassembly output.

## Compilation and Execution

### Compilation
The project can be compiled using the following command:

- g++ -std=c++11 -o disassem Parse.cpp

This compiles the 'Parse.cpp' source file into an executable named 'disassem'.

### Running the Program
To run the disassembler, use the following syntax:

- ./disassem <input_file.obj> [<additional_input_files.obj>...]

Example:
- ./disassem test1.obj test2.obj test3.obj

This will process each specified .obj file and generate a corresponding output file named <input_filename>_struct.txt, containing the disassembled instructions.

### Input and Output
- Input: The program accepts one or more .obj files as input, each containing SIC/XE object code.
- Output: For each input file, the program generates an output file with the disassembled instructions. The output files are named based on the input files but with a _struct.txt suffix.

### Input File Format
The expected input is a SIC/XE object file with the following structure:
- Header record (H)
- Text records (T)
- Modification records (M), if any
- End record (E)

Example:
Given an input file test1.obj, the program will generate an output file named test1_struct.txt with the disassembled SIC/XE instructions formatted as specified by the project requirements.
#### test1.obj:
#### HSUM 000000000031
#### T0000001C0500000320146910178D1BA0101BC0002F20073B2FF40F102EFD9041
#### M00000705
#### M00001705
#### E000000

### Output File
The output 'obj_struct.txt' will contain the disassembled object code with columns for Instruction, Format, Operand Addressing Type, Target Address Addressing Mode, and Object Code.

#### test1_struct.txt:
![image](https://github.com/subomb/SICXE-Disassembler/assets/119646112/8380ba72-c18b-4754-8508-5303abccda98)

