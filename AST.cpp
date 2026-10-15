#include "AST.h"

#include <iostream>

// Written by AI for temporary purposes.
// (For debugging, I do not care if one-time helper functions are written by AI)
// This compiler is otherwise free of slop
void Print(const AST::Ptr& node, std::size_t indent = 0)
{
    const auto pad = [indent]() {
        return std::string(indent * 2, ' ');
    };

    if (!node)
    {
        std::cout << pad() << "<null>\n";
        return;
    }

    const auto print_child = [](const AST::Ptr& child, std::size_t level) {
        Print(child, level);
    };

    std::visit([&](const auto& category)
    {
        using Category = std::decay_t<decltype(category)>;

        std::visit([&](const auto& value)
        {
            using T = std::decay_t<decltype(value)>;

            // Top-level nodes
            if constexpr (std::is_same_v<T, AST::Module>)
            {
                std::cout << pad() << "Module\n";
                for (const auto& function : value.functions)
                    print_child(function, indent + 1);
            }
            else if constexpr (std::is_same_v<T, AST::Function>)
            {
                std::cout << pad() << "Function: " << value.name << '\n';
                std::cout << pad() << "  Parameters:\n";
                print_child(value.parameters, indent + 2);
                std::cout << pad() << "  Body:\n";
                print_child(value.body, indent + 2);
            }
            else if constexpr (std::is_same_v<T, AST::FunctionParameters>)
            {
                std::cout << pad() << "FunctionParameters\n";
                for (const auto& parameter : value.parameters)
                    print_child(parameter, indent + 1);
            }
            else if constexpr (std::is_same_v<T, AST::FunctionParameter>)
            {
                std::cout << pad() << "Parameter: " << value.name << '\n';
                print_child(value.type, indent + 1);
            }
            else if constexpr (std::is_same_v<T, AST::Type>)
            {
                std::cout << pad() << "Type: " << value.name << '\n';
            }
            else if constexpr (std::is_same_v<T, AST::BoundedPointerType>)
            {
                std::cout << pad() << "BoundedPointerType\n";
                std::cout << pad() << "  Base type:\n";
                print_child(value.base_type, indent + 2);
                std::cout << pad() << "  Minimum:\n";
                print_child(value.minimum, indent + 2);
                std::cout << pad() << "  Maximum:\n";
                print_child(value.maximum, indent + 2);
            }
            else if constexpr (std::is_same_v<T, AST::PointerType>) {
                std::cout << pad() << "PointerType\n";
                std::cout << pad() << "  Base type:\n";
                print_child(value.base_type, indent + 2);
            }

            // Statements
            else if constexpr (std::is_same_v<T, AST::Variable>)
            {
                std::cout << pad() << "Variable: " << value.name << '\n';
                std::cout << pad() << "  Type:\n";
                print_child(value.type, indent + 2);
                std::cout << pad() << "  Initialization:\n";
                print_child(value.initialization, indent + 2);
            }
            else if constexpr (std::is_same_v<T, AST::Block>)
            {
                std::cout << pad() << "Block\n";
                for (const auto& statement : value.statements)
                    print_child(statement, indent + 1);
            }
            else if constexpr (std::is_same_v<T, AST::If>)
            {
                std::cout << pad() << "If\n";
                std::cout << pad() << "  Condition:\n";
                print_child(value.condition, indent + 2);
                std::cout << pad() << "  Then:\n";
                print_child(value.then_true, indent + 2);
                std::cout << pad() << "  Else:\n";
                print_child(value.then_false, indent + 2);
            }

            // Expressions
            else if constexpr (std::is_same_v<T, AST::Unary>)
            {
                std::cout << pad() << "Unary\n";
                std::cout << pad() << "  Destination:\n";
                print_child(value.destination, indent + 2);
                std::cout << pad() << "  Source:\n";
                print_child(value.source, indent + 2);
            }
            else if constexpr (std::is_same_v<T, AST::Binary>)
            {
                std::cout << pad() << "Binary\n";
                std::cout << pad() << "  Destination:\n";
                print_child(value.destination, indent + 2);
                std::cout << pad() << "  Left:\n";
                print_child(value.left, indent + 2);
                std::cout << pad() << "  Right:\n";
                print_child(value.right, indent + 2);
            }
            else if constexpr (std::is_same_v<T, AST::LiteralInteger>)
            {
                std::cout << pad() << "Integer: " << value.value << '\n';
            }
            else if constexpr (std::is_same_v<T, AST::Address>)
            {
                std::cout << pad() << "Address\n";
                print_child(value.value, indent + 1);
            }
            else if constexpr (std::is_same_v<T, AST::BoundedAddress>)
            {
                std::cout << pad() << "BoundedAddress\n";
                std::cout << pad() << "  Value:\n";
                print_child(value.value, indent + 2);
                std::cout << pad() << "  Minimum:\n";
                print_child(value.min, indent + 2);
                std::cout << pad() << "  Maximum:\n";
                print_child(value.max, indent + 2);
            }

        }, category);
    }, node->data);
}
