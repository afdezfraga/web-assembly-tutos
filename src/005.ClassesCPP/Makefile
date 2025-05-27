# Compiler and flags
EMCC = em++
CXXFLAGS = -O2 -D_EMSCRIPTEN -s WASM=1 -s MODULARIZE=1 -s EXPORT_NAME="MyClassModule"


# Source and output files
SRC = classes.cpp
OUT = classes.js

# Build target
all: $(OUT)

$(OUT): $(SRC)
	$(EMCC) $(CXXFLAGS) $(SRC) -o $(OUT) -lembind

clean:
	rm -f classes.js classes.wasm
