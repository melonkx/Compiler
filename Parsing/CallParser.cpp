#pragma once

#include "CallParser.h"

CallParser::CallParser(TokenReader& reader_, DiagnosticsEngine& diagnostics_) :
    Parser(reader_, diagnostics_),
    m_argument_list_parser(reader_, diagnostics_)
{
    m_argument_list_parser.set_delimiters(TokenType::LeftParen, TokenType::RightParen);
}

AST::Ptr CallParser::parse() override {
    if (reader().i
}

void CallParser::set_expression_parser(ExpressionParser* parser) {
    m_expression_parser = parser;
    m_argument_list_parser.set_element_parser(parser);
}
