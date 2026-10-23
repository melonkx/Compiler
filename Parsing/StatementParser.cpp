#include "StatementParser.h"
#include "ExpressionParser.h"
#include "TypeParser.h"

StatementParser::StatementParser(TokenReader& reader_, DiagnosticsEngine& diagnostics_) :
    Parser(reader_, diagnostics_)
{
    set_rule(TokenType::If,             &StatementParser::parse_if);
    set_rule(TokenType::Return,         &StatementParser::parse_return);
    set_rule(TokenType::Const,          &StatementParser::parse_const);
    set_rule(TokenType::Var,            &StatementParser::parse_var);
    set_rule(TokenType::LeftBracket,    &StatementParser::parse_block);
    set_rule(TokenType::Identifier,     &StatementParser::parse_identifier, false);
}

AST::Ptr StatementParser::parse() {
    if (m_expression_parser == nullptr) {
        throw std::runtime_error("StatementParser::m_expression_parser == nullptr");
    }

    if (m_type_parser == nullptr) {
        throw std::runtime_error("StatementParser::m_type_parser == nullptr");
    }

    // Consume any stray end of statements
    reader().consume(TokenType::EndOfStatement);

    if (auto&& ptr = rule_selector().accept(); ptr) {
        if (!reader().consume(TokenType::EndOfStatement)) {
            diagnostics().report(make_expected("end of statement"));
        }
        
        return ptr;
    }
    // .. error: expected statement
    return nullptr;
}

void StatementParser::set_expression_parser(ExpressionParser* parser) {
    m_expression_parser = parser;
}

void StatementParser::set_type_parser(TypeParser* parser) {
    m_type_parser = parser;
}

AST::Ptr StatementParser::parse_if() {
    AST::If if_statement;
    if_statement.condition = m_expression_parser->parse();
    
    if (!reader().consume(TokenType::Then)) {
        diagnostics().report(make_expected("'then' after if condition")); 
    }

    if_statement.then_true = parse();
    
    if (reader().consume(TokenType::Else)) {
        if_statement.then_false = parse();
    }

    return make_ast(std::move(if_statement));
}

AST::Ptr StatementParser::parse_const() {
    auto decl = parse_variable_declaration();
    AST::Variable* decl_var = (AST::Variable*)decl.get();
    decl_var->is_const = true;
    if (!decl_var->initialization) {
        diagnostics().report(make_expected("constant initializer expression"));
    }
    return decl;
}

AST::Ptr StatementParser::parse_var() {
    auto decl = parse_variable_declaration();
    AST::Variable* decl_var = (AST::Variable*)decl.get();

    if (!decl_var->initialization && !decl_var->type) {
        diagnostics().report(make_expected("variable initialization or type declaration"));
    }
    return decl;
}

AST::Ptr StatementParser::parse_variable_declaration() {
    AST::Variable variable;
    
    if (reader().is(TokenType::Identifier)) {
        variable.name = reader().advance().text;
    }
    else {
        // TODO: Better error messages!
        diagnostics().report(make_expected("variable name"));  
    }
    
    if (reader().consume(TokenType::Colon)) {
        variable.type = m_type_parser->parse();
    }

    if (reader().consume(TokenType::Equal)) {
        variable.initialization = m_expression_parser->parse(); 
    }
    
    return make_ast(std::move(variable));
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
            reader().advance();
        }
    }

    if (!found_ending_brace) {
        diagnostics().report(make_expected("closing brace")); 
    }

    return make_ast(std::move(block));
}

AST::Ptr StatementParser::parse_return() {
    AST::Return return_statement;
    return_statement.expression = m_expression_parser->parse();
    return make_ast(std::move(return_statement));
}

AST::Ptr StatementParser::parse_identifier() {
    auto identifier = make_ast(AST::Identifier{reader().advance().text});
    
    if (reader().is(m_expression_parser->argument_list_parser().opening())) {
        return m_expression_parser->parse_function_call(std::move(identifier));
    }

    diagnostics().report(make_expected("function call"));
    reader().advance();
    return nullptr;
}
