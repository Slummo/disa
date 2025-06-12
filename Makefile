# Compiler and flags
CC := gcc
CFLAGS := -lc
FLAGS := -Wall -Wextra -Wshadow

# Directories
SRC := src
TEST := test
INPUT := input
OBJ := obj
BIN := bin

# Target
TARGET := test

# Source files
SRCS := $(shell find $(SRC) -type f -name "*.c")
TEST_SRCS := $(shell find $(TEST) -type f -name "*.c")
ALL_SRCS := $(SRCS) $(TEST_SRCS)

# Object files
OBJS := $(patsubst %.c, $(OBJ)/%.o, $(ALL_SRCS))

# Default target
all: $(BIN)/$(TARGET)

# Link all objects into test binary
$(BIN)/$(TARGET): $(OBJS)
	@mkdir -p $(BIN)
	$(CC) $(FLAGS) $^ -o $@

# Compile all .c files to .o files
$(OBJ)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(FLAGS) -I$(SRC) -I$(TEST) -c $< -o $@

# Run the target
run: all
	./$(BIN)/$(TARGET) $(INPUT)/$(ARGS)

# Clean build artifacts
clean:
	rm -rf $(OBJ) $(BIN)/$(TARGET)

.PHONY: all clean run