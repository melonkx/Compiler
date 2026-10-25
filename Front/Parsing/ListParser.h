#pragma once
#include "Parser.h"

class ListParser : public Parser {
public:
    ListParser(TokenReader& reader_, DiagnosticsEngine& diagnostics_);
    AST::Ptr parse() override;
    void set_delimiters(TokenType opening, TokenType closing);
    TokenType opening() const;
    void set_element_parser(Parser* parser);
private:
    TokenType m_opening;
    TokenType m_closing;
    Parser* m_element_parser;
};
