# Include Raspberry Pi sysroot headers
INCLUDES = -I/include/

# Target binary name
TARGET = helloworld

# Source files (all .c in src directory)
SRC = $(wildcard src/*.c)

# Object files
OBJ = $(SRC:.c=.o)

# Default target
all: $(TARGET)

# Link objects into the final binary
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Compile each .cpp to .o using cross-compiler
%.o: %.c
	$(CC) $(CCFLAGS) $(INCLUDES) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJ) $(TARGET)