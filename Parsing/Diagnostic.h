#pragma once
#include <string>
#include <vector>
#include "../SourceLocation.h"

enum class Severity {
    Error,
    Warning,
    Note
};

struct Diagnostic {
    SourceLocation location;
    std::string code;
    Severity severity;
    std::string message;
    std::optional<std::string> help;
};
