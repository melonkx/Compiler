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

    Print(ast, 0);

    return 0;
}


