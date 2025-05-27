#include <iostream>

#ifdef _EMSCRIPTEN
#include <emscripten.h>
#include <emscripten/bind.h>
#endif

void helloWorld() {
    std::cout << "Hello, World!" << std::endl;
}

int add(int a, int b) {
    return a + b;
}

#ifdef _EMSCRIPTEN
EMSCRIPTEN_BINDINGS(basic_functions) {
    emscripten::function("helloWorld", &helloWorld);
    emscripten::function("add", &add);
}
#endif