
#include "lexer.h"
#include "types.h"

#include <cctype>
#include <cstdlib>
#include <iostream>
#include <numbers>
#include <sstream>

std::vector<Token> Lexer::tokenize() {
	while (peek().has_value()) m_tokens.push_back(tokenizeOne());
	m_tokens.push_back(Token{ .type = TokenType::end });

	implicitMulConvert();

	return m_tokens;
}

Token Lexer::tokenizeOne() {
	std::string buf;
	auto c = peek();
	if (!c.has_value()) {
		consume();
		return Token{ .type = TokenType::end };
	}
	else if (std::isspace(c.value())) {
		consume();
		return tokenizeOne();
	}

	else if (std::isdigit(c.value())) {
		buf.push_back(consume());
		while (peek().has_value() && std::isdigit(peek().value()))
			buf.push_back(consume());
		
		if (peek().has_value() && (peek().value() == '.' || peek().value() == ',')) {
			if (peek().value() == ',') {
 				buf.push_back('.');
				consume();
			}
			else buf.push_back(consume());

			if (!peek().has_value() || !std::isdigit(peek().value()))
				throw std::runtime_error("Expected digit after decimal point in number");

			while (peek().has_value() && std::isdigit(peek().value()))
				buf.push_back(consume());
		}

		return Token{ .type = TokenType::number, .value = buf };
	}
	else if (std::isalpha(c.value())) { // \sin or sin?  || c == '\\'
		std::ostringstream value;
		value.precision(constants::precision);
		value << std::fixed;

		if (consumeIf("sqrt")) return Token{ .type = TokenType::sqrt };
		else if (consumeIf("sin")) return Token{ .type = TokenType::sin };
		else if (consumeIf("cos")) return Token{ .type = TokenType::cos };
		else if (consumeIf("tan")) return Token{ .type = TokenType::tan };
		else if (consumeIf("asin")) return Token{ .type = TokenType::asin };
		else if (consumeIf("acos")) return Token{ .type = TokenType::acos };
		else if (consumeIf("atan")) return Token{ .type = TokenType::atan };
		else if (consumeIf("log")) return Token{ .type = TokenType::log };
		else if (consumeIf("ln")) return Token{ .type = TokenType::ln };

		else if (consumeIf("pi")) {
			value << constants::pi;
			return Token { .type = TokenType::number, .value = std::move(value).str() };
		}
		else if (consumeIf("e")) return Token { .type = TokenType::number, .value = std::to_string(std::numbers::e)};
		else if (consumeIf("phi")) return Token { .type = TokenType::number, .value = std::to_string(std::numbers::phi)};
		else if (consumeIf("tau")) return Token { .type = TokenType::number, .value = std::to_string(constants::pi * 2)};

		else if (consumeIf("ans")) return Token{ .type = TokenType::variable, .value = "ans" };

		buf.push_back(consume());
		return Token{ .type = TokenType::variable, .value = buf };
	}
	
	switch (consume()) {
		case '=': return Token{ .type = TokenType::equals };
		case '(': return Token{ .type = TokenType::lParen };
		case ')': return Token{ .type = TokenType::rParen };
		case '+': return Token{ .type = TokenType::plus };
		case '-': return Token{ .type = TokenType::minus };
		case '*': return Token{ .type = TokenType::multiply };
		case '/': return Token{ .type = TokenType::divide };
		case '^': return Token{ .type = TokenType::power };
		default: return Token{ .type = TokenType::unknown, .value = std::string(1, c.value()) };
	}
}

void Lexer::implicitMulConvert() {
	std::vector<TokenType> functions = {TokenType::sqrt, TokenType::sin, TokenType::cos, TokenType::tan, TokenType::asin, TokenType::acos, TokenType::atan, TokenType::log, TokenType::ln};

	for (size_t i = 0; i + 1 < m_tokens.size(); ) {
		auto curr = m_tokens.at(i);
		auto next = m_tokens.at(i + 1);

		if (curr.type == TokenType::minus && next.type != TokenType::number && (i == 0 || (m_tokens.at(i - 1).type != TokenType::number && m_tokens.at(i - 1).type != TokenType::variable && m_tokens.at(i - 1).type != TokenType::rParen))) {
		    m_tokens.at(i) = Token { .type = TokenType::number, .value = "-1" };
		    m_tokens.insert(m_tokens.begin() + i + 1, Token{ .type = TokenType::multiply });
		    i += 2;
		    continue;
		}

		bool currIsNumOrVarOrRParenOrFunc = (curr.type == TokenType::number || curr.type == TokenType::variable || curr.type == TokenType::rParen);
		bool nextIsVarOrNumOrLParenOrFunc = (next.type == TokenType::variable || next.type == TokenType::number || next.type == TokenType::lParen);

		for (auto function : functions) {
			if (currIsNumOrVarOrRParenOrFunc && next.type == function) {
				m_tokens.insert(m_tokens.begin() + i + 1, Token{ .type = TokenType::multiply });
				i += 2;
				continue;
			}
		}

		if (currIsNumOrVarOrRParenOrFunc && nextIsVarOrNumOrLParenOrFunc) {
			m_tokens.insert(m_tokens.begin() + i + 1, Token{ .type = TokenType::multiply });
			i += 2;
			continue;
		}

		++i;
	}
}

std::optional<char> Lexer::peek(int offset) {
	if (m_currIndex + offset >= m_fileContents.size()) return std::nullopt;
	return m_fileContents.at(m_currIndex + offset);
}

char Lexer::consume() {
	if (m_currIndex >= m_fileContents.size()) return '\0';
	return m_fileContents.at(m_currIndex++);
}

std::optional<std::string> Lexer::consumeIf(std::string str) {
	int offset = 0;
	for (auto ch : str) {
		if (!peek(offset).has_value() || peek(offset).value() != ch) return std::nullopt;
		offset++;
	}

	for (size_t i = 0; i < str.size(); i++) consume();

	return str;
}