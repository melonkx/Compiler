#pragma once
#include <vector>
#include <span>
#include <optional>
#include <functional>
#include "Diagnostic.h"

class DiagnosticsEngine {
public:
    using Hook = std::function<void(const Diagnostic&)>;
    void report(Diagnostic diagnostic);
    bool has_error() const;
    std::span<const Diagnostic> all() const;
    void hook(Hook&& hook);
    void unhook();
private:
    std::vector<Diagnostic> m_diagnostics;
    bool m_reported_error{};
    Hook m_hook;
};


