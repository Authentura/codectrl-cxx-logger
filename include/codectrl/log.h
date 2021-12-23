#pragma once
#include <asio.hpp>
#include <cstdint>
#include <iostream>
#include <map>
#include <optional>
#include <vector>

#include "backtrace_data.h"

namespace CodeCtrl {
template <typename T>
class Log {
   public:
    std::vector<data::BacktraceData> stack = {};
    uint32_t line_number = 0;
    std::map<uint32_t, std::string> code_snippet = {};
    std::string message = "";
    std::string message_type = typeid(T).name();
    std::string file_name = "";
    std::string address = "";
    std::vector<std::string> warnings = {};

   private:
    std::string host;
    uint32_t port;
    int surround;

   public:
    Log(int surround = 3, std::string host = "127.0.0.1", uint32_t port = 3001)
        : host(host), port(port), surround(surround) {}

    std::optional<asio::error_code> log(std::string message) {
        asio::error_code ec;
        asio::io_context context;
        asio::ip::tcp::endpoint endpoint(asio::ip::make_address(host, ec),
                                         port);

        std::cout << message << std::endl;
        get_stack_trace();

        return {};
    }

   private:
    void get_stack_trace() {}
    static void get_code(std::string file_path, uint32_t line_number) {}
    static void get_code_snippet(std::string file_path,
                                 uint32_t line_number,
                                 uint32_t surround) {}
};
}  // namespace CodeCtrl