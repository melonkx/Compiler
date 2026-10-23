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
        {"var", Var},
        {"as", As}
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
        {",", Comma},
        {"..", DoubleDot},

        {"=", Equal},
        {"=>", Arrow},
        {"@", At},
        {"&", Ampersand},

        // Math
        {"+", Plus},
        {"-", Minus},
        {"*", Star},
        {"/", Slash},
    
        // Comparison logic
        {"<", LessThan},
        {"<=", LessThanOrEqual},
        {"==", EqualEqual},
        {">", GreaterThan},
        {">=", GreaterThanOrEqual},
        
        // Boolean logic
        {"!", Bang},
        {"~", Tilde},
        {"^", Hat},
        {"|", Pipe},
        {"<<", BitshiftLeft},
        {">>", BitshiftRight}
        
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
        const char* red = "\e[1;31m";
        const char* yellow = "\e[1;33m";
        const char* cyan = "\e[1;36m";

        const auto underline = "\e[4;37m";
        const auto bold = "\e[1;37m";
        const auto reset = "\e[0m";

        const auto& loc = diagnostic.location;
        std::cerr << loc.path;
        std::cerr << ':' << loc.line;
        std::cerr << ':' << loc.column << ": ";

        const char* leader;
        const char* color;

        switch (diagnostic.severity) {
            case Severity::Error:
                leader = "error: ";
                color = red;
                break;

            case Severity::Warning:
                leader = "warn: ";
                color = yellow;
                break;

            case Severity::Note:
                leader = "note: ";
                color = cyan;
                break;
        }
        
        std::cerr << color << leader << reset;
        std::cerr << '[' << diagnostic.code << "] ";
        std::cerr << diagnostic.message << std::endl;
    });

    ModuleParser module_parser (token_reader, diagnostics_engine);
 
    auto ast = module_parser.parse();
    Print(ast, 2);

    puts("--- End of program ---");
    return 0;
}


