# Compiler and flags
CC := gcc
CFLAGS := -lc
COMPILE_FLAGS := -Wall -Wextra -Wshadow

DEBUG ?= 0

ifeq ($(DEBUG),1)
  BUILD_FLAGS := $(CFLAGS) $(COMPILE_FLAGS) -g -O0
else
  BUILD_FLAGS := $(CFLAGS) $(COMPILE_FLAGS) -O2
endif

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

# Link
$(BIN)/$(TARGET): $(OBJS)
	@mkdir -p $(BIN)
	$(CC) $(BUILD_FLAGS) $^ -o $@

# Compile all .c files to .o files
$(OBJ)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(BUILD_FLAGS) -I$(SRC) -I$(TEST) -c $< -o $@

# Run release
run: all
	./$(BIN)/$(TARGET) $(INPUT)/$(ARGS)

# Call make with debug
debug:
	$(MAKE) DEBUG=1 all

# Run debugger
drun: DEBUG := 1
drun: all
	@gdb ./$(BIN)/$(TARGET) $(INPUT)/$(ARGS)

# Clean build artifacts
clean:
	rm -rf $(OBJ) $(BIN)/$(TARGET)

.PHONY: all run debug drun clean