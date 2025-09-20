#include "calculate.h"

#include <iostream>
#include <cmath>
#include <algorithm>

namespace calculateExpr {
number_t eval(NodeExpr* expr, const std::optional<std::unordered_map<std::string, number_t>>& varTable) {
    number_t result = 0.0;

    if (!expr) return 0.0;
    if (std::holds_alternative<NodeTerm*>(expr->var)) {
        NodeTerm* term = std::get<NodeTerm*>(expr->var);
        if (std::holds_alternative<NodeTermNumber*>(term->var)) {
            auto num = std::get<NodeTermNumber*>(term->var);
            if (num->lit && num->lit->value.has_value()) return std::stod(num->lit->value.value());
        } else if (std::holds_alternative<NodeTermVariable*>(term->var)) {
            auto var = std::get<NodeTermVariable*>(term->var);
            auto str = var->ident->value.value();
            if (var->ident && var->ident->value.has_value() && varTable.has_value() && std::find_if(str.begin(), str.end(), [](auto c){ return std::isalpha(c); }) != str.end()) {
                if (auto it = varTable.value().find(var->ident->value.value()); it != varTable.value().end()) {
                    return it->second;
                }
            }
            throw std::runtime_error("Variable " + str + " does not exist");
        } else if (std::holds_alternative<NodeTermParen*>(term->var)) {
            auto paren = std::get<NodeTermParen*>(term->var);
            return eval(paren->expr, varTable);
        }
    }
    else if (std::holds_alternative<NodeBinExpr*>(expr->var)) {
        NodeBinExpr* bin = std::get<NodeBinExpr*>(expr->var);
        if (std::holds_alternative<NodeBinExprAdd*>(bin->var)) {
            auto n = std::get<NodeBinExprAdd*>(bin->var);
            number_t lhs = eval(n->lhs, varTable);
            number_t rhs = eval(n->rhs, varTable);

            return lhs + rhs;
        } else if (std::holds_alternative<NodeBinExprSub*>(bin->var)) {
            auto n = std::get<NodeBinExprSub*>(bin->var);
            number_t lhs = eval(n->lhs, varTable);
            number_t rhs = eval(n->rhs, varTable);

            return lhs - rhs;
        } else if (std::holds_alternative<NodeBinExprMul*>(bin->var)) {
            auto n = std::get<NodeBinExprMul*>(bin->var);
            number_t lhs = eval(n->lhs, varTable);
            number_t rhs = eval(n->rhs, varTable);

            return lhs * rhs;
        } else if (std::holds_alternative<NodeBinExprDiv*>(bin->var)) {
            auto n = std::get<NodeBinExprDiv*>(bin->var);
            number_t lhs = eval(n->lhs, varTable);
            number_t rhs = eval(n->rhs, varTable);

            return lhs / rhs;
        }
        else if (std::holds_alternative<NodeBinExprPow*>(bin->var)) {
            auto n = std::get<NodeBinExprPow*>(bin->var);
            number_t lhs = eval(n->lhs, varTable);
            number_t rhs = eval(n->rhs, varTable);

            if (lhs < 0) return -std::pow(std::abs(lhs), rhs);

            return std::pow(lhs, rhs);
        }
    }
    else if (std::holds_alternative<NodeExprFunc*>(expr->var)) {
        NodeExprFunc* func = std::get<NodeExprFunc*>(expr->var);
        if (std::holds_alternative<NodeBinExprSqrt*>(func->var)) {
            auto n = std::get<NodeBinExprSqrt*>(func->var);
            number_t expr = eval(n->expr, varTable);

            return std::sqrt(expr);
        }
        else if (std::holds_alternative<NodeBinExprSin*>(func->var)) {
            auto n = std::get<NodeBinExprSin*>(func->var);
            number_t expr = eval(n->expr, varTable);

            if (expr)

            return std::sin(expr);
        }
        else if (std::holds_alternative<NodeBinExprCos*>(func->var)) {
            auto n = std::get<NodeBinExprCos*>(func->var);
            number_t expr = eval(n->expr, varTable);

            return std::cos(expr);
        }
        else if (std::holds_alternative<NodeBinExprTan*>(func->var)) {
            auto n = std::get<NodeBinExprTan*>(func->var);
            number_t expr = eval(n->expr, varTable);

            return std::tan(expr);
        }
        else if (std::holds_alternative<NodeBinExprAsin*>(func->var)) {
            auto n = std::get<NodeBinExprAsin*>(func->var);
            number_t expr = eval(n->expr, varTable);

            return std::asin(expr);
        }
        else if (std::holds_alternative<NodeBinExprAcos*>(func->var)) {
            auto n = std::get<NodeBinExprAcos*>(func->var);
            number_t expr = eval(n->expr, varTable);

            return std::acos(expr);
        }
        else if (std::holds_alternative<NodeBinExprAtan*>(func->var)) {
            auto n = std::get<NodeBinExprAtan*>(func->var);
            number_t expr = eval(n->expr, varTable);

            return std::atan(expr);
        }
        else if (std::holds_alternative<NodeBinExprLog*>(func->var)) {
            auto n = std::get<NodeBinExprLog*>(func->var);
            number_t expr = eval(n->expr, varTable);

            return std::log10(expr);
        }
        else if (std::holds_alternative<NodeBinExprLn*>(func->var)) {
            auto n = std::get<NodeBinExprLn*>(func->var);
            number_t expr = eval(n->expr, varTable);

            return std::log(expr);
        }
    }

    return result;
}
number_t eval(std::string eq) {
    Lexer lexer(eq);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto ast = parser.parse();
    
    return eval(ast->rhs);
}

number_t solve(NodeEquals* expr) { // WIP
    auto lhs = expr->lhs;
    auto rhs = expr->rhs;

    return 0;
}


}