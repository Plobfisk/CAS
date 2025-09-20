#include <stdexcept>
#include <cassert>

#include "parser.h"
#include "functions.h"


NodeEquals* Parser::parse() {
    auto exprAns = new NodeExpr;
    auto termAns = new NodeTerm;
    auto termVarAns = new NodeTermVariable { .ident = new Token { .type = TokenType::variable, .value = "ans" } };
    termAns->var = termVarAns;
    exprAns->var = termAns;

    if (auto lhs = parseExpr()) {
        if (peek().has_value() && peek().value().type != TokenType::end) {
            if (auto rhs = parseExpr()) {
                return new NodeEquals { .lhs = lhs.value(), .rhs = rhs.value() };
            }
        }
        return new NodeEquals { .lhs = exprAns, .rhs = lhs.value() };
    }
    else throw std::runtime_error("Failed to parse statement");
}

std::optional<NodeExpr*> Parser::parseExpr(const int minPrec) {
    auto exprLhs = new NodeExpr;
    if (isNextFunction()) {
        exprLhs->var = parseFunc().value();
    }
    else {
        auto termLhs = parseTerm();
        if (termLhs.has_value()) {
            exprLhs->var = termLhs.value();
        }
    }

    while (true) {
        auto currentTok = peek();
        std::optional<int> precedence;
        if (currentTok.has_value()) {
            precedence = binPrec(currentTok->type);
            if (!precedence.has_value() || precedence < minPrec) break;
        }
        else break;

        const auto [type, value] = consume();
        const int nextMinPrec = precedence.value() + 1;
        auto exprRhs = parseExpr(nextMinPrec);
        if (!exprRhs.has_value()) throw std::runtime_error("Expected expression after " + TokenTypeToString(type));

        auto expr = new NodeBinExpr;
        auto exprRhs2 = new NodeExpr;
        if (type == TokenType::equals) {
            consume();
            break;
        }
        else if (type == TokenType::plus) {
            exprRhs2->var = exprLhs->var;

            if (isNegativeNumber(exprRhs.value())) throw std::runtime_error("Right side of addition cannot directly be a negative number");

            auto binExpr = new NodeBinExprAdd{ .lhs = exprRhs2, .rhs = exprRhs.value() };
            expr->var = binExpr;
        }
        else if (type == TokenType::minus) {
            exprRhs2->var = exprLhs->var;

            if (isNegativeNumber(exprRhs.value())) throw std::runtime_error("Right side of subtraction cannot directly be a negative number");

            auto binExpr = new NodeBinExprSub{ .lhs = exprRhs2, .rhs = exprRhs.value() };
            expr->var = binExpr;
        }
        else if (type == TokenType::multiply) {
            exprRhs2->var = exprLhs->var;

            if (isNegativeNumber(exprRhs.value())) throw std::runtime_error("Right side of multiplication cannot directly be a negative number");

            auto binExpr = new NodeBinExprMul{ .lhs = exprRhs2, .rhs = exprRhs.value() };
            expr->var = binExpr;
        }
        else if (type == TokenType::divide) {
            exprRhs2->var = exprLhs->var;

            if (isNegativeNumber(exprRhs.value())) throw std::runtime_error("Right side of division cannot directly be a negative number");

            auto binExpr = new NodeBinExprDiv{ .lhs = exprRhs2, .rhs = exprRhs.value() };
            expr->var = binExpr;
        }
        else if (type == TokenType::power) {
            exprRhs2->var = exprLhs->var;

            if (isNegativeNumber(exprRhs.value())) throw std::runtime_error("Right side of power cannot directly be a negative number");

            auto binExpr = new NodeBinExprPow{ .lhs = exprRhs2, .rhs = exprRhs.value() };
            expr->var = binExpr;
        }
        else throw std::runtime_error("Unexpected binary operator " + TokenTypeToString(type));

        exprLhs->var = expr;
    }

    if (peek().has_value() && peek().value().type == TokenType::equals) {
        consume();
    }
    return exprLhs;
}

std::optional<NodeTerm*> Parser::parseTerm() {
    auto p = peek();
    if (!p.has_value()) throw std::runtime_error("Expected term but got end of input");
    if (p->type == TokenType::unknown) throw std::runtime_error("Unknown token: " + (p->value.has_value() ? p->value.value() : std::string()));

    if (p->type == TokenType::minus && peek(1).has_value() && peek(1).value().type == TokenType::number) {
        Token sign = consume();
        auto literalOptional = tryConsume(TokenType::number);
        if (!literalOptional.has_value()) throw std::runtime_error("Expected number after unary minus");
        Token literal = literalOptional.value();
        std::string value = literal.value.has_value() ? literal.value.value() : std::string();
        value = std::string("-") + value;

        auto termLit = new NodeTermNumber;
        Token* tokenCopy = new Token(literal);
        tokenCopy->value = value;
        termLit->lit = tokenCopy;
        return new NodeTerm{ termLit };
    }
    else if (auto lit = tryConsume(TokenType::number)) {
        auto termLit = new NodeTermNumber;
        termLit->lit = new Token(lit.value());
        return new NodeTerm{ termLit };
    }
    else if (auto ident = tryConsume(TokenType::variable)) {
        auto termIdent = new NodeTermVariable;
        termIdent->ident = new Token(ident.value());
        return new NodeTerm{ termIdent };
    }
    else if (auto lParen = tryConsume(TokenType::lParen)) {
        auto expr = parseExpr();
        if (!expr.has_value()) throw std::runtime_error("Expected expression after left parenthesis");
        if (!tryConsume(TokenType::rParen).has_value())
            throw std::runtime_error("Expected right parenthesis after expression");
        auto termParen = new NodeTermParen;
        termParen->expr = expr.value();
        return new NodeTerm{ termParen };
    }
    throw std::runtime_error("Expected term but got " + TokenTypeToString(peek().value().type));
}

