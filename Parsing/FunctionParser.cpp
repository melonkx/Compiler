#include "FunctionParser.h"

FunctionParser::FunctionParser(TokenReader& reader_, DiagnosticsEngine& diagnostics_)
    : Parser(reader_, diagnostics_),
    m_type_parser(reader_, diagnostics_),
    m_statement_parser(reader_, diagnostics_),
    m_expression_parser(reader_, diagnostics_),
    m_parameter_parser(reader_, diagnostics_),
    m_parameter_list_parser(reader_, diagnostics_)
{
    m_statement_parser.set_expression_parser(&m_expression_parser);

    // Initialize parameter parser
    m_parameter_parser.set_type_parser(&m_type_parser);

    // Initialize parameter list
    m_parameter_list_parser.set_element_parser(&m_parameter_parser);
    m_parameter_list_parser.set_delimiters(TokenType::LeftParen, TokenType::RightParen);

}

AST::Ptr FunctionParser::parse() {
    AST::Function function;
    
    if (reader().is(TokenType::Identifier)) {
        function.name = reader().advance().text;
    } else {
        diagnostics().report(make_expected("function name"));
    }
    
    function.parameters = m_parameter_list_parser.parse();
    
    if (reader().is(TokenType::LeftBracket)) {
        function.body = m_statement_parser.parse();
    }
    else if (reader().is(TokenType::FunctionArrow)) {
        function.body = m_expression_parser.parse();
    }
    else {
        // TODO: Help messages
        // did you mean: function FunctionName (Parameters) (=> expression or {})
        auto error = make_expected("function block or expression");
        diagnostics().report(error);
    }

    return make_ast(std::move(function));
}

