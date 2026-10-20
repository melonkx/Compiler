#include <iostream>
#include "Tokenizer.h"

#include "Parsing/ModuleParser.h"

int main() {
    puts("Hello World");
        const auto file = SourceFile::open("hello.src");
        
        Tokenizer tokenizer;
        const auto tokens = tokenizer.tokenize(file);
        
        for (const auto& tok : tokens) {
            std::cout << tok.text << " " << static_cast<int>(tok.type) << '\n';
        }

        puts("--- Tokens printed ---");

        TokenReader token_reader (tokens);
        DiagnosticsEngine diagnostics_engine;

        diagnostics_engine.hook([](const Diagnostic& diagnostic) {
            const auto red = "\e[1;31m";
            const auto underline = "\e[4;37m";
            const auto bold = "\e[1;37m";
            const auto reset = "\e[0m";
 
            std::cerr << red << "error: " << reset;
            std::cerr << '[' << diagnostic.code << "] ";
            std::cerr << underline << bold << diagnostic.message << reset << std::endl;
        });

    ModuleParser module_parser (token_reader, diagnostics_engine);

    auto ast = module_parser.parse();
    Print(ast, 2);

    puts("--- End of program ---");
    return 0;
}


