#include <algorithm>
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <boost/stacktrace.hpp>
#include "codectrl.h"

using namespace CodeCtrl::log;

template <typename T>
Log<T>::Log(int surround, std::string host, uint32_t port)
    : address(""),
      message(message),
      message_type(typeid(T).name()),
      stack({}),
      warnings({}),
      file_name(""),
      host(host),
      port(port),
      surround(surround) {}

template <typename T>
std::optional<asio::error_code> Log<T>::log(std::string message) {
    asio::error_code ec;
    asio::io_context context;
    asio::ip::tcp::endpoint endpoint(asio::ip::make_address(host, ec), port);

    std::cout << message << std::endl;
    get_stack_trace();
}

template <typename T>
void Log<T>::get_stack_trace() {
    auto stacktrace = boost::stacktrace::stacktrace();

    std::for_each(stacktrace.begin(), stacktrace.end(),
                  [](boost::stacktrace::frame frame) {
                      std::cout << frame.source_file() << std::endl;
                  });
}

template <typename T>
void Log<T>::get_code(std::string file_path, uint32_t line_number) {}

template <typename T>
void Log<T>::get_code_snippet(std::string file_path,
                              uint32_t line_number,
                              uint32_t surround) {}