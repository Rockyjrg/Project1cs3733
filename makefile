# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Werror -std=c99

# Executable name
TARGET = shellrun

# Source file
SRC = shellmain.c

# Build the executable
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Run the shell program
run: $(TARGET)
	./$(TARGET)

# Clean up compiled files
clean:
	rm -f $(TARGET)