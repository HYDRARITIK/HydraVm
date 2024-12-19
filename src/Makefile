# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# List of source files
SRC = main.c load.c instruction.c virtual.c trap.c

# List of object files (created from the source files)
OBJ = $(SRC:.c=.o)

# The output binary name
OUTPUT = my_program

# Default target to build the program
all: $(OUTPUT)

# Linking the object files to create the executable
$(OUTPUT): $(OBJ)
	$(CC) $(OBJ) -o $(OUTPUT)

# Compile the source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and the binary
clean:
	rm -f $(OBJ) $(OUTPUT)

# Include the header files in the rule to ensure the proper dependencies
# These headers are already included directly in your C files
