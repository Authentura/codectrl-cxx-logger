#include <iostream>
#include "codectrl/log.h"

int main() {
    CodeCtrl::log::Log<std::string> logger;

    logger.log("hello world");

    return 0;
}