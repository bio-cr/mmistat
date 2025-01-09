# Compiler and flags
CC = gcc
CFLAGS = -O2 -I.

# Target and source files
TARGET = mmistat
SRC = main.c index.c
OBJ = $(SRC:.c=.o)

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Clean up build files
clean:
	rm -f $(OBJ) $(TARGET)

