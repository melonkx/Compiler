#include "ExpressionParser.h"
#include <sstream>

ExpressionParser::ExpressionParser(TokenReader& reader_, DiagnosticsEngine& diagnostics_) :
    Parser(reader_, diagnostics_) 
{
    set_rule(TokenType::Identifier,     &ExpressionParser::parse_identifier,      false);
    set_rule(TokenType::LiteralInteger, &ExpressionParser::parse_literal_integer, false);
    set_rule(TokenType::LiteralString,  &ExpressionParser::parse_literal_string,  false);
}

AST::Ptr ExpressionParser::parse() {
    if (auto ptr = rule_selector().accept(); ptr) {
        return ptr;
    }
    // unexpected but not an error
    return nullptr;
}

AST::Ptr ExpressionParser::parse_identifier() {
    AST::Identifier identifier;
    identifier.name = reader().advance().text;
    // TODO: Binary operations!
    return make_ast(std::move(identifier));
}

AST::Ptr ExpressionParser::parse_literal_integer() {
    std::stringstream stringstream (reader().advance().text);
    AST::LiteralInteger literal_integer;
    stringstream >> literal_integer.value;
    // TODO: Binary operations!
    return make_ast(std::move(literal_integer));
}

AST::Ptr ExpressionParser::parse_literal_string() {
    AST::LiteralString literal_string;
    literal_string.text = reader().advance().text;
    // TODO: Stringy operations!
    return make_ast(std::move(literal_string));
}

