#include <iostream>

#ifdef _EMSCRIPTEN
#include <emscripten.h>
#include <emscripten/bind.h>
#endif

class MyBasicClass {
public:
    MyBasicClass(int value) : value(value) {}
    void setValue(int newValue) {
        value = newValue;
    }
    int getValue() const {
        return value;
    }
    void greet() const {
        std::cout << "Hello world! I currently have a value of " << value << std::endl;
    }
private:
    int value;
};

#ifdef _EMSCRIPTEN
EMSCRIPTEN_BINDINGS(my_basic_class_module) {
    emscripten::class_<MyBasicClass>("MyBasicClass")
        .constructor<int>()
        .function("setValue", &MyBasicClass::setValue)
        .function("getValue", &MyBasicClass::getValue)
        .function("greet", &MyBasicClass::greet);
}
#endif