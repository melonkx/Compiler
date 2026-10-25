#pragma once
#include "TypeParser.h"
#include "StatementParser.h"
#include "ExpressionParser.h"
#include "ParameterParser.h"
#include "ListParser.h"

class FunctionParser : public Parser {
public:
    FunctionParser(TokenReader& reader_, DiagnosticsEngine& diagnostics_);
    AST::Ptr parse() override; 

private:
    TypeParser m_type_parser;
    StatementParser m_statement_parser;
    ExpressionParser m_expression_parser;
    ParameterParser m_parameter_parser;
    ListParser m_parameter_list_parser;
};
