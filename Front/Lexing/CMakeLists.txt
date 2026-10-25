add_library(Lexing
    Scanner.cpp
    SourceFile.cpp
    Tokenizer.cpp
    TokenReader.cpp
)

target_include_directories(Lexing
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
