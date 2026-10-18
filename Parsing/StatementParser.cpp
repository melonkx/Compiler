#include "StatementParser.h"
#include "ExpressionParser.h"

void StatementParser::init_rule() {
    set_rule(TokenType::If,             &StatementParser::parse_if);
    set_rule(TokenType::Return,         &StatementParser::parse_return);
    set_rule(TokenType::Const,          &StatementParser::parse_const);
    set_rule(TokenType::Var,            &StatementParser::parse_var);
    set_rule(TokenType::LeftBracket,    &StatementParser::parse_block);
}

AST::Ptr StatementParser::parse() {
    if (auto&& ptr = m_rule_selector.accept(); ptr) {
        return ptr;
    }
    // .. error: expected statement
    return nullptr;
}

AST::Ptr StatementParser::parse_if() {
    AST::If if_statement;
    if_statement.condition = m_expression_parser->parse();
    
    if (!m_reader.consume(TokenType::Then)) {
        // error: expected 'then' after if condition
    }

    if_statement.then_true = parse();
    
    if (m_reader.consume(TokenType::Else)) {
        if_statement.then_false = parse();
    }

    return make_ast(std::move(if_statement));
}

AST::Ptr StatementParser::parse_const() {
    return nullptr;
}

AST::Ptr StatementParser::parse_var() {
    return nullptr;
}

AST::Ptr StatementParser::parse_block() {
    return nullptr;
}

AST::Ptr StatementParser::parse_return() {
    return nullptr;
}
