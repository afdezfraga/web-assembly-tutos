# Makefile for compiling C code with em++ and cleaning generated files

TARGET = hello
SRC = hello.c
EMCC = emcc
CFLAGS = -O2
OUTPUT_FILES = $(TARGET).html $(TARGET).js $(TARGET).wasm

all: $(OUTPUT_FILES)

$(TARGET).html: $(SRC)
	$(EMCC) $(CFLAGS) $(SRC) -o $(TARGET).html

clean:
	rm -f $(OUTPUT_FILES)