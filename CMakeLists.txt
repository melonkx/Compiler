set(CMAKE_CXX_STANDARD 23)

project(Compiler)

add_executable(Compiler
    main.cpp
    SourceFile.cpp
    Scanner.cpp
    Tokenizer.cpp
    TokenReader.cpp

    # Parsing
    Parsing/AST.cpp
    Parsing/DiagnosticsEngine.cpp
    Parsing/ProductionRuleSelector.cpp
    Parsing/Parser.cpp
    Parsing/LiteralParser.cpp
    Parsing/StatementParser.cpp
    Parsing/ExpressionParser.cpp 
)
