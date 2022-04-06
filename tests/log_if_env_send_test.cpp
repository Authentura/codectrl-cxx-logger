
#include <iostream>
#include "codectrl/log.hpp"

void layer_four() {
    CodeCtrl::log_if_env("Hello, world env");
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

    return 0;
}
