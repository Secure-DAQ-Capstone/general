#include <iostream>

extern "C" {
    // The `hello` function will be imported, and we will provide its definition later
    void hello() {
        std::cout << "Hello, WebAssembly!" << std::endl;
    }
}

int main() {
    // This part of the code will not run in the WebAssembly context; it will be replaced by the export function
    hello();
    std::cout << "Hello, from main!" << std::endl;
    return 0;
}
