#pragma once
#include "Parser.h"

class ExpressionParser;

class TypeParser : public Parser {
public:
    TypeParser(TokenReader& reader_, DiagnosticsEngine& diagnostics_);
    AST::Ptr parse() override;
    void set_expression_parser(ExpressionParser* parser);
private:
    AST::Ptr parse_pointer(AST::Ptr&& base_type);
    AST::Ptr parse_bounded_pointer(AST::Ptr&& base_type);
private:
    ExpressionParser* m_expression_parser{};
};
