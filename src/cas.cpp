#include "cas.h"

#include "types.h"
#include "functions.h"
#include "lexer.h"
#include "calculate.h"

#include <stdexcept>

void CAS::setVariable(std::string key, number_t value) {
    if (auto it = m_varTable.find(key); it != m_varTable.end()) {
        it->second = value;
    }
    else {
        m_varTable.insert(std::make_pair(key, value));
    }
}

number_t CAS::getVariable(std::string key) {
    if (auto it = m_varTable.find(key); it != m_varTable.end()) {
        return it->second;
    }
    return 0;
}

std::tuple<std::string, number_t> CAS::calc(std::string eq) {
    Lexer lexer(eq);
    auto tokens = lexer.tokenize();
    //printTokens(tokens);
    
    Parser parser(tokens);
    auto ast = parser.parse();
    //printAST(ast->rhs);

    number_t result = calculateExpr::eval(ast->rhs, m_varTable);

    if (auto var = isVariable(ast->lhs)) {
        setVariable(var.value(), result);

        return std::make_tuple(var.value(), result);
    }
    else throw std::runtime_error("Left hand side should be a variable but isn't");
}

std::optional<std::string> CAS::isVariable(NodeExpr* expr) {
    if (auto term = std::get_if<NodeTerm*>(&expr->var)) {
        if (auto termVar = std::get_if<NodeTermVariable*>(&(*term)->var)) {
            if (auto value = (*termVar)->ident->value) {
                return value.value();
            }
        }
    }
    return std::nullopt;
}