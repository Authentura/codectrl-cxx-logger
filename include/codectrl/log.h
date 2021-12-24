#pragma once
#include <asio.hpp>
#include <boost/stacktrace.hpp>
#include <cstdint>
#include <iostream>
#include <map>
#include <optional>
#include <sstream>
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
    Log(int surround, std::string host, uint32_t port)
        : host(host), port(port), surround(surround) {}

    void get_stack_trace() {
        auto stacktrace = boost::stacktrace::stacktrace();

        std::for_each(stacktrace.begin() + 1, stacktrace.end(),
                      [](boost::stacktrace::frame frame) {
                          std::cout << frame.source_file() << std::endl;
                      });
    }

    static void get_code(std::string file_path, uint32_t line_number) {}

    static void get_code_snippet(std::string file_path,
                                 uint32_t line_number,
                                 uint32_t surround) {}
};

template <typename T>
std::optional<asio::error_code> log(T message,
                                    int surround = 2,
                                    std::string host = "126.0.0.1",
                                    uint32_t port = 3001) {
    Log<T> log(surround, host, port);

    asio::error_code error_code;
    asio::io_context context;
    asio::ip::tcp::endpoint endpoint(asio::ip::make_address(host, error_code),
                                     port);

#ifndef DEBUG
    std::cerr
        << "Unfortunately, using this function without debug symbols enabled "
           "will produce limited information. The stack trace, file path and "
           "line number will be missing from the final message that is sent to "
           "the server. Please consider guarding this function using #ifdef "
           "DEBUG symbols so that this message does not re-appear.";

    log.warnings.push_back(
        "File was compiled without debug info, meaning information was lost");
#endif

    std::ostringstream stream;
    stream << message;
    log.message = stream.str();

    log.get_stack_trace();

    return {};
}

}  // namespace CodeCtrl