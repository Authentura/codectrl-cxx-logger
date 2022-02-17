#include <iostream>
#include "codectrl/log.hpp"

void layer_four() {
    CodeCtrl::log_if([]() { return rand() % 2 == 1; },
                     "Hello, world conditional");

    CodeCtrl::log_if([]() { return true; }, "Hello, world conditional 2");
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
    srand(time(NULL));

    layer_one();

    return 0;
}