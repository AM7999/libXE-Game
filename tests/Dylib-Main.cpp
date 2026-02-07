#include "../extern/dylib/include/dylib.hpp"

#include <iostream>

int main() {
    dylib::library lib("./testlib", dylib::decorations::os_default());

    auto adder = lib.get_function<void()>("hello");

    adder();
}