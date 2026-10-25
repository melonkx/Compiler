#pragma once
#include "Parser.h"

class TypeParser;

class ParameterParser : public Parser {
public:
    ParameterParser(TokenReader& reader_, DiagnosticsEngine& diagnostics_);
    AST::Ptr parse() override;
    void set_type_parser(TypeParser* parser);
private:
    TypeParser* m_type_parser;
};
