#include "log.h"
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

using namespace CodeCtrl::log;

template <typename T>
Log<T>::Log(std::string message, int surround, std::string host, uint32_t port)
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
std::optional<asio::error_code> Log<T>::log() {
    asio::error_code ec;
    asio::io_context context;
    asio::ip::tcp::endpoint endpoint(asio::ip::make_address(host, ec), port);
}