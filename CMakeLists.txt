set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
    Parsing/ModuleParser.cpp
    Parsing/StatementParser.cpp
    Parsing/ExpressionParser.cpp
    Parsing/TypeParser.cpp
    Parsing/FunctionParser.cpp
    Parsing/ParameterParser.cpp
    Parsing/ListParser.cpp
)
