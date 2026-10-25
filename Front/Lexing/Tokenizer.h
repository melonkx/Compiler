#pragma once

#include <vector>
#include <span>
#include <unordered_map>
#include <unordered_set>


#include "SourceFile.h"
#include "Token.h"
#include "Scanner.h"

class Tokenizer {
public:
    [[nodiscard]] std::vector<Token> tokenize(const SourceFile& source_file);
    void load_keywords(std::span<const std::pair<std::string_view, TokenType>> keywords);
    void load_symbols(std::span<const std::pair<std::string_view, TokenType>> symbols);

private:
    bool in_identifier(char ch, std::size_t index);
    bool in_dollar_identifier(char ch, std::size_t index);
    bool in_number(char ch, std::size_t index);
    bool is_symbolic_char(char ch);

private:
    void scan_symbol();
    void push_token(TokenType type);
    void load_symbol(std::string_view sym, TokenType type);

private:
    std::vector<Token> m_tokens;
    Scanner* m_scanner;
    const SourceFile* m_source_file;
    std::unordered_map<std::string_view, TokenType> m_keywords;
    std::unordered_map<std::string_view, TokenType> m_symbols;
    std::unordered_set<char> m_symchars;
};
 
