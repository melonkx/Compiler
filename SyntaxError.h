#pragma once

#include "SourceLocation.h"

struct SyntaxError {
    std::string message;
    SourceLocation location;
};
