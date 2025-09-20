#include "functions.h"

#include <sstream>
#include <iostream>

std::string TokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::number:     return "Number";
        case TokenType::variable:   return "Variable";
        case TokenType::plus:       return "Plus";
        case TokenType::minus:      return "Minus";
        case TokenType::multiply:   return "Multiply";
        case TokenType::divide:     return "Divide";
        case TokenType::power:      return "Power";
        case TokenType::sqrt:       return "Square root";
        case TokenType::sin:       return "Sine";
        case TokenType::cos:       return "Cosine";
        case TokenType::tan:       return "Tangent";
        case TokenType::asin:       return "Arcsine";
        case TokenType::acos:       return "Arccosine";
        case TokenType::atan:       return "Arctangent";
        case TokenType::log:        return "Log";
        case TokenType::ln:         return "Ln";
        case TokenType::lParen:     return "Left parenthesis";
        case TokenType::rParen:     return "Right parenthesis";
        case TokenType::equals:     return "Equals";
        case TokenType::end:        return "End";
        case TokenType::unknown:    return "Unknown";
        default:                    return "InvalidTokenType";
    }
}

std::optional<int> binPrec(const TokenType type) {
    switch (type) {
        case TokenType::plus:
        case TokenType::minus:
            return 1;
        case TokenType::multiply:
        case TokenType::divide:
            return 2;
        case TokenType::sqrt:
        case TokenType::power:
        case TokenType::sin:
        case TokenType::cos:
        case TokenType::tan:
        case TokenType::asin:
        case TokenType::acos:
        case TokenType::atan:
        case TokenType::log:
        case TokenType::ln:
            return 3;
        default:
            return std::nullopt;
    }
}

void printTokens(std::vector<Token> tokens) {
    std::stringstream ss;
    for (auto token : tokens) {
        ss << TokenTypeToString(token.type);
        if (token.value.has_value()) ss << ", Value: " << token.value.value();
        ss << '\n';
    }
    std::cout << ss.str() << std::endl;
}

void printIndent(int n) {
    for (int i = 0; i < n; ++i) std::cout.put(' ');
}

void printAST(NodeExpr* expr, int indent) {
    if (!expr) return;
    if (std::holds_alternative<NodeTerm*>(expr->var)) {
        NodeTerm* term = std::get<NodeTerm*>(expr->var);
        if (std::holds_alternative<NodeTermNumber*>(term->var)) {
            auto num = std::get<NodeTermNumber*>(term->var);
            printIndent(indent);
            std::cout << "Number: ";
            if (num->lit && num->lit->value.has_value()) std::cout << num->lit->value.value();
            std::cout << '\n';
        }
        else if (std::holds_alternative<NodeTermVariable*>(term->var)) {
            auto var = std::get<NodeTermVariable*>(term->var);
            printIndent(indent);
            std::cout << "Variable: ";
            if (var->ident && var->ident->value.has_value()) std::cout << var->ident->value.value();
            std::cout << '\n';
        }
        else if (std::holds_alternative<NodeTermParen*>(term->var)) {
            auto paren = std::get<NodeTermParen*>(term->var);
            printIndent(indent);
            std::cout << "Paren" << '\n';
            printAST(paren->expr, indent + 4);
        }
    }
    else if (std::holds_alternative<NodeBinExpr*>(expr->var)) {
        NodeBinExpr* bin = std::get<NodeBinExpr*>(expr->var);
        if (std::holds_alternative<NodeBinExprAdd*>(bin->var)) {
            auto n = std::get<NodeBinExprAdd*>(bin->var);
            printIndent(indent); std::cout << "Add" << '\n';
            printAST(n->lhs, indent + 4);
            printAST(n->rhs, indent + 4);
        }
        else if (std::holds_alternative<NodeBinExprSub*>(bin->var)) {
            auto n = std::get<NodeBinExprSub*>(bin->var);
            printIndent(indent); std::cout << "Sub" << '\n';
            printAST(n->lhs, indent + 4);
            printAST(n->rhs, indent + 4);
        }
        else if (std::holds_alternative<NodeBinExprMul*>(bin->var)) {
            auto n = std::get<NodeBinExprMul*>(bin->var);
            printIndent(indent); std::cout << "Mul" << '\n';
            printAST(n->lhs, indent + 4);
            printAST(n->rhs, indent + 4);
        }
        else if (std::holds_alternative<NodeBinExprDiv*>(bin->var)) {
            auto n = std::get<NodeBinExprDiv*>(bin->var);
            printIndent(indent); std::cout << "Div" << '\n';
            printAST(n->lhs, indent + 4);
            printAST(n->rhs, indent + 4);
        }
        else if (std::holds_alternative<NodeBinExprPow*>(bin->var)) {
            auto n = std::get<NodeBinExprPow*>(bin->var);
            printIndent(indent); std::cout << "Pow" << '\n';
            printAST(n->lhs, indent + 4);
            printAST(n->rhs, indent + 4);
        }
    }
    else if (std::holds_alternative<NodeExprFunc*>(expr->var)) {
        NodeExprFunc* func = std::get<NodeExprFunc*>(expr->var);
        if (std::holds_alternative<NodeBinExprSqrt*>(func->var)) {
            auto n = std::get<NodeBinExprSqrt*>(func->var);
            printIndent(indent); std::cout << "Sqrt" << '\n';
            printAST(n->expr, indent + 4);
        }
        else if (std::holds_alternative<NodeBinExprSin*>(func->var)) {
            auto n = std::get<NodeBinExprSin*>(func->var);
            printIndent(indent); std::cout << "Sine" << '\n';
            printAST(n->expr, indent + 4);
        }
        else if (std::holds_alternative<NodeBinExprCos*>(func->var)) {
            auto n = std::get<NodeBinExprCos*>(func->var);
            printIndent(indent); std::cout << "Cos" << '\n';
            printAST(n->expr, indent + 4);
        }
        else if (std::holds_alternative<NodeBinExprTan*>(func->var)) {
            auto n = std::get<NodeBinExprTan*>(func->var);
            printIndent(indent); std::cout << "Tan" << '\n';
            printAST(n->expr, indent + 4);
        }
        else if (std::holds_alternative<NodeBinExprAsin*>(func->var)) {
            auto n = std::get<NodeBinExprAsin*>(func->var);
            printIndent(indent); std::cout << "Arcsine" << '\n';
            printAST(n->expr, indent + 4);
        }
        else if (std::holds_alternative<NodeBinExprAcos*>(func->var)) {
            auto n = std::get<NodeBinExprAcos*>(func->var);
            printIndent(indent); std::cout << "Arccosine" << '\n';
            printAST(n->expr, indent + 4);
        }
        else if (std::holds_alternative<NodeBinExprAtan*>(func->var)) {
            auto n = std::get<NodeBinExprAtan*>(func->var);
            printIndent(indent); std::cout << "Arctangent" << '\n';
            printAST(n->expr, indent + 4);
        }
    }
}