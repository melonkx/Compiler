#include "ExpressionParser.h"
#include <sstream>

ExpressionParser::ExpressionParser(TokenReader& reader_, DiagnosticsEngine& diagnostics_) :
    Parser(reader_, diagnostics_),
    m_argument_list_parser(reader_, diagnostics_)
{
    set_rule(TokenType::Identifier,     &ExpressionParser::parse_identifier,        false);
    set_rule(TokenType::At,             &ExpressionParser::parse_address_from,      true);
    set_rule(TokenType::Ampersand,      &ExpressionParser::parse_address_of,        true);
    set_rule(TokenType::LiteralInteger, &ExpressionParser::parse_literal_integer,   false);
    set_rule(TokenType::LiteralString,  &ExpressionParser::parse_literal_string,    false);
    
    m_argument_list_parser.set_delimiters(TokenType::LeftParen, TokenType::RightParen);
    m_argument_list_parser.set_element_parser(this);
}

AST::Ptr ExpressionParser::parse() {
    if (auto ptr = rule_selector().accept(); ptr) {
        return ptr;
    }
    // unexpected but not an error
    return nullptr;
}

ListParser& ExpressionParser::argument_list_parser() {
    return m_argument_list_parser;
}

AST::Ptr ExpressionParser::parse_identifier() {
    auto identifier = make_ast(
        AST::Identifier{.name = reader().advance().text}
    );
    
    if (reader().is(m_argument_list_parser.opening())) {
        return parse_function_call(std::move(identifier));
    }

    return std::move(identifier);
}

AST::Ptr ExpressionParser::parse_function_call(AST::Ptr&& function) {
    AST::Ptr arguments = m_argument_list_parser.parse();
    return make_ast(AST::Call{
        .function = std::move(function),
        .arguments = std::move(arguments)
    });
}

AST::Ptr ExpressionParser::parse_address_of() {
    return make_ast(AST::AddressOf{parse()});
}

AST::Ptr ExpressionParser::parse_address_from() {
    return make_ast(AST::AddressFrom{parse()});
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

