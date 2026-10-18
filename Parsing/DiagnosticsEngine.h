#pragma once
#include <vector>
#include <span>
#include <optional>
#include "Diagnostic.h"

class DiagnosticsEngine {
public:
    void report(Diagnostic diagnostic);
    bool has_error() const;
    std::span<const Diagnostic> all() const;
private:
    std::vector<Diagnostic> m_diagnostics;
    bool m_reported_error{};
};


