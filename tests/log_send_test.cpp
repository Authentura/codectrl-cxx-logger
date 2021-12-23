#include <iostream>
#include "codectrl/log.h"

int main() {
    CodeCtrl::Log<std::string> logger;

    logger.log("Hello, world!");

    return 0;
}