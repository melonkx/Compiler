#include "ModuleParser.h"

ModuleParser::ModuleParser(TokenReader& reader_, DiagnosticsEngine& diagnostics_) 
    : Parser(reader_, diagnostics_),
    m_function_parser(reader_, diagnostics_)
{
    set_rule(TokenType::Object,     &ModuleParser::parse_object,    true);
    set_rule(TokenType::Function,   &ModuleParser::parse_function,  true);
}

AST::Ptr ModuleParser::parse() {
    return nullptr;
}

AST::Ptr ModuleParser::parse_object() {
    return nullptr;
}
