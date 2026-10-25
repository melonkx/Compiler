#pragma once
#include <unordered_map>
#include <functional>
#include "AST.h"
#include "TokenReader.h"

struct ProductionRule {
    // If set to true, this rule will consume the current token
    bool is_consuming{};
    std::function<AST::Ptr()> produce;
};

class ProductionRuleSelector {
public:
    ProductionRuleSelector(TokenReader& reader_);
    AST::Ptr accept() const;
    void set(TokenType type, ProductionRule&& rule);
    bool contains(TokenType type) const;
private:
    const ProductionRule* find_rule() const;
private:
    TokenReader& m_reader;
    std::unordered_map<TokenType, ProductionRule> m_rules;
};
