#pragma once
#include <string>

namespace CodeCtrl::data {
struct BacktraceData {
    std::string name;
    std::string file_path;
    uint32_t line_number;
    uint32_t column_number;
    std::string code;
};
}  // namespace CodeCtrl::data