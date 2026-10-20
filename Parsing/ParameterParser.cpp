#include "ParameterParser.h"
#include "TypeParser.h"

ParameterParser::ParameterParser(TokenReader& reader_, DiagnosticsEngine& diagnostics_) 
    : Parser(reader_, diagnostics_)
{}

AST::Ptr ParameterParser::parse() {
    AST::FunctionParameter parameter;

    if (!reader().is(TokenType::Identifier)) {
        diagnostics().report(make_expected("parameter name"));
    }

    parameter.name = reader().advance().text;

    if (!reader().consume(TokenType::Colon)) {
        diagnostics().report(make_expected(": <type> after parameter name"));
    }

    parameter.type = m_type_parser->parse();
    return make_ast(std::move(parameter));
}

void ParameterParser::set_type_parser(TypeParser* parser) {
    m_type_parser = parser;
}
