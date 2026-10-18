#pragma once

#include "TokenReader.h"
#include "AST.h"
#include "SyntaxError.h"

#include <span>

class Parser {
public:
    explicit Parser(std::span<const Token> tokens);
    AST::Ptr parse();
    bool has_syntax_errors() const;
    std::span<const SyntaxError> syntax_errors() const;
    
private:
    enum class BinaryOperation {
        Plus,
        Minus,
        Multiply,
        Divide
    };

private:
#pragma region Top Level
    AST::Ptr parse_function();
    AST::Ptr parse_expression();
    AST::Ptr parse_statement();
#pragma endregion

#pragma region Functions..
    AST::Ptr parse_function_parameters();
    AST::Ptr parse_function_parameter();
#pragma endregion

#pragma region Types
    AST::Ptr parse_type();
    AST::Ptr parse_pointer_type(AST::Ptr&& type);
    AST::Ptr parse_bounded_pointer_type(AST::Ptr&& type);
#pragma endregion
    
#pragma region Expressions
    AST::Ptr parse_literal_integer();
    AST::Ptr parse_literal_string();
    AST::Ptr parse_identifier();
    AST::Ptr parse_binary(AST::Ptr&& left, BinaryOperation op);
    AST::Ptr parse_bounded_pointer();
#pragma endregion

#pragma region Statements
    AST::Ptr parse_block();
    AST::Ptr parse_if();
    AST::Ptr parse_return();
#pragma endregion

private:
    AST::Ptr make_literal_integer(std::string_view text);

private:
#pragma region Errors
    bool failed() const;

    void expected_error(
        const std::string& what,
        std::optional<Suggestion> suggestion = std::nullopt
    );
#pragma endregion

private:
    template<typename T>
    AST::Ptr make_ast(T&& v) {
        return failed() ? nullptr : std::make_unique<AST>(std::move(v));
    }

private:
    TokenReader m_reader;
    std::vector<SyntaxError> m_errors;
    bool m_failed{};
};
