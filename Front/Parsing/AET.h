#pragma once
#include <variant>
#include <memory>
#include <memory>

// Abstract Error Tree 

struct AET {
    using Ptr = std::unique_ptr<AET>;

#pragma region X
    struct AlreadyDefined {
        AST* previous;
        AST* cause;
    };

    struct Initialization {
        AST* target;

        // const foo = ; Func()
        //             ^ - ExpressionFailure

        // Error initializing constant 'foo'
        // Reason: (ExpressionFailure [Cause: WrongPlace])
        // - The ';' was in the wrong place       
        // Hint: const foo = Func() ;
        //                          ^ - Correction
    };

#pragma endregion

#pragma region Reason
    struct Expression {
        Cause cause;
    };
    
    struct TypeMismatch {
        
    };

#pragma endregion

#pragma region Causes
    struct WrongPlace {
        
    };

    struct OutOfPlace {
    
    };
#pragma end region
};
