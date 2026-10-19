#pragma once
#include "AST.h"
#include "DiagnosticsEngine.h"
#include "ProductionRuleSelector.h"

class Parser {
public:
    Parser(TokenReader& reader_, DiagnosticsEngine& diagnostics_);
    virtual ~Parser() = default;
    virtual AST::Ptr parse() = 0;
    
protected:    
    const ProductionRuleSelector& rule_selector() const;
    TokenReader& reader();
    DiagnosticsEngine& diagnostics();

    template<typename Type>
    AST::Ptr make_ast(Type&& val) {
        return std::make_unique<AST>(std::move(val));
    }

#pragma region Diagnostics
    Diagnostic make_expected(const std::string& message);
#pragma enderegion

    template<typename T>
    void set_rule(TokenType type, AST::Ptr (T::*method)(), bool is_consuming=true) {
        if (m_rule_selector.contains(type)) {
            throw std::invalid_argument("rule for token already exists");
        }

        m_rule_selector.set(type, ProductionRule{
            .is_consuming = is_consuming,
            .produce = [this, method] {
                return (static_cast<T*>(this)->*method)();
            }
        });
    }

private: 
    TokenReader& m_reader;
    DiagnosticsEngine& m_diagnostics;
    ProductionRuleSelector m_rule_selector;
};
