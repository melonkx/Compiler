#pragma once
#include "Parser.h"


class ExpressionParser : public Parser {
public:
    AST::Ptr parse() override;
    void init_rule() override;
private:
    AST::Ptr parse_identifier();
    AST::Ptr parse_literal_integer();
    AST::Ptr parse_literal_string();
};
