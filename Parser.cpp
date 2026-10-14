#include "Parser.h"

#include <iostream>
#include <sstream>
#include <source_location>


#define OUTPUT_LOCATION  std::cout << std::source_location::current().function_name() << std::endl  

Parser::Parser(const std::span<const Token> tokens) : 
    reader(tokens) {
}

AST::Ptr Parser::parse() {
    OUTPUT_LOCATION;

    AST::Module mod;

    while (!reader.at_end()) {
        if (reader.consume(TokenType::Function)) {
            auto func = parse_function();
            
            if (!func) {
                return nullptr;
            }

            mod.functions.emplace_back(std::move(func));
        }
        else {
            // Throw an error
            return nullptr;
        }
    }
    
    return std::make_unique<AST>(std::move(mod));
}

AST::Ptr Parser::parse_expression() {
    OUTPUT_LOCATION;

    if (reader.is(TokenType::LiteralInteger)) {
        return parse_literal_integer();
    }

    if (reader.is(TokenType::LiteralString)) {
        return parse_literal_string();
    }

    if (reader.is(TokenType::Identifier)) {
        return parse_identifier();
    }

    // Unexpected
    // unexpected("...");
    return nullptr;
}

AST::Ptr Parser::parse_statement() { 
    OUTPUT_LOCATION;

    if (reader.consume(TokenType::LeftBracket)) {
        return parse_block();
    }

    if (reader.consume(TokenType::If)) {
        return parse_if();
    }

    if (reader.consume(TokenType::Const)) {
        return parse_const_declaration();
    }

    if (reader.consume(TokenType::Var)) {
        return parse_var_declaration();
    }
    
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
    if (!reader.is(TokenType::Identifier)) {
        // Expected identifier
        //expected("expected...");
        return nullptr;
    }

    function.name = reader.advance().text;
 
    // Read parameters
    if (reader.consume(TokenType::LeftParen)) {
        function.parameters = parse_function_parameters();

        if (has_syntax_errors()) {
            // Exit
            return nullptr;
        }
    }

    // Read body
    if (reader.consume(TokenType::FunctionArrow)) {
        function.body = parse_expression();
    }
    else if (reader.consume(TokenType::LeftBracket)) {
        function.body = parse_block();
    }
    else {
        // Unexpected
        return nullptr;
    }

    if (!function.body) {
        return nullptr;
    }
    
    return std::make_unique<AST>(std::move(function));
}

AST::Ptr Parser::parse_function_parameters() {
    OUTPUT_LOCATION;
    
    AST::FunctionParameters function_parameters;
    bool found_closing_paren = false;

    while (!reader.at_end()) {
        if (reader.is(TokenType::Identifier)) {
            auto parameter = parse_function_parameter();

            if (!parameter) {
                return nullptr;
            }
            
            function_parameters.parameters.emplace_back(std::move(parameter));

            if (!reader.consume(TokenType::Comma)) {
                if (!reader.is(TokenType::RightParen)) {
                    unexpected_error();
                    return nullptr;
                }
            }
        }
        else if (reader.is(TokenType::RightParen)) {
            reader.advance();
            found_closing_paren = true;
            break;
        } else {
            // Unexpected
            unexpected_error();
            return nullptr;
        }

    }

    if (!found_closing_paren) {
        return nullptr;
    }
    
    return std::make_unique<AST>(std::move(function_parameters));
}

AST::Ptr Parser::parse_function_parameter() {
    OUTPUT_LOCATION;

    AST::FunctionParameter parameter;

    if (reader.is(TokenType::Identifier)) {
        parameter.name = reader.advance().text;
    }
    else {
        expected_error("parameter name");
        return nullptr;
    }

    if (!reader.consume(TokenType::Colon)) {
        // Expected :
        // Error: expected : after 
        expected_error(": after parameter name");
        return nullptr;
    }

    parameter.type = parse_type();

    if (!parameter.type) {
        return nullptr;
    }

    return std::make_unique<AST>(std::move(parameter));
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

    if (reader.is(TokenType::Identifier)) {
        type.name = reader.advance().text;    
    }
    else { 
        expected_error("type name");
        return nullptr;
    }

    auto ptr = std::make_unique<AST>(std::move(type));

    if (reader.consume(TokenType::Star)) {
        return parse_pointer_type(std::move(ptr));
    }

    return std::move(ptr);
}

