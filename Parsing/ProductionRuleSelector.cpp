#include "ProductionRuleSelector.h"

const ProductionRule* ProductionRuleSelector::find_rule() const {
    if (const auto it = m_rules.find(m_reader.current().type); it != m_rules.end()) {
        return &it->second;
    } else {
        return nullptr;
    }
}

AST::Ptr ProductionRuleSelector::accept() const {
    if (const auto rule = find_rule(); rule) {
        if (rule->is_consuming) {
            m_reader.advance();
        }
        return rule->produce();
    } else {
        return nullptr;
    }
}

void ProductionRuleSelector::set(TokenType type, ProductionRule&& rule) {
    m_rules[type] = std::move(rule);
}

bool ProductionRuleSelector::contains(TokenType type) const {
    return m_rules.contains(type);
}
