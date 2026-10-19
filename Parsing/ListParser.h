#pragma once
#include "Parser.h"

class ListParser : public Parser {
public:
    AST::Ptr parse() override;
    void init_rule() override;
    void set_delimiters(TokenType opening, TokenType closing);
    TokenType opening_delimiter() const;
    void set_element_parser(Parser* parser);
private:
    TokenType m_opening;
    TokenType m_closing;
    Parser* m_element_parser;
};
