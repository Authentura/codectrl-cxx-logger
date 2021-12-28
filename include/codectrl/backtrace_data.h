#pragma once
#include <cstdint>
#include <string>

namespace CodeCtrl::data {
class BacktraceData {
   public:
    std::string name_;
    std::string file_path_;
    uint32_t line_number_;
    uint32_t column_number_;
    std::string code_;

   public:  // class methods
    BacktraceData(const std::string& name,
                  const std::string& file_path,
                  uint32_t line_number,
                  uint32_t column_number,
                  const std::string& code)
        : name_(name),
          file_path_(file_path),
          line_number_(line_number),
          column_number_(column_number),
          code_(code) {}

   public:  // getter methods for (de)serialisation
    const std::string& name() const { return name_; }
    const std::string& file_path() const { return file_path_; }
    uint32_t line_number() const { return line_number_; }
    uint32_t column_number() const { return column_number_; }
    const std::string& code() const { return code_; }

   public:  // operator overloads
    friend bool operator==(const BacktraceData& lhs, const BacktraceData& rhs) {
        return (lhs.name_ == rhs.name_ && lhs.file_path_ == rhs.file_path_ &&
                lhs.line_number_ == rhs.line_number_ &&
                lhs.column_number_ == rhs.column_number_ &&
                lhs.code_ == rhs.code_);
    }

    friend bool operator!=(const BacktraceData& lhs, const BacktraceData& rhs) {
        return !(lhs == rhs);
    }
};
}  // namespace CodeCtrl::data
