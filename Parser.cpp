#include "Parser.h"

#include <iostream>
#include <sstream>
#include <source_location>
#include <format>

#define OUTPUT_LOCATION do { \
    const auto location = std::source_location::current(); \
    std::cout << location.line() << "\t| " << location.function_name() <<  std::endl; \
} while (0)

Parser::Parser(const std::span<const Token> tokens) : 
    m_reader(tokens) {
}

AST::Ptr Parser::parse() {
    OUTPUT_LOCATION;

    AST::Module mod;

    while (!m_reader.at_end()) {
        if (m_reader.consume(TokenType::Function)) {
            auto func = parse_function();
            
            if (!func) {
                continue;
            }

            mod.functions.emplace_back(std::move(func));
        }
        else {
            // Throw an error
            break;
        }
    }
 
    return make_ast(std::move(mod));
}

AST::Ptr Parser::parse_expression() {
    OUTPUT_LOCATION;

    if (m_reader.is(TokenType::LiteralInteger)) {
        return parse_literal_integer();
    }

    if (m_reader.is(TokenType::LiteralString)) {
        return parse_literal_string();
    }

    if (m_reader.is(TokenType::Identifier)) {
        return parse_identifier();
    }

    expected_error("expression");
    return nullptr;
}

AST::Ptr Parser::parse_statement() { 
    OUTPUT_LOCATION;

    if (m_reader.consume(TokenType::LeftBracket)) {
        return parse_block();
    }

    if (m_reader.consume(TokenType::If)) {
        return parse_if();
    }

    if (m_reader.consume(TokenType::Return)) {
        return parse_return();
    }

    if (m_reader.consume(TokenType::Const)) {
        return parse_const_declaration();
    }

    if (m_reader.consume(TokenType::Var)) {
        return parse_var_declaration();
    }
    
    expected_error("statement"); 
    return nullptr;
}

AST::Ptr Parser::parse_function() {
    OUTPUT_LOCATION;
    
    AST::Function function;

    // Forms:
    // Expressive form: function <name> (params) => <expr>
    // Block Form:      function <name> (params) { }
    // (params) is *Optional*

    // Read function name
    if (!m_reader.is(TokenType::Identifier)) {
        expected_error("function name");
    }

    if (!m_reader.at_end()) {
        function.name = m_reader.advance().text;
        
        if (failed()) {
            function.name += " (failing)";
        }
    }

    // Read parameters
    if (m_reader.consume(TokenType::LeftParen)) {
        function.parameters = parse_function_parameters();
    }

    // Read body
    if (m_reader.consume(TokenType::FunctionArrow)) {
        function.body = parse_statement();
    }
    else if (m_reader.consume(TokenType::LeftBracket)) {
        function.body = parse_block();
    }
    else {
        expected_error("function body as { or =>");
    }

    return make_ast(std::move(function));
}


AST::Ptr Parser::parse_function_parameters() {
    OUTPUT_LOCATION;
    
    AST::FunctionParameters function_parameters;
    bool found_closing_paren = false;

    while (!m_reader.at_end()) {
        if (m_reader.is(TokenType::Identifier)) {
            auto parameter = parse_function_parameter();

            if (parameter) {
                function_parameters.parameters.emplace_back(std::move(parameter));
            }

            if (!m_reader.consume(TokenType::Comma)) {
                if (!m_reader.is(TokenType::RightParen)) {
                    expected_error(") to close function parameter list");
                }
            }
        }
        else if (m_reader.is(TokenType::RightParen)) {
            m_reader.advance();
            found_closing_paren = true;
            break;
        } else {
            expected_error("<TODO> (Function parameters)");
        }

    }
    
    return make_ast(std::move(function_parameters));
}

AST::Ptr Parser::parse_function_parameter() {
    OUTPUT_LOCATION;

    AST::FunctionParameter parameter;

    if (!m_reader.is(TokenType::Identifier)) {
        expected_error("parameter name");
    }

    if (!m_reader.at_end()) {
        parameter.name = m_reader.advance().text;
    }

    if (!m_reader.consume(TokenType::Colon)) {
        // Expected :
        // Error: expected : after 
        expected_error(": after parameter name");
    }

    parameter.type = parse_type();
    return make_ast(std::move(parameter));
}