AST::Ptr Parser::parse_pointer_type(AST::Ptr&& type) {
    OUTPUT_LOCATION;
    if (reader.consume(TokenType::LeftSquareBracket)) {
        return parse_bounded_pointer_type(std::move(type));
    }

    AST::PointerType pointer_type;
    pointer_type.base_type = std::move(type);

    return std::make_unique<AST>(std::move(pointer_type));
}

AST::Ptr Parser::parse_bounded_pointer_type(AST::Ptr&& type) {
    OUTPUT_LOCATION;

    AST::BoundedPointerType bounded_pointer_type;
    bounded_pointer_type.base_type = std::move(type);

    // Type* [...]
    if (reader.consume(TokenType::TripleDot)) {
        if (!reader.consume(TokenType::RightSquareBracket)) {
            expected_error("closing ]");
            return nullptr;
        }

        return std::make_unique<AST>(
            std::move(bounded_pointer_type)
        );
    }

    auto first_constraint = parse_expression();
    if (!first_constraint) {
        return nullptr;
    }

    // Type* [Min...] or Type* [Min...Max]
    if (reader.consume(TokenType::TripleDot)) {
        bounded_pointer_type.minimum = std::move(first_constraint);

        // Type* [Min...]
        if (reader.consume(TokenType::RightSquareBracket)) {
            return std::make_unique<AST>(
                std::move(bounded_pointer_type)
            );
        }

        // Type* [Min...Max]
        auto second_constraint = parse_expression();
        if (!second_constraint) {
            return nullptr;
        }

        if (!reader.consume(TokenType::RightSquareBracket)) {
            return nullptr;
        }

        bounded_pointer_type.maximum = std::move(second_constraint);
    }
    else {
        // Type* [Max]
        if (!reader.consume(TokenType::RightSquareBracket)) {
            return nullptr;
        }

        bounded_pointer_type.maximum = std::move(first_constraint);
    }

    return std::make_unique<AST>(std::move(bounded_pointer_type));
}



AST::Ptr Parser::parse_block() {
    OUTPUT_LOCATION;
    
    AST::Block block;
    bool found_closing_brace = false;

    // Read until we hit a } or there's a syntax error
    while (!reader.at_end()) {
        if (reader.is(TokenType::RightBracket)) {
            reader.advance();
            found_closing_brace = true;
            break;
        }

        auto statement = parse_statement();
        
        if (statement) {
            block.statements.emplace_back(std::move(statement));
        }
        else {
            break;
        }
    }
    
    // Error:
    if (!found_closing_brace) {
        return nullptr;
    }

    return std::make_unique<AST>(std::move(block));
}
#pragma region Expressions

AST::Ptr Parser::parse_literal_integer() {
    OUTPUT_LOCATION;

    AST::Ptr literal_integer = make_literal_integer(reader.advance().text);
   
    if (reader.consume(TokenType::Plus)) {
        return parse_binary(
                std::move(literal_integer),
                BinaryOperation::Plus);
    }
    else if (reader.consume(TokenType::Minus)) {
        return parse_binary(
                std::move(literal_integer),
                BinaryOperation::Minus);
    }
    else if (reader.consume(TokenType::Star)) {
        return parse_binary(
                std::move(literal_integer),
                BinaryOperation::Multiply);
    } 
    else if (reader.consume(TokenType::Slash)) {
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
        return nullptr;
    }

    if (reader.consume(TokenType::Then)) {
        if_statement.then_true = parse_statement();
    }
    else if (reader.consume(TokenType::LeftBracket)) {
        if_statement.then_true = parse_block();
    }
    else {
        // Unexpected
        return nullptr;
    }

    // There was a syntax error parsing the true branch
    if (!if_statement.then_true) {
        return nullptr;
    }
    
    if (reader.consume(TokenType::Elif)) {
        if_statement.then_false = parse_if();
        
        // Syntax error parsing false branch
        if (!if_statement.then_false) {
            return nullptr;
        }
    }
    else if (reader.consume(TokenType::Else)) {  
        if_statement.then_false = parse_statement();
       
        // Syntax error parsing false branch
        if (!if_statement.then_false) {
            return nullptr;
        }
    }

    return std::make_unique<AST>(std::move(if_statement));
}

AST::Ptr Parser::parse_const_declaration() {
    OUTPUT_LOCATION;

    return nullptr;
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

    return std::make_unique<AST>(std::move(integer));
}

#pragma region Errors

void Parser::unexpected_error() {

}

void Parser::expected_error(const std::string& what) {
    
}

bool Parser::has_syntax_errors() const {
    return !errors.empty();
}

#pragma endregion
