#pragma once
#include "Parser.h"

class ExpressionParser : public Parser {
public:
    ExpressionParser(TokenReader& reader_, DiagnosticsEngine& diagnostics_);
    AST::Ptr parse() override;
private:
    AST::Ptr parse_identifier();
    AST::Ptr parse_literal_integer();
    AST::Ptr parse_literal_string();
};
