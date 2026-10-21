#pragma once
#include <variant>
#include <vector>
#include <string>
#include <memory>

struct AST {
    using Ptr = std::unique_ptr<AST>;

#pragma region Top
    struct Module {
        std::vector<Ptr> functions;
    };

    struct Function {
        std::string name;
        Ptr parameters;
        Ptr body;
    };

    struct FunctionParameter {
        std::string name;
        Ptr type;
    };

    struct List { std::vector<Ptr> elements; };

    struct FunctionParameters { Ptr list; };

    struct Type { std::string name; };

#pragma region Pointers

    struct PointerType { Ptr base_type; };

    struct BoundedPointerType {
        struct UnknownExtent {};
        struct FixedExtent {
            Ptr length;
        };

        Ptr base_type;
        std::variant<UnknownExtent, FixedExtent> extent;
    };

#pragma endregion

#pragma endregion

#pragma region Statements
    struct Variable {
        bool is_const{};
        std::string name;
        Ptr type;
        Ptr initialization;
    };

    struct Block {
        std::vector<Ptr> statements;
    };

    struct If {
        Ptr condition;
        Ptr then_true;
        Ptr then_false;
    };

    struct Return {
        Ptr expression;
    };

#pragma endregion

#pragma region Expressions    
     struct Unary {
        enum class Type {};
        Type type;
        Ptr destination;
        Ptr source;
    };

    struct Binary {
        enum class Type {};
        Type type;
        Ptr destination;
        Ptr left;
        Ptr right;
    };

    struct Identifier { std::string name; };
    struct LiteralInteger { long long value; };
    struct LiteralString { std::string text; };
    struct AddressOf { Ptr value; };
    struct AddressFrom { Ptr value; };

#pragma endregion
   
    using Top = std::variant<
        Module,
        Function, List, FunctionParameter,
        Type, PointerType, BoundedPointerType
    >;
    
    using Statement = std::variant<
        Variable,
        Block,
        If,
        Return
    >;
    
    using Expression = std::variant<
        Unary, Binary,
        Identifier, LiteralInteger, LiteralString,
        AddressOf, AddressFrom>;

    std::variant<Top, Statement, Expression, List> data;
};


// Prints an AST to stdout
void Print(const AST::Ptr& ptr, std::size_t indent);  

