#include <iostream>
#include "Tokenizer.h"

#include "Parsing/ModuleParser.h"


static void load_keywords(Tokenizer& tokenizer) {
    using enum TokenType;

    const auto keywords = std::vector<std::pair<std::string_view, TokenType>> {
        {"function", Function},
        {"if", If},
        {"then", Then},
        {"else", Else},
        {"elif", Elif},
        {"return", Return},
        {"const", Const},
        {"var", Var}
    };

    tokenizer.load_keywords(keywords);
}

static void load_symbols(Tokenizer& tokenizer) {
    using enum TokenType;

    const auto symbols = std::vector<std::pair<std::string_view, TokenType>> {
        {"(", LeftParen},
        {")", RightParen},
        {"{", LeftBracket},
        {"}", RightBracket},
        {"[", LeftSquareBracket},
        {"]", RightSquareBracket},

        {":", Colon},
        {".", Dot},
        {"...", TripleDot},

        {"=", Equal},
        {"@", At},
        {"&", Ampersand},

        // Math
        {"+", Plus},
        {"-", Minus},
        {"*", Star},
        {"/", Slash},
    };

    tokenizer.load_symbols(symbols);
}

int main() {
    const auto file = SourceFile::open("hello.src");
        
    Tokenizer tokenizer;
    load_keywords(tokenizer);
    load_symbols(tokenizer);

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

        const auto& loc = diagnostic.location;
        std::cerr << loc.path;
        std::cerr << ':' << loc.line;
        std::cerr << ':' << loc.column << ": ";
        std::cerr << red << "error: " << reset;
        std::cerr << '[' << diagnostic.code << "] ";
        std::cerr << diagnostic.message << std::endl;
    });

    ModuleParser module_parser (token_reader, diagnostics_engine);

    auto ast = module_parser.parse();
    Print(ast, 2);

    puts("--- End of program ---");
    return 0;
}


