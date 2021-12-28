#pragma once
#include <algorithm>
#include <asio.hpp>
#include <boost/stacktrace.hpp>
#include <cstdint>
#include <deque>
#include <fstream>
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
    std::deque<data::BacktraceData> stack = {};
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
    std::string previous_function = "";
    uint32_t previous_line_number = 0;

   public:
    Log(int surround, std::string host, uint32_t port)
        : host(host), port(port), surround(surround) {}

    void get_stack_trace() {
        auto stacktrace = boost::stacktrace::stacktrace();

        std::for_each(
            stacktrace.begin() + 1, stacktrace.end(),
            [this](boost::stacktrace::frame frame) {
                // this is some potentially confusing syntax so i will clarify:
                // std::string::find returns the index of the first character
                // where the substring is found. we compare whether the fact the
                // found index is equal to std::string::npos because npos is
                // equal to the *largest possible size_t index* as anything
                // equal to npos is not found within the given std::string.
                // therefore if we want the string *not* to be found we wanna
                // make sure that std::string::find returns the value of npos.
                if (frame.name().find("CodeCtrl::log") == std::string::npos &&
                    frame.name().find("libc") == std::string::npos &&
                    frame.name() != "_start" && !frame.name().empty()) {
                    uint32_t line_number = frame.source_line();

                    std::string code = get_code(
                        frame.name(), frame.source_file(), line_number);

                    data::BacktraceData data(frame.name(), frame.source_file(),
                                             line_number, 0, code);

                    stack.push_front(data);
                }
            });
    }

    std::string get_code(std::string function_name,
                         std::string file_path,
                         uint32_t& line_number) {
        std::string code = "";

        std::ifstream file(file_path);
        uint32_t current_line = 0;

        for (std::string line; std::getline(file, line); current_line++) {
            // if we're still above the previously found match, we continue
            if (current_line < previous_line_number)
                continue;

            // if we have no "previous_function" *and* that the current line is
            // equal to the log function call then we can tell the current line
            // number is correct
            if (previous_function.empty() &&
                line.find("CodeCtrl::log") != std::string::npos) {
                previous_function = function_name;
                previous_line_number = current_line + 1;

                code = line;
                break;
            }

            // if we have a "previous_function" and it's found on this current
            // line then we know that this is the correct line number
            if (!previous_function.empty() &&
                line.find(previous_function) != std::string::npos) {
                previous_function = function_name;
                previous_line_number = current_line + 1;
                line_number = current_line + 1;

                code = line;
                break;
            }
        }

        return code;
    }

    static std::map<uint32_t, std::string> get_code_snippet(
        std::string file_path,
        uint32_t line_number,
        uint32_t surround) {
        return {};
    }
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