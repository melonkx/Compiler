#pragma once
#include "Parser.h"

class FunctionParser;

class ModuleParser : public Parser {
public:
    AST::Ptr parse() override;
    void init_rule() override;
    void set_function_parser(FunctionParser* function_parser);
private:
    AST::Ptr parse_object();
    AST::Ptr parse_function();
private:
    FunctionParser* m_function_parser;
};
