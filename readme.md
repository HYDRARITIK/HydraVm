# Hydra VM

Hydra VM is a virtual machine implementation of the LC-3 (Little Computer 3) architecture in C. The LC-3 is a 16-bit processor architecture designed for educational purposes. Hydra VM simulates the execution of LC-3 instructions, including operations like `ADD`, `BR`, `LDR`, `STR`, and many more. It also supports various trap routines for external configuration such as `PUTS`, `TIN`, `TOUT`, and others.

## Features

- **16-bit instruction set**: Implements the LC-3's instruction set, including basic operations like:
  - `ADD` (Addition)
  - `BR` (Branching)
  - `LDR` (Load Register)
  - `STR` (Store Register)
  - And many more...
  
- **Trap routines**: Includes several trap routines to interact with the environment, including:
  - `PUTS` (Prints a string)
  - `TIN` (Reads a character from input)
  - `TOUT` (Writes a character to output)
  - Additional system-level traps for interaction with external configuration.

- **Memory Management**: Hydra VM emulates memory management with the LC-3's 16-bit address space.

- **Instruction Fetch and Decode**: Simulates the LC-3's instruction fetching and decoding pipeline.

- **Makefile for easy compilation**: The project includes a Makefile to simplify the build process and compilation.


### Prerequisites

- **C Compiler**: The project is written in C and can be compiled with any standard C compiler (e.g., GCC).
- **Make**: The project uses `make` for building the application. Ensure that `make` is installed on your system.

### Compilation

To compile the Hydra VM, simply run the following command in the project directory:

```bash
make   
./my_program /path_to_object_file of program
