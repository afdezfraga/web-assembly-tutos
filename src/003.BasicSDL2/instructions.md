
## Compiling an SDL2 Game

To compile an SDL2 game, you need to ensure the following:
1. Install the SDL2 development libraries for your platform.
2. Include the SDL2 headers in your source code.
3. Link against the SDL2 library during compilation.
4. Use a compiler like `gcc` or `clang` to build your game. For example:
   ```bash
   gcc -o game game.c -lSDL2
   ```
5. Ensure that the SDL2 runtime library is available in your system's library path.

## Adapting the Game for Emscripten

To compile and run your SDL2 game in a web environment using Emscripten, follow these steps:
1. **Install Emscripten**:
   - Download and install Emscripten from its official website: https://emscripten.org/.
   - Set up the Emscripten environment by sourcing the `emsdk_env` script.
2. **Modify Your Code**:
   - Ensure your game uses SDL2 functions that are compatible with Emscripten.
   - Replace any platform-specific code (e.g., file I/O or threading) with web-compatible alternatives.
   - Use `emscripten_set_main_loop()` to handle the game loop instead of a traditional infinite loop.
3. **Compile with Emscripten**:
   - Use the `emcc` compiler to build your game. For example:
     ```bash
     emcc game.c -s USE_SDL=2 -o game.html
     ```
   - The `-s USE_SDL=2` flag tells Emscripten to include SDL2 support.
   - The `-o game.html` flag specifies the output as a web page.
4. **Test the Game**:
   - Open the generated `game.html` file in a web browser to test your game.
   - Use a local web server if required, as some browsers block local file access.
5. **Optimize for the Web**:
   - Minimize asset sizes and use compressed formats for faster loading.
   - Consider using WebAssembly (`.wasm`) for better performance.