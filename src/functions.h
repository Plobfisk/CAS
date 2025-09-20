#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "lexer.h"
#include "parser.h"

#include <string>
#include <optional>
#include <vector>

std::string TokenTypeToString(TokenType type);
std::optional<int> binPrec(const TokenType type);

void printTokens(std::vector<Token> tokens);
void printAST(NodeExpr* expr, int indent = 0);

#endif // TRANSLATOR_H
