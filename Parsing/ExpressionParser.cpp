#include "ExpressionParser.h"
#include <sstream>

AST::Ptr ExpressionParser::parse() {
    if (auto ptr = rule_selector().accept(); ptr) {
        return ptr;
    }
    // unexpected...
    return nullptr;
}

void ExpressionParser::init_rule() {
    bool is_consuming = false;
    set_rule(TokenType::Identifier, &ExpressionParser::parse_identifier, is_consuming);
    set_rule(TokenType::LiteralInteger, &ExpressionParser::parse_literal_integer, is_consuming);
    set_rule(TokenType::LiteralString, &ExpressionParser::parse_literal_string, is_consuming);
}

AST::Ptr ExpressionParser::parse_identifier() {
    AST::Identifier identifier;
    identifier.name = reader().advance().text;
    return make_ast(std::move(identifier));
}

AST::Ptr ExpressionParser::parse_literal_integer() {
    std::stringstream stringstream (reader().advance().text);
    AST::LiteralInteger literal_integer;
    stringstream >> literal_integer.value;
    return make_ast(std::move(literal_integer));
}

AST::Ptr ExpressionParser::parse_literal_string() {
    AST::LiteralString literal_string;
    literal_string.text = reader().advance().text;
    return make_ast(std::move(literal_string));
}

