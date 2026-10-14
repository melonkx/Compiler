#pragma once

#include <string>

struct SourceLocation {
    unsigned long line;
    unsigned long column;
    std::string path;
};
