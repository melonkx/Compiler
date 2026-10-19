#pragma once
#include "Parser.h"


class FunctionParser : public Parser {
public:
    AST::Ptr parse() override;
    void init_rule() override; 
    void set_statement_parser(StatementParser* statement_parser);
    void set_expression_parser(ExpressionParser* expression_parser);
private:
    AST::Ptr parse_parameters();
private:
    StatementParser* m_statement_parser;
    ExpressionParser* m_expression_parser;
};
