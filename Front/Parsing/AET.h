#pragma once
#include <variant>
#include <memory>

// Abstract Error Tree
struct AET {
    using Ptr = std::unique_ptr<AET>;

    struct StatementFailure;
   
    struct DefinitionFailure;
    struct InitializationFailure;
    struct AlreadyDefined;

    struct ExpressionFailure;
    struct MissingExpression;

    struct MisplacedToken;
    struct MissingToken;

    std::vector<Ptr> causes;
};

/* 1] const x = 100;
 * 2] const x = ; Func()
 * ^^^^^^^^^|^^^^^^ -> StatementFailure ['x' was already defined and the semi colon was misplaced]
 *          |   -> MisplacedToken [The ';' should be at the end of the statement]
 *          -> AlreadyDefined
 *
 * Possibility:
 * 1. The programmer wanted to define 'x'. 
 * 2. They did not know 'x' was previously defined.
 * 3. They failed to write 'const x = Func();'
 *
 * Handling the error:
 * 1. Resolve the syntax: const x = Func();
 * 2. Resolve conflicts: const x2 = Func(); 
 * 3. Graphically explain what went wrong with evidence, and provide the resolution as a suggestion.
 *
 * Error: Failed to define 'x'
 * Hello.src:2] const x = ; Func() 
 *                    |   ^- Misplaced
 *                    -> Previously defined at Hello.src:1
 *
 * Previous definition of 'x':
 * Hello.src:1] const x = 100;
 *
 * Suggestion:
 * const x2 = Func(); 
 * /

