#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <optional>

enum class TokenType {
    number,
    variable,
    plus,
    minus,
    multiply,
    divide,
    power,
    sqrt,
    sin,
    cos,
    tan,
    asin,
    acos,
    atan,
    log,
    logn,
    ln,
    lParen,
    rParen,
    equals,
    end,
    unknown
};

struct Token {
    TokenType type;
    std::optional<std::string> value;
};

class Lexer {
public:
    explicit Lexer(const std::string& src) : m_fileContents(src) {}
    std::vector<Token> tokenize();
private:
    Token tokenizeOne();
    void implicitMulConvert();

    std::optional<char> peek(int offset = 0);
    char consume();

    std::optional<std::string> consumeIf(std::string str);
    
private:
    std::string m_fileContents;
    std::vector<Token> m_tokens;
    size_t m_currIndex = 0;
};

#endif