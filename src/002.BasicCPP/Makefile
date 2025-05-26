# Makefile for compiling C++ with em++ and cleaning generated files

# Compiler
EMCC = em++

# Source and output files
SRC = hello.cpp
OUT = hello.html

# Build target
all: $(OUT)

$(OUT): $(SRC)
	$(EMCC) $(SRC) -o $(OUT)

# Clean target
clean:
	rm -f $(OUT) hello.js hello.wasm