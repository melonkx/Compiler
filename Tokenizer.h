#pragma once

#include <vector>

#include "Token.h"
#include "Scanner.h"

class Tokenizer {
public:
    explicit Tokenizer();
    void set_source(SourceFile* source);
    std::vector<Token> tokenize();
private:
    static bool in_identifier(char ch, std::size_t index);
    static bool in_dollar_identifier(char ch, std::size_t index);
    static bool in_number(char ch, std::size_t index);
    static bool is_symbol(char ch);

private:
    bool scan_symbol();
    void push_token(TokenType type);

private:
    SourceFile* source_file;
    std::vector<Token> tokens;
    
    std::unordered_map<std::string_view, TokenType> keywords;
    std::unordered_map<std::string_view, TokenType> symbols;
};
