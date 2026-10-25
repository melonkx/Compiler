#include "Parser.h"
#include <format>

Parser::Parser(TokenReader& reader_, DiagnosticsEngine& diagnostics_)
    : m_reader(reader_), m_diagnostics(diagnostics_), m_rule_selector(m_reader)
{}

Diagnostic Parser::make_expected(const std::string& message) {
    return {
        .location = reader().current().location,
        .code = "E001",
        .severity = Severity::Error,
        .message = std::format("expected {} (got \"{}\")", message, reader().current().text),
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
