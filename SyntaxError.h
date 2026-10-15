#pragma once

#include <optional>

#include "SourceLocation.h"

struct Suggestion {
    std::string message;
};

struct SyntaxError {
    std::string message;
    SourceLocation location;
    std::optional<Suggestion> suggestion;
};
