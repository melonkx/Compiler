#include "Parser.h"

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
