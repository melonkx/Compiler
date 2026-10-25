add_library(Parsing
    AST.cpp

    # Parsers
    Parser.cpp
    ModuleParser.cpp
    FunctionParser.cpp
    StatementParser.cpp
    ExpressionParser.cpp
    ListParser.cpp
    ParameterParser.cpp
    TypeParser.cpp
    DiagnosticsEngine.cpp
    ProductionRuleSelector.cpp
)

target_include_directories(Parsing
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Parsing
    PUBLIC
        Lexing
)
