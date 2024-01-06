#pragma once
#include <iostream>
#include <string>
#include <optional>
namespace rst
{
    enum ErrorType
    {
        OPTION_ERROR,
        RUNTIME_ERROR,
        SYNTAX_ERROR,
        FATAL_ERROR,
    };
    void rst_error(ErrorType error_type, const std::string &msg);
    std::optional<std::string> read_file(const std::string &path);
    void debug_print(const std::string &msg);
}