AST::Ptr Parser::parse_type() {
    OUTPUT_LOCATION;
    // Forms: 
    // TypeName
    // TypeName*            - Pointer to a Single TypeName
    // TypeName* [...]      - Pointer to potentially infinite TypeName's
    // TypeName* [<Min>...] - Pointer to at least Min TypeNames and at Maximu - potentially infinite
    // TypeName* [<Max>]    - Bounded Pointer to at most 'Max' TypeName's
    // TypeName* [<Min>...<Max>]
    //
    //
    //
    // const address0 : Int* [100] = 0x4000
    // const address1 : Int* [100] = 0x8000
    // const x = &address0  

    AST::Type type;

    if (!m_reader.is(TokenType::Identifier)) {
        expected_error("type name");
    }

    if (!m_reader.at_end()) {
        type.name = m_reader.advance().text;
    }

    auto ptr = std::make_unique<AST>(std::move(type));

    if (m_reader.consume(TokenType::Star)) {
        return parse_pointer_type(std::move(ptr));
    }

    return std::move(ptr);
}

AST::Ptr Parser::parse_pointer_type(AST::Ptr&& type) {
    OUTPUT_LOCATION;
    if (m_reader.consume(TokenType::LeftSquareBracket)) {
        return parse_bounded_pointer_type(std::move(type));
    }

    AST::PointerType pointer_type;
    pointer_type.base_type = std::move(type);

    return make_ast(std::move(pointer_type));
}

AST::Ptr Parser::parse_bounded_pointer_type(AST::Ptr&& type) {
    OUTPUT_LOCATION;

    AST::BoundedPointerType bounded_pointer_type;
    bounded_pointer_type.base_type = std::move(type);

    bool began_with_triple_dot {};

    // Type* [...]
    if (m_reader.consume(TokenType::TripleDot)) {
        began_with_triple_dot = true;
        if (!m_reader.is(TokenType::RightSquareBracket)) {
            expected_error("] to close pointer bounds",
                   Suggestion{.message = "Type* [Maximum] or Type* [...]"}
            );
        }
        else {
            return make_ast(std::move(bounded_pointer_type));
        }
    }

    auto first_constraint = parse_expression();

    if (!first_constraint && !began_with_triple_dot) {
        expected_error("constraint for pointer bounds");
    }

    // Type* [Min...] or Type* [Min...Max]
    if (m_reader.consume(TokenType::TripleDot)) {
        bounded_pointer_type.minimum = std::move(first_constraint);

        // Type* [Min...]
        if (m_reader.consume(TokenType::RightSquareBracket)) {
            return make_ast(std::move(bounded_pointer_type));
        }

        // Type* [Min...Max]
        auto second_constraint = parse_expression();
        
        if (!m_reader.consume(TokenType::RightSquareBracket)) {
            expected_error("] after maximum to close pointer bounds");
            return nullptr;
        }

        bounded_pointer_type.maximum = std::move(second_constraint);
    }
    else {
        // Type* [Max]
        if (!m_reader.consume(TokenType::RightSquareBracket)) {
            std::optional<Suggestion> suggestion;
            if (first_constraint) {
                suggestion = {
                    .message = "Type* [maximum]"
                };
            }

            expected_error("] after maximum to close pointer bounds", suggestion);
            return nullptr;
        }

        bounded_pointer_type.maximum = std::move(first_constraint);
    }

    return make_ast(std::move(bounded_pointer_type));
}

AST::Ptr Parser::parse_block() {
    OUTPUT_LOCATION;
   
    AST::Block block;
    bool found_closing_brace = false;

    // Read until we hit a } or there's a syntax error
    while (!m_reader.at_end()) {
        if (m_reader.is(TokenType::RightBracket)) {
            m_reader.advance();
            found_closing_brace = true;
            break;
        }

        auto statement = parse_statement();
        
        if (statement) {
            block.statements.emplace_back(std::move(statement));
        } else {
            m_reader.advance();
        }
    }
    
    // Error:
    if (!found_closing_brace) {
        return nullptr;
    }

    return make_ast(std::move(block));
}
#pragma region Expressions

