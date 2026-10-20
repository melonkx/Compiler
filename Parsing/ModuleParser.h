#pragma once
#include "FunctionParser.h"

class ModuleParser : public Parser {
public:
    ModuleParser(TokenReader& reader_, DiagnosticsEngine& diagnostics_);    
    AST::Ptr parse() override;
private:
    AST::Ptr parse_object();
    AST::Ptr parse_function();
private:
    FunctionParser m_function_parser;
};
