#include <iostream>
#include "codectrl/log.h"

void layer_four() {
    CodeCtrl::log("Hello, world");
}

void layer_three() {
    layer_four();
}

void layer_two() {
    layer_three();
}

void layer_one() {
    layer_two();
}

int main() {
    layer_one();

#ifndef DEBUG
    std::cout << "not in debug mode" << std::endl;
#endif

    return 0;
}