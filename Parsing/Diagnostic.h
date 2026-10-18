#pragma once
#include <string>
#include <vector>

enum class Severity {
    Error,
    Warning,
    Note
};

struct Diagnostic {
    std::string code;
    Severity severity;
    std::string message;
    std::optional<std::string> help;
};
