#include "TypeParser.h"
#include "ExpressionParser.h"
#include <iostream>

TypeParser::TypeParser(TokenReader& reader_, DiagnosticsEngine& diagnostics_)
    : Parser(reader_, diagnostics_)
{}

AST::Ptr TypeParser::parse() {
    // TODO: debug assert!!!!
    // in production, this code will never execute.
    if (m_expression_parser == nullptr) {
        throw std::runtime_error("TypeParser::m_expression_parser == nullptr");
    } 

    AST::Type type;
    
    if (reader().is(TokenType::Identifier)) {
        type.name = reader().advance().text;
    } else {
        diagnostics().report(make_expected("type name"));
    }

    auto type_ast = make_ast(std::move(type));
    
    // May be pointer to a pointer, in that case, we need to loop the type checking
    while (!reader().at_end()) {
        if (reader().consume(TokenType::Star)) {
            type_ast = parse_pointer(std::move(type_ast));
        } else {
            break;
        }
    }

    return type_ast;
}

void TypeParser::set_expression_parser(ExpressionParser* parser) {
    m_expression_parser = parser;
}

AST::Ptr TypeParser::parse_pointer(AST::Ptr&& base_type) {
    auto ptr_type = make_ast(AST::PointerType{.base_type = std::move(base_type)});

    if (reader().consume(TokenType::LeftSquareBracket)) {
        return parse_bounded_pointer(std::move(ptr_type));
    }

    return std::move(ptr_type);
}

AST::Ptr TypeParser::parse_bounded_pointer(AST::Ptr&& base_type) {
    using BoundedPointer = AST::BoundedPointerType;
    using UnknownExtent = BoundedPointer::UnknownExtent;
    using FixedExtent = BoundedPointer::FixedExtent;

    BoundedPointer bounded_pointer {.base_type = std::move(base_type)};
    bool is_extent_set = false;

    if (reader().consume(TokenType::TripleDot)) {
        bounded_pointer.extent = UnknownExtent{};
        is_extent_set = true;
    } else {
        auto length = m_expression_parser->parse();
        
        if (length != nullptr) {
            bounded_pointer.extent = FixedExtent {
                .length = std::move(length)
            };
            is_extent_set = true;
        }
    }

    if (!is_extent_set) {
        diagnostics().report(make_expected("bounded pointer extent"));
    }

    if (!reader().consume(TokenType::RightSquareBracket)) {
        diagnostics().report(make_expected("] to close bounded pointer"));
    }

    return make_ast(bounded_pointer);
}
