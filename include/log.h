#pragma once
#include <iostream>
#include <map>
#include <vector>
#include "backtrace_data.h"

namespace log {
template <typename T>
struct Log {
    std::vector<BacktraceData> stack;
    uint32_t line_number;
    std::map<uint32_t, std::string> code_snippet;
    std::string message;
    std::string message_type;
    std::string file_name;
    std::string address;
    std::vector<std::string> warnings;

    Log(std::string message,
        int surround = 3,
        std::string host = "127.0.0.1",
        std::string port = "3001")
        : address(""),
          message(message),
          message_type(typeid(T).name()),
          stack({}),
          surround(surround),
          warnings({}),
          file_name(""){};
};
}