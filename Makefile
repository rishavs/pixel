# Variables
COMPILER = gcc
HEADERS_FOLDER = include
OUTPUT_EXECUTABLE = bin/pixel
TEST_EXECUTABLE = bin/test
MAIN_FILE = src/pixel.c
TEST_FILE = test.c
COMPILER_FLAGS = -fno-omit-frame-pointer #-fsanitize=address
COMPILER_WARNINGS = -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter
INCLUDES = -I$(HEADERS_FOLDER)
SOURCE_FILES = \
	src/compiler/errors.c \
	src/compiler/helpers.c \
	src/core/list.c \
	src/core/bag.c \
	src/compiler/compiler.c \
	src/compiler/reader.c \
	src/compiler/lexer.c \
	src/compiler/parser.c \
	src/compiler/codegen.c

# Default target
all: build

# Build target
build:
	$(COMPILER) $(COMPILER_WARNINGS) $(COMPILER_FLAGS) $(INCLUDES) $(MAIN_FILE) $(SOURCE_FILES) -o $(OUTPUT_EXECUTABLE)

# Run target
run: build
	./bin/pixel run tests/inputs/third.pix

# Test target
test: 
	$(COMPILER) $(COMPILER_FLAGS) $(TEST_FILE) src/*.c -o .\bin\test.exe
	./bin/test.exe

# Clean target
clean:
	rm -f $(OUTPUT_EXECUTABLE) test