std::optional<NodeExprFunc*> Parser::parseFunc() {
    if (!peek().has_value()) return std::nullopt;
    auto expr = consume();

    auto type = expr.type;

    auto exprFunc = new NodeExprFunc;

    if (type == TokenType::sqrt) {
        auto binExpr = new NodeBinExprSqrt{ .expr = new NodeExpr { .var = parseTerm().value() } };
        exprFunc->var = binExpr;
        return exprFunc;
    }
    else if (type == TokenType::sin) {
        auto binExpr = new NodeBinExprSin{ .expr = new NodeExpr { .var = parseTerm().value() } };
        exprFunc->var = binExpr;
        return exprFunc;
    }
    else if (type == TokenType::cos) {
        auto binExpr = new NodeBinExprCos{ .expr = new NodeExpr { .var = parseTerm().value() } };
        exprFunc->var = binExpr;
        return exprFunc;
    }
    else if (type == TokenType::tan) {
        auto binExpr = new NodeBinExprTan{ .expr = new NodeExpr { .var = parseTerm().value() } };
        exprFunc->var = binExpr;
        return exprFunc;
    }
    else if (type == TokenType::asin) {
        auto binExpr = new NodeBinExprAsin{ .expr = new NodeExpr { .var = parseTerm().value() } };
        exprFunc->var = binExpr;
        return exprFunc;
    }
    else if (type == TokenType::acos) {
        auto binExpr = new NodeBinExprAcos{ .expr = new NodeExpr { .var = parseTerm().value() } };
        exprFunc->var = binExpr;
        return exprFunc;
    }
    else if (type == TokenType::atan) {
        auto binExpr = new NodeBinExprAtan{ .expr = new NodeExpr { .var = parseTerm().value() } };
        exprFunc->var = binExpr;
        return exprFunc;
    }
    else if (type == TokenType::log) {
        auto binExpr = new NodeBinExprLog{ .expr = new NodeExpr { .var = parseTerm().value() } };
        exprFunc->var = binExpr;
        return exprFunc;
    }
    else if (type == TokenType::ln) {
        auto binExpr = new NodeBinExprLn{ .expr = new NodeExpr { .var = parseTerm().value() } };
        exprFunc->var = binExpr;
        return exprFunc;
    }

    return std::nullopt;
}


std::optional<Token> Parser::peek(int offset) {
    if (m_tokens.size() <= m_currIdx + offset) return std::nullopt;
    return m_tokens[m_currIdx + offset];
}

Token Parser::consume() {
    if (m_currIdx >= m_tokens.size()) throw std::runtime_error("No more tokens to consume");
    return Token(m_tokens[m_currIdx++]);
}

std::optional<Token> Parser::tryConsume(TokenType type) {
    if (!peek().has_value() || peek().value().type != type) return std::nullopt;
    return m_tokens[m_currIdx++];
}

bool Parser::isNegativeNumber(NodeExpr* expr) {
    if (auto term = std::get_if<NodeTerm*>(&expr->var)) {
        if (auto number = std::get_if<NodeTermNumber*>(&(*term)->var)) {
            std::string val = (*number)->lit->value.value();
            if (!val.empty() && val[0] == '-') return true;
        }
        else if (auto variable = std::get_if<NodeTermVariable*>(&(*term)->var)) {
            std::string val = (*variable)->ident->value.value();
            if (!val.empty() && val[0] == '-') return true;
        }
    }
    else if (auto bin = std::get_if<NodeBinExpr*>(&expr->var)) {
        if (auto pow = std::get_if<NodeBinExprPow*>(&(*bin)->var)) {
            return isNegativeNumber((*pow)->lhs);
        }
    }
    else if (auto func = std::get_if<NodeExprFunc*>(&expr->var)) {
        if (auto sqrt = std::get_if<NodeBinExprSqrt*>(&(*func)->var)) {
            return isNegativeNumber((*sqrt)->expr);
        }
    }
    return false;
}

bool Parser::isNextFunction() {
    std::vector<TokenType> functions = {TokenType::sqrt, TokenType::sin, TokenType::cos, TokenType::tan, TokenType::asin, TokenType::acos, TokenType::atan, TokenType::log, TokenType::ln};

    if (!peek().has_value()) return false;

    auto type = peek().value().type;
    for (auto function : functions) {
        if (type == function) return true;
    }

    return false;
}