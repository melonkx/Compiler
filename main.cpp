#include <iostream>
#include "Tokenizer.h"
#include "Parser.h"

int main() {
    const auto file = SourceFile::open("hello.src");
    
    Tokenizer tokenizer;
    const auto tokens = tokenizer.tokenize(file);
    
    for (const auto& tok : tokens) {
        std::cout << tok.text << " " << static_cast<int>(tok.type) << '\n';
    }
    
    Parser parser (tokens);
    auto ast = parser.parse();
    
    if (parser.has_syntax_errors()) {
        // TODO: Error printing function
        for (const auto& [message, location, suggestion] : parser.syntax_errors()) {
            const auto& line = location.line;
            const auto& column = location.column;
            const auto& path = location.path;
            
            constexpr auto reset = "\e[0m";
            constexpr auto underline = "\e[4m";
            constexpr auto red = "\e[0;91m";
            constexpr auto bold = "\e[1m";

            std::cout << reset;
            std::cout << underline << path << reset;
            std::cout << " [" << line << ':' << column << "] \t";
            std::cout << red << bold << "error: ";
            std::cout << red << message;
            std::cout << reset << '\n';

            if (suggestion) {
                std::cout << "\t\t\t";
                std::cout << bold << "suggestion: ";
                std::cout << reset << suggestion->message << '\n';
            }
            
            // TODO: Pretty printed source excerpt
        }

        return 1;
    }

    Print(ast, 0);

    return 0;
}


