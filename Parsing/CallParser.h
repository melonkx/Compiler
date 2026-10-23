#pragma once
#include "ListParser.h"

class ExpressionParser;

class CallParser : public Parser {
public:
    CallParser(TokenReader& reader_, DiagnosticsEngine& diagnostics_);
    AST::Ptr parse() override;
    void set_expression_parser(ExpressionParser* parser);
private:
    ExpressionParser* m_expression_parser;
    ListParser m_argument_list_parser;
};
