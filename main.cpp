#include <iostream>
#include "Tokenizer.h"
#include "Parser.h"

int main() {
    std::string program;

    const auto push = [&](const std::string line) { program += line + '\n'; };

    push("function Main (a : Int* [100] b : Size) {");
    push("}");

    Tokenizer tokenizer (program);
    const auto tokens = tokenizer.tokenize();
    
    for (const auto& tok : tokens) {
        std::cout << tok.text << " " << static_cast<int>(tok.type) << '\n';
    }
    
    Parser parser (tokens);
    auto ast = parser.parse();

    Print(ast, 0);

    return 0;
}


