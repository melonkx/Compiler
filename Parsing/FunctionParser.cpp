#include "FunctionParser.h"
#include "StatementParser.h"
#include "ExpressionParser.h"

AST::Ptr FunctionParser::parse() {
    return nullptr;
}

void FunctionParser::init_rule() {
}

void FunctionParser::set_statement_parser(StatementParser* statement_parser) {
    m_statement_parser = statement_parser;
}

void FunctionParser::set_expression_parser(ExpressionParser* expression_parser) {
    m_expression_parser = expression_parser;
}

AST::Ptr FunctionParser::parse_parameters() {
    return nullptr;
}
