#pragma once

#include <string>

struct SourceLocation {
    std::size_t line;
    std::size_t column;
    std::string path;
    std::size_t index;
};
