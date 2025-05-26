# Compiler and flags
EMCC = em++
GCC = g++

# Source and output files
SRC = main.cpp
DESKTOP_OUT = desktop_app
WEB_OUT = web_app.html
OTHER_WEB_OUTS = web_app.js web_app.wasm

# Targets
all: desktop web

desktop: $(SRC)
	$(GCC) $(CFLAGS) -o $(DESKTOP_OUT) $(SRC) -lSDL2

web: $(SRC)
	$(EMCC) $(CFLAGS) -D_EMSCRIPTEN -o $(WEB_OUT) $(SRC) -s WASM=1 -s USE_SDL=2

clean:
	rm -f $(DESKTOP_OUT) $(WEB_OUT) $(OTHER_WEB_OUTS)