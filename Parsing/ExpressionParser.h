#pragma once
#include "Parser.h"

class LiteralParser;

class ExpressionParser : public Parser {
public:
    AST::Ptr parse() override;
    void init_rule() override;
    void set_literal_parser(LiteralParser* parser);
private:
    AST::Ptr parse_identifier();
private:
    LiteralParser* m_literal_parser {};
};
