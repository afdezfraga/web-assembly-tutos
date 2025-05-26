# Instructions

Asuming emscripten is sourced and working, run the following command to compile the C++ code to WebAssembly:

```bash
emcc hello.c -o hello.html
```

Then run with the command:

```bash
emrun hello.html
```