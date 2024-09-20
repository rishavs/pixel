# Variables
COMPILER = gcc
HEADERS_FOLDER = include
OUTPUT_EXECUTABLE = bin/pixel
TEST_EXECUTABLE = bin/test
MAIN_FILE = src/pixel.c
TEST_FILE = test.c
COMPILER_FLAGS = -I$(HEADERS_FOLDER)
SOURCE_FILES = src/list.c \
	src/compiler.c \
	src/reader.c

# Default target
all: build

# Build target
build:
	$(COMPILER) $(COMPILER_FLAGS) $(MAIN_FILE) $(SOURCE_FILES) -o $(OUTPUT_EXECUTABLE) -lgc

# Run target
run: build
	./bin/pixel run tests/inputs/first.pix

# Test target
test: 
	$(COMPILER) $(COMPILER_FLAGS) $(TEST_FILE) src/*.c -o .\bin\test.exe
	./bin/test.exe

# Clean target
clean:
	rm -f $(OUTPUT_EXECUTABLE) test