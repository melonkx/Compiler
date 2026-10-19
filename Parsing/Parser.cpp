#include "Parser.h"

Parser::Parser(TokenReader& reader_, DiagnosticsEngine& diagnostics_)
    : m_reader(reader_), m_diagnostics(diagnostics_)
{}

Diagnostic Parser::make_expected(const std::string& message) {
    return {
        .code = "E001",
        .severity = Severity::Error,
        .message = message,
        .help = std::nullopt
    };
}

const ProductionRuleSelector& Parser::rule_selector() const {
    return m_rule_selector;
}

TokenReader& Parser::reader() {
    return m_reader;
}

DiagnosticsEngine& Parser::diagnostics() {
    return m_diagnostics;
}
