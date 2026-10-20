#include "DiagnosticsEngine.h"

void DiagnosticsEngine::report(Diagnostic diagnostic) {
    if (diagnostic.severity == Severity::Error) {
        m_reported_error = true;
    }
    if (m_hook) {
        m_hook(diagnostic);
    }
    m_diagnostics.emplace_back(std::move(diagnostic));
}

bool DiagnosticsEngine::has_error() const {
    return m_reported_error;
}

std::span<const Diagnostic> DiagnosticsEngine::all() const {
    return m_diagnostics;
}

void DiagnosticsEngine::hook(Hook&& hook) {
    m_hook = hook;
}

void DiagnosticsEngine::unhook() {
    m_hook = nullptr;
}

