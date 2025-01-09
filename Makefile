# Compiler and flags
CC = gcc
CFLAGS = -O2 -I. -Iminimap2 -Wall
CPPFLAGS = -DHAVE_KALLOC
LDFLAGS = -lm -lz -lpthread

# Source files and objects
SRC = main.c minimap2/index.c minimap2/kalloc.c minimap2/bseq.c minimap2/kthread.c \
      minimap2/misc.c minimap2/sketch.c minimap2/options.c
OBJS = $(SRC:.c=.o)

# Target
TARGET = mmistat

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $(OBJS) $(LDFLAGS)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)
