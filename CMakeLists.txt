set(CMAKE_CXX_STANDARD 23)

project(Compiler)

add_executable(Compiler
    main.cpp
    
    # TODO: Tokenizing /
    SourceFile.cpp
    Scanner.cpp
    Tokenizer.cpp
    TokenReader.cpp

    # Parsing /
    Parsing/AST.cpp
    Parsing/DiagnosticsEngine.cpp
    Parsing/ProductionRuleSelector.cpp

    # ... Parsers 
    Parsing/Parser.cpp
    Parsing/StatementParser.cpp
    Parsing/ExpressionParser.cpp 
    Parsing/ListParser.cpp 
)
