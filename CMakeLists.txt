
project(Compiler)

add_executable(Compiler
    main.cpp
    SourceFile.cpp
    Scanner.cpp
    Tokenizer.cpp
    TokenReader.cpp
    AST.cpp
    Parser.cpp)
