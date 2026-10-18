#pragma once
#include "Parser.h"

class LiteralParser : public Parser {
public:
    AST::Ptr parse() override;        
    void init_rule() override;
private:
    AST::Ptr parse_integer();
    AST::Ptr parse_string();
};
