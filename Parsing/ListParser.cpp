#include "ListParser.h"

ListParser::ListParser(TokenReader& reader_, DiagnosticsEngine& diagnostics_) :
    Parser(reader_, diagnostics_)
{}

AST::Ptr ListParser::parse() {
    if (m_element_parser == nullptr) {
        throw std::runtime_error("ListParser::m_element_parser == nullptr");
    }
    
    if (!reader().consume(m_opening)) {
        auto error = make_expected("opening delimiter in list");
        // report.help = (source code with the delimiter included)
        diagnostics().report(error);
        return nullptr;
    }

    bool found_closing_delimiter = false;
    AST::List list;
    
    while (!reader().at_end()) {
        if (reader().consume(m_closing)) {
            found_closing_delimiter = true;
            break;
        }

        auto element = m_element_parser->parse();
        
        if (element) {
            list.elements.emplace_back(std::move(element));
        }

        if (reader().is(m_closing)) continue;

        if (!reader().consume(TokenType::Comma)) {
            auto error = make_expected("comma");
            diagnostics().report(error);
        }
    }

    if (!found_closing_delimiter) {
        // unexpected end of file
        //auto report = ...
        diagnostics().report({.code = "E002", .message = "unexpected end of file"});
        return nullptr;
    }

    return make_ast(std::move(list));
}

void ListParser::set_delimiters(TokenType opening, TokenType closing) {
    m_opening = opening;
    m_closing = closing;
}

TokenType ListParser::opening() const {
    return m_opening;
}

void ListParser::set_element_parser(Parser* parser) { 
    if (parser == nullptr) {
        throw std::invalid_argument("in ListParser::set_element_parser(), parser == nullptr");
    } 
    m_element_parser = parser;
}
