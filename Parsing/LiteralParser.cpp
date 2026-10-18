#include "LiteralParser.h"
#include <sstream>

AST::Ptr LiteralParser::parse() {
    if (auto&& ptr = m_rule_selector.accept(); ptr) {
        return ptr;
    }
    // unexpected
    return nullptr;
}

void LiteralParser::init_rule() {
    bool is_consuming = false;
    set_rule(TokenType::LiteralInteger, &LiteralParser::parse_integer,  is_consuming);
    set_rule(TokenType::LiteralString,  &LiteralParser::parse_string,   is_consuming);
}

AST::Ptr LiteralParser::parse_integer() {
    std::stringstream stream (m_reader.current().text);

    AST::LiteralInteger integer;
    stream >> integer.value;

    m_reader.advance();
    return make_ast(std::move(integer));
}

AST::Ptr LiteralParser::parse_string() {
    AST::LiteralString string;
    string.text = m_reader.advance().text;
    return make_ast(std::move(string));
}
