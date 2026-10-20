#include "ModuleParser.h"

ModuleParser::ModuleParser(TokenReader& reader_, DiagnosticsEngine& diagnostics_) 
    : Parser(reader_, diagnostics_),
    m_function_parser(reader_, diagnostics_)
{}

AST::Ptr ModuleParser::parse() {
    AST::Module mod;

    while (!reader().at_end()) {
        if (reader().consume(TokenType::Function)) {
            auto function = parse_function();
            if (!function) continue;
            mod.functions.emplace_back(std::move(function));    
        }
        else if (reader().consume(TokenType::Object)) {
            // TODO: object parsing    
        } else {
            diagnostics().report(make_expected("'function' or 'object' at top-level"));
            break;
        }
    }

    return make_ast(std::move(mod));
}

AST::Ptr ModuleParser::parse_function() {
    return m_function_parser.parse();
}

AST::Ptr ModuleParser::parse_object() {
    return nullptr;
}
