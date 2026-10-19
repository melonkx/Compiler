#pragma once
#include "Parser.h"

class ExpressionParser;

class StatementParser : public Parser {
public:
    StatementParser(TokenReader& reader_, DiagnosticsEngine& diagnostics_);
    AST::Ptr parse() override;
    void set_expression_parser(ExpressionParser* parser);
private:
    AST::Ptr parse_if();
    AST::Ptr parse_block();
    AST::Ptr parse_return();
    AST::Ptr parse_const();
    AST::Ptr parse_var();
private:
    ExpressionParser* m_expression_parser {}; 
};
