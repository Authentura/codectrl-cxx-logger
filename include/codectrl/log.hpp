#pragma once

#include <algorithm>
#include <asio.hpp>
#include <boost/stacktrace.hpp>
#include <boost/type_index.hpp>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <jsoncons_ext/cbor/cbor.hpp>
#include <map>
#include <optional>
#include <sstream>
#include <vector>

#include "backtrace_data.h"
#include "codectrl/can_to_string.h"

using namespace jsoncons;

namespace CodeCtrl {
inline std::string trim(std::string& str) {
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

class LogData {
    std::deque<data::BacktraceData> stack_;
    uint32_t line_number_;
    std::map<uint32_t, std::string> code_snippet_;
    std::string message_;
    std::string message_type_;
    std::string file_name_;
    std::string language_;
    std::string address_;
    std::vector<std::string> warnings_;

   public:  // class methods
    LogData(const std::deque<data::BacktraceData>& stack,
            uint32_t line_number,
            const std::map<uint32_t, std::string>& code_snippet,
            const std::string& message,
            const std::string& message_type,
            const std::string& file_name,
            const std::string& language,
            const std::string& address,
            const std::vector<std::string>& warnings)
        : stack_(stack),
          line_number_(line_number),
          code_snippet_(code_snippet),
          message_(message),
          message_type_(message_type),
          file_name_(file_name),
          language_(language),
          address_(address),
          warnings_(warnings){};

   public:  // setters for (de)serialisation
    const std::deque<data::BacktraceData>& stack() const { return stack_; }
    uint32_t line_number() const { return line_number_; }
    const std::map<uint32_t, std::string>& code_snippet() const {
        return code_snippet_;
    }
    const std::string& message() const { return message_; }
    const std::string& message_type() const { return message_type_; }
    const std::string& file_name() const { return file_name_; }
    const std::string& language() const { return language_; }
    const std::string& address() const { return address_; }
    const std::vector<std::string>& warnings() const { return warnings_; }

   public:  // operator overloads
    friend bool operator==(const LogData& lhs, const LogData& rhs) {
        return (lhs.stack_ == rhs.stack_ &&
                lhs.line_number_ == rhs.line_number_ &&
                lhs.code_snippet_ == rhs.code_snippet_ &&
                lhs.message_ == rhs.message_ &&
                lhs.message_type_ == rhs.message_type_ &&
                lhs.file_name_ == rhs.file_name_ &&
                lhs.language_ == rhs.language_ &&
                lhs.address_ == rhs.address_ &&
                lhs.warnings_ == rhs.warnings_);
    }

    friend bool operator!=(const LogData& lhs, const LogData& rhs) {
        return !(lhs == rhs);
    }
};

template <typename T>
class Log {
   public:
    std::deque<data::BacktraceData> stack = {};
    uint32_t line_number = 0;
    std::map<uint32_t, std::string> code_snippet = {};
    std::string message = "";
    std::string message_type = boost::typeindex::type_id<T>().pretty_name();
    std::string file_name = "";
    std::string language = "c++";
    std::string address = "";
    std::vector<std::string> warnings = {};

   private:
    std::string host;
    uint32_t port;
    int surround;
    std::string previous_function = "";
    uint32_t previous_line_number = 0;

   public:  // main class methods
    Log(int surround, std::string host, uint32_t port)
        : host(host), port(port), surround(surround) {}

    void get_stack_trace() {
        auto stacktrace = boost::stacktrace::stacktrace();

        std::for_each(
            stacktrace.begin() + 1, stacktrace.end(),
            [this](boost::stacktrace::frame frame) {
#ifdef DEBUG
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

                    auto file_path = std::filesystem::path(frame.source_file());
                    file_path = std::filesystem::canonical(file_path);

                    data::BacktraceData data(frame.name(), file_path.string(),
                                             line_number, 0, code);

                    stack.push_front(data);
                }
#endif
            });
    }

    std::string get_code(std::string function_name,
                         std::string file_path,
                         uint32_t& line_number) {
        std::string code = "";

        std::ifstream file(file_path);
        uint32_t current_line = 0;

        // NOTE: we have to manually do this because *for whatever reason*, both
        // the ways that gcc on linux do stacktraces do not properly find the
        // proper line numbers, they find the return address instead.
        //
        // TODO: testing on windows to whether or not it has the same issue as
        // gcc on linux
        for (std::string line; std::getline(file, line); current_line++) {
            // if we're still above the previously found match, we continue
            if (current_line < previous_line_number)
                continue;

            // if we have no "previous_function" *and* that the current line is
            // equal to the log function call then we can tell the current line
            // number is correct
            if (previous_function.empty() &&
                (line.find("CodeCtrl::log") != std::string::npos ||
                 line.find("CodeCtrl::log_if") != std::string::npos)) {
                previous_function = function_name;
                previous_line_number = current_line + 1;

                code = trim(line);
                break;
            }

            // if we have a "previous_function" and it's found on this current
            // line then we know that this is the correct line number
            if (!previous_function.empty() &&
                line.find(previous_function) != std::string::npos) {
                previous_function = function_name;
                previous_line_number = current_line + 1;
                line_number = current_line + 1;

                code = trim(line);
                break;
            }
        }

        return code;
    }

    static std::map<uint32_t, std::string> get_code_snippet(
        std::string file_path,
        uint32_t line_number,
        uint32_t surround) {
        std::ifstream file(file_path);

        std::map<uint32_t, std::string> code_snippet = {};
        uint32_t offset_min = line_number - surround;
        uint32_t offset_max = line_number + surround;
        uint32_t current_line_number = 0;

        for (std::string line; std::getline(file, line);
             current_line_number++) {
            if (current_line_number + 1 < offset_min ||
                current_line_number + 1 > offset_max) {
                continue;
            }

            code_snippet[current_line_number + 1] = line;
        }

        return code_snippet;
    }

    LogData into_log_data() {
        LogData data(stack, line_number, code_snippet, message, message_type,
                     file_name, language, address, warnings);
        return data;
    }
};

template <typename T>
std::optional<asio::error_code> log(T message,
                                    int surround = 3,
                                    std::string host = "127.0.0.1",
                                    uint32_t port = 3001) {
    Log<T> log(surround, host, port);

#ifndef DEBUG
    std::cerr
        << "Unfortunately, using this function without debug symbols enabled "
           "will produce limited information. The stack trace, file path and "
           "line number will be missing from the final message that is sent to "
           "the server. Please consider guarding this function using #ifdef "
           "DEBUG so that this message does not re-appear.";

    log.warnings.push_back(
        "File was compiled without debug info, meaning information was lost");
#endif

    if constexpr (can_to_string_v<T>) {
        log.message = std::to_string(message);
    } else if constexpr(has_to_string_v<T>) {
        log.message = message.to_string();
    } else if constexpr(std::is_same_v<T, const char*>) {
        log.message = message;
    } else {
        return {};
    }

    log.get_stack_trace();

    if (log.stack != std::deque<data::BacktraceData>()) {
        data::BacktraceData last = log.stack.back();

        log.code_snippet = Log<T>::get_code_snippet(
            last.file_path(), last.line_number(), surround);
        log.line_number = last.line_number();
        log.file_name = last.file_path();
    }

    std::string data;
    cbor::encode_cbor(log.into_log_data(), data);

    asio::error_code error_code = {};
    asio::io_context context;
    asio::ip::tcp::endpoint endpoint(asio::ip::make_address(host, error_code),
                                     port);

    asio::ip::tcp::resolver resolver(context);
    asio::ip::tcp::resolver::results_type endpoints =
        resolver.resolve(endpoint, error_code);

    asio::ip::tcp::socket socket(context);
    asio::connect(socket, endpoints);

    socket.write_some(asio::buffer(data), error_code);

    if (error_code)
        return error_code;
    else
        return {};
}

template <typename T>
std::optional<asio::error_code> log_if(bool (*const condition)(),
                                       T message,
                                       int surround = 3,
                                       std::string host = "127.0.0.1",
                                       uint32_t port = 3001) {
    if (condition()) {
        return log(message, surround, host, port);
    }

    return {};
}

template <typename T>
std::optional<asio::error_code> log_when_env(T message,
                                             int surround = 3,
                                             std::string host = "127.0.0.1",
                                             uint32_t port = 3001) {
    if (std::getenv("CODECTRL_DEBUG")) {
        return log(message, surround, host, port);
    }
#ifdef DEBUG
    else {
        std::cerr << "log not called: CODECTRL_DEBUG environment variable is "
                     "not present\n";
    }
#endif

    return {};
}
}  // namespace CodeCtrl

JSONCONS_ALL_CTOR_GETTER_TRAITS(CodeCtrl::LogData,
                                stack,
                                code_snippet,
                                message,
                                message_type,
                                line_number,
                                file_name,
                                address,
                                warnings)

JSONCONS_ALL_CTOR_GETTER_TRAITS(CodeCtrl::data::BacktraceData,
                                name,
                                file_path,
                                line_number,
                                column_number,
                                code)
