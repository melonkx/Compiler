#include "StatementParser.h"
#include "ExpressionParser.h"

StatementParser::StatementParser(TokenReader& reader_, DiagnosticsEngine& diagnostics_) :
    Parser(reader_, diagnostics_)
{
    set_rule(TokenType::If,             &StatementParser::parse_if);
    set_rule(TokenType::Return,         &StatementParser::parse_return);
    set_rule(TokenType::Const,          &StatementParser::parse_const);
    set_rule(TokenType::Var,            &StatementParser::parse_var);
    set_rule(TokenType::LeftBracket,    &StatementParser::parse_block);
}

AST::Ptr StatementParser::parse() {
    if (auto&& ptr = rule_selector().accept(); ptr) {
        return ptr;
    }
    // .. error: expected statement
    return nullptr;
}

AST::Ptr StatementParser::parse_if() {
    AST::If if_statement;
    if_statement.condition = m_expression_parser->parse();
    
    if (!reader().consume(TokenType::Then)) {
        // error: expected 'then' after if condition
    }

    if_statement.then_true = parse();
    
    if (reader().consume(TokenType::Else)) {
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
    AST::Block block;
    bool found_ending_brace = false;

    while (!reader().at_end()) {
        if (reader().consume(TokenType::RightBracket)) {
            found_ending_brace = true;
            break;
        }

        if (auto statement = parse(); statement) {
            block.statements.emplace_back(std::move(statement));
        } else {
            // error...
            diagnostics().report(make_expected("statement in block"));
        }
    }

    if (!found_ending_brace) {
        diagnostics().report(make_expected("closing brace")); 
    }

    return make_ast(std::move(block));
}

AST::Ptr StatementParser::parse_return() {
    return nullptr;
}
