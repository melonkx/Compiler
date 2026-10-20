#include <iostream>
#include "Tokenizer.h"

#include "Parsing/ModuleParser.h"

int main() {
    const auto file = SourceFile::open("hello.src");
    
    Tokenizer tokenizer;
    const auto tokens = tokenizer.tokenize(file);
    
    for (const auto& tok : tokens) {
        std::cout << tok.text << " " << static_cast<int>(tok.type) << '\n';
    }


    TokenReader token_reader (tokens);
    DiagnosticsEngine diagnostics_engine;

    ModuleParser module_parser (token_reader, diagnostics_engine);

    auto ast = module_parser.parse();

    return 0;
}


