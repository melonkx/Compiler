#include "AST.h"

#include <iostream>

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <variant>
#include <type_traits>
#include <cstddef>

namespace {
    void Indent(std::size_t indent)
    {
        std::cout << std::string(indent, ' ');
    }

    void PrintChild(
        const char* name,
        const AST::Ptr& child,
        std::size_t indent)
    {
        Indent(indent);
        std::cout << name << ":\n";
        Print(child, indent + 2);
    }

    template <typename T>
    void PrintValue(const T& value, std::size_t indent)
    {
        using U = std::decay_t<T>;

        // Recursively unwrap nested variants.
        if constexpr (
            std::is_same_v<U, AST::Top> ||
            std::is_same_v<U, AST::Statement> ||
            std::is_same_v<U, AST::Expression> ||
            std::is_same_v<U, decltype(AST{}.data)>)
        {
            std::visit(
                [indent](const auto& node)
                {
                    PrintValue(node, indent);
                },
                value);
        }
        else if constexpr (std::is_same_v<U, AST::Module>)
        {
            Indent(indent);
            std::cout << "Module\n";

            for (const auto& function : value.functions)
                Print(function, indent + 2);
        }
        else if constexpr (std::is_same_v<U, AST::Function>)
        {
            Indent(indent);
            std::cout << "Function: " << value.name << '\n';

            PrintChild("Parameters", value.parameters, indent + 2);
            PrintChild("Body", value.body, indent + 2);
        }
        else if constexpr (std::is_same_v<U, AST::FunctionParameter>)
        {
            Indent(indent);
            std::cout << "FunctionParameter: " << value.name << '\n';

            PrintChild("Type", value.type, indent + 2);
        }
        else if constexpr (std::is_same_v<U, AST::List>)
        {
            Indent(indent);
            std::cout << "List\n";

            for (const auto& element : value.elements)
                Print(element, indent + 2);
        }
        else if constexpr (std::is_same_v<U, AST::FunctionParameters>)
        {
            Indent(indent);
            std::cout << "FunctionParameters\n";

            PrintChild("List", value.list, indent + 2);
        }
        else if constexpr (std::is_same_v<U, AST::Type>)
        {
            Indent(indent);
            std::cout << "Type: " << value.name << '\n';
        }
        else if constexpr (std::is_same_v<U, AST::PointerType>)
        {
            Indent(indent);
            std::cout << "PointerType\n";

            PrintChild("BaseType", value.base_type, indent + 2);
        }
        else if constexpr (std::is_same_v<U, AST::BoundedPointerType>)
        {
            Indent(indent);
            std::cout << "BoundedPointerType\n";

            PrintChild("BaseType", value.base_type, indent + 2);

            Indent(indent + 2);
            std::cout << "Extent:\n";

            std::visit(
                [indent](const auto& extent)
                {
                    using E = std::decay_t<decltype(extent)>;

                    if constexpr (
                        std::is_same_v<
                            E, AST::BoundedPointerType::UnknownExtent>)
                    {
                        Indent(indent + 4);
                        std::cout << "UnknownExtent\n";
                    }
                    else
                    {
                        Indent(indent + 4);
                        std::cout << "FixedExtent\n";

                        PrintChild(
                            "Length", extent.length, indent + 6);
                    }
                },
                value.extent);
        }
        else if constexpr (std::is_same_v<U, AST::Variable>)
        {
            Indent(indent);
            std::cout << "Variable: " << value.name
                      << (value.is_const ? " (const)" : "") << '\n';

            PrintChild("Type", value.type, indent + 2);
            PrintChild("Initialization", value.initialization, indent + 2);
        }
        else if constexpr (std::is_same_v<U, AST::Block>)
        {
            Indent(indent);
            std::cout << "Block\n";

            for (const auto& statement : value.statements)
                Print(statement, indent + 2);
        }
        else if constexpr (std::is_same_v<U, AST::If>)
        {
            Indent(indent);
            std::cout << "If\n";

            PrintChild("Condition", value.condition, indent + 2);
            PrintChild("Then", value.then_true, indent + 2);
            PrintChild("Else", value.then_false, indent + 2);
        }
        else if constexpr (std::is_same_v<U, AST::Return>)
        {
            Indent(indent);
            std::cout << "Return\n";

            PrintChild("Expression", value.expression, indent + 2);
        }
        else if constexpr (std::is_same_v<U, AST::Unary>)
        {
            Indent(indent);
            std::cout << "Unary (type "
                      << static_cast<int>(value.type) << ")\n";

            PrintChild("Destination", value.destination, indent + 2);
            PrintChild("Source", value.source, indent + 2);
        }
        else if constexpr (std::is_same_v<U, AST::Binary>)
        {
            Indent(indent);
            std::cout << "Binary (type "
                      << static_cast<int>(value.type) << ")\n";

            PrintChild("Destination", value.destination, indent + 2);
            PrintChild("Left", value.left, indent + 2);
            PrintChild("Right", value.right, indent + 2);
        }
        else if constexpr (std::is_same_v<U, AST::Identifier>)
        {
            Indent(indent);
            std::cout << "Identifier: " << value.name << '\n';
        }
        else if constexpr (std::is_same_v<U, AST::LiteralInteger>)
        {
            Indent(indent);
            std::cout << "LiteralInteger: " << value.value << '\n';
        }
        else if constexpr (std::is_same_v<U, AST::LiteralString>)
        {
            Indent(indent);
            std::cout << "LiteralString: \"" << value.text << "\"\n";
        }
        else if constexpr (std::is_same_v<U, AST::Address>)
        {
            Indent(indent);
            std::cout << "Address\n";

            PrintChild("Value", value.value, indent + 2);
        }
        else if constexpr (std::is_same_v<U, AST::BoundedAddress>)
        {
            Indent(indent);
            std::cout << "BoundedAddress\n";

            PrintChild("Value", value.value, indent + 2);
            PrintChild("Min", value.min, indent + 2);
            PrintChild("Max", value.max, indent + 2);
        }
    }
}

void Print(const AST::Ptr& ptr, std::size_t indent)
{
    if (!ptr)
    {
        Indent(indent);
        std::cout << "(null)\n";
        return;
    }

    PrintValue(ptr->data, indent);
}

