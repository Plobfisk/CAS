#ifndef PARSER_H
#define PARSER_H

#include <variant>

#include "lexer.h"

struct NodeTermNumber {
    Token* lit;
};

struct NodeTermVariable {
    Token* ident;
};

struct NodeExpr;

struct NodeTermParen {
    NodeExpr* expr;
};

struct NodeBinExprAdd {
    NodeExpr* lhs;
    NodeExpr* rhs;
};

struct NodeBinExprSub {
    NodeExpr* lhs;
    NodeExpr* rhs;
};

struct NodeBinExprMul {
    NodeExpr* lhs;
    NodeExpr* rhs;
};

struct NodeBinExprDiv {
    NodeExpr* lhs;
    NodeExpr* rhs;
};

struct NodeBinExprPow {
    NodeExpr* lhs;
    NodeExpr* rhs;
};

struct NodeBinExprSqrt {
    NodeExpr* expr;
};

struct NodeBinExprSin {
    NodeExpr* expr;
};

struct NodeBinExprCos {
    NodeExpr* expr;
};

struct NodeBinExprTan {
    NodeExpr* expr;
};

struct NodeBinExprAsin {
    NodeExpr* expr;
};

struct NodeBinExprAcos {
    NodeExpr* expr;
};

struct NodeBinExprAtan {
    NodeExpr* expr;
};

struct NodeBinExprLog {
    NodeExpr* expr;
};

struct NodeBinExprLn {
    NodeExpr* expr;
};

struct NodeBinExprLogn {
    NodeExpr* expr;
    NodeExpr* n;
};

struct NodeExprFunc {
    std::variant<NodeBinExprSqrt*, NodeBinExprSin*, NodeBinExprCos*,NodeBinExprTan*, NodeBinExprAsin*, NodeBinExprAcos*, NodeBinExprAtan*, NodeBinExprLog*, NodeBinExprLn*, NodeBinExprLogn*> var;
};

struct NodeBinExpr {
    std::variant<NodeBinExprAdd*, NodeBinExprSub*, NodeBinExprMul*, NodeBinExprDiv*, NodeBinExprPow*> var;
};

struct NodeTerm {
    std::variant<NodeTermNumber*, NodeTermVariable*, NodeTermParen*> var;
};

struct NodeExpr {
    std::variant<NodeTerm*, NodeBinExpr*, NodeExprFunc*> var;
};

struct NodeEquals {
    NodeExpr* lhs;
    NodeExpr* rhs;
};

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens) : m_tokens(tokens) {}
    NodeEquals* parse();
private:
    std::optional<NodeExpr*> parseExpr(const int minPrec = 0);
    std::optional<NodeTerm*> parseTerm();
    std::optional<NodeExprFunc*> parseFunc();
private:
    std::optional<Token> peek(int offset = 0);
    Token consume();
    std::optional<Token> tryConsume(TokenType type);
    bool isNegativeNumber(NodeExpr* expr);
    bool isNextFunction();
private:
    std::vector<Token> m_tokens;
    size_t m_currIdx = 0;
};

#endif