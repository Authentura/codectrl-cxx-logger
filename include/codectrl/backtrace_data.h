#pragma once
#include <cstdint>
#include <string>

namespace CodeCtrl::data {
struct BacktraceData {
    std::string name_;
    std::string file_path_;
    uint32_t line_number_;
    uint32_t column_number_;
    std::string code_;

    // class methods
    BacktraceData(const std::string&, const std::string&, uint32_t, uint32_t, const std::string&);

    // getter methods for (de)serialisation
    const std::string& name() const;
    const std::string& file_path() const;
    uint32_t line_number() const;
    uint32_t column_number() const;
    const std::string& code() const;

    // operator overloads
    friend bool operator==(const BacktraceData&, const BacktraceData&);
    friend bool operator!=(const BacktraceData&, const BacktraceData&);
};
}  // namespace CodeCtrl::data
