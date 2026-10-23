#pragma once
#include "Parser.h"

class ExpressionParser;
class TypeParser;

class StatementParser : public Parser {
public:
    StatementParser(TokenReader& reader_, DiagnosticsEngine& diagnostics_);
    AST::Ptr parse() override;
    void set_expression_parser(ExpressionParser* parser);
    void set_type_parser(TypeParser* parser);
private:
    AST::Ptr parse_if();
    AST::Ptr parse_block();
    AST::Ptr parse_return();
    AST::Ptr parse_const();
    AST::Ptr parse_var();
    AST::Ptr parse_variable_declaration();
    AST::Ptr parse_identifier();
private:
    ExpressionParser* m_expression_parser {};
    TypeParser* m_type_parser{};
};