AST::Ptr Parser::parse_literal_integer() {
    OUTPUT_LOCATION;

    AST::Ptr literal_integer = make_literal_integer(m_reader.advance().text);
   
    if (m_reader.consume(TokenType::Plus)) {
        return parse_binary(
                std::move(literal_integer),
                BinaryOperation::Plus);
    }
    else if (m_reader.consume(TokenType::Minus)) {
        return parse_binary(
                std::move(literal_integer),
                BinaryOperation::Minus);
    }
    else if (m_reader.consume(TokenType::Star)) {
        return parse_binary(
                std::move(literal_integer),
                BinaryOperation::Multiply);
    } 
    else if (m_reader.consume(TokenType::Slash)) {
        return parse_binary(
                std::move(literal_integer),
                BinaryOperation::Divide);
    }

    return literal_integer;
}

AST::Ptr Parser::parse_literal_string() {
    OUTPUT_LOCATION;
     
    return nullptr;
}

AST::Ptr Parser::parse_identifier() {
    OUTPUT_LOCATION;

    return nullptr;
}

AST::Ptr Parser::parse_binary(AST::Ptr&& left, const BinaryOperation op) {
    OUTPUT_LOCATION;

    return nullptr;
}

AST::Ptr Parser::parse_bounded_pointer() {
    OUTPUT_LOCATION; 
    
    return nullptr;
}
#pragma endregion

#pragma region Statements

AST::Ptr Parser::parse_if() {
    OUTPUT_LOCATION;

    AST::If if_statement;
    if_statement.condition = parse_expression();
    
    if (if_statement.condition == nullptr) {
        expected_error("condition for if");
    }

    if (m_reader.consume(TokenType::Then)) {
        if_statement.then_true = parse_statement();
    }
    else if (m_reader.consume(TokenType::LeftBracket)) {
        if_statement.then_true = parse_block();
    }
    else {
        // Unexpected
        expected_error("if body as { or then");
    }
 
    if (m_reader.consume(TokenType::Elif)) {
        if_statement.then_false = parse_if();
   }
    else if (m_reader.consume(TokenType::Else)) {  
        if_statement.then_false = parse_statement();
   }

    return make_ast(std::move(if_statement));
}

AST::Ptr Parser::parse_return() {
    OUTPUT_LOCATION;

    AST::Return ret;
    ret.expr = parse_expression();
    return make_ast(std::move(ret));
}

AST::Ptr Parser::parse_const_declaration() {
    OUTPUT_LOCATION;

    AST::Variable const_decl{};

    if (!m_reader.is(TokenType::Identifier)) {
        expected_error("name for declared constant");
    }
    
    if (!m_reader.at_end()) {
        const_decl.name = m_reader.advance().text;
    }

    if (m_reader.consume(TokenType::Colon)) {
        const_decl.type = parse_type();    
    }

    if (!m_reader.consume(TokenType::Equal)) {
        expected_error("= after constant declaration");
    }

    const_decl.initialization = parse_expression();

    return make_ast(std::move(const_decl));
}

AST::Ptr Parser::parse_var_declaration() {
    OUTPUT_LOCATION;
    
    return nullptr;
}

#pragma endregion

AST::Ptr Parser::make_literal_integer(const std::string_view text) {
    OUTPUT_LOCATION;

    std::stringstream ss;
    ss << text;
    
    AST::LiteralInteger integer;
    ss >> integer.value; 

    return make_ast(std::move(integer));
}

#pragma region Errors

bool Parser::failed() const {
    return m_failed;
}

void Parser::expected_error(
        const std::string& what,
        const std::optional<Suggestion> suggestion
) {
    const auto& current_token = m_reader.current();
    m_errors.push_back({
            .message = std::format("expected {} (got {})", what, current_token.text),
            .location = current_token.location,
            .suggestion = suggestion
            });
    m_failed = true;
}

bool Parser::has_syntax_errors() const {
    return !m_errors.empty();
}

std::span<const SyntaxError> Parser::syntax_errors() const {
    return m_errors;
}

#pragma endregion
