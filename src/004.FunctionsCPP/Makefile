# Compiler and flags
EMCC = em++
CXXFLAGS = -O2 -D_EMSCRIPTEN -s WASM=1 -s MODULARIZE=1 -s EXPORT_NAME="createModule" 

# Source and output files
SRC = functions.cpp
OUT = functions.js

# Build target
all: $(OUT)

$(OUT): $(SRC)
	$(EMCC) $(CXXFLAGS) $(SRC) -o $(OUT) -lembind

clean:
	rm -f functions.js functions.wasm
