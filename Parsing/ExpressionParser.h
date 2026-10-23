#pragma once
#include "ListParser.h"

class ExpressionParser : public Parser {
public:
    ExpressionParser(TokenReader& reader_, DiagnosticsEngine& diagnostics_);
    AST::Ptr parse() override;
    ListParser& argument_list_parser();
    AST::Ptr parse_function_call(AST::Ptr&& function);
private:
    AST::Ptr parse_identifier(); 
    AST::Ptr parse_address_of();
    AST::Ptr parse_address_from();
    AST::Ptr parse_literal_integer();
    AST::Ptr parse_literal_string();
private:
    ListParser m_argument_list_parser;
};
