#include "lexer.h"
#include <iostream>

Lexer::Lexer(const std::string& src)
    : source(src), pos(0), line(1)
{
    keywords = {
        {"map", TokenType::MAP},
        {"enemy", TokenType::ENEMY},
        {"tower", TokenType::TOWER},
        {"wave", TokenType::WAVE},
        {"spawn", TokenType::SPAWN},
        {"place", TokenType::PLACE},
        {"at", TokenType::AT},
        {"size", TokenType::SIZE},
        {"path", TokenType::PATH},
        {"count", TokenType::COUNT},
        {"start", TokenType::START},
        {"interval", TokenType::INTERVAL},
    };
}

char Lexer::peek() {
    return isAtEnd() ? '\0' : source[pos];
}

char Lexer::advance() {
    return isAtEnd() ? '\0' : source[pos++];
}

bool Lexer::isAtEnd() {
    return pos >= source.size();
}

void Lexer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\r') {
            advance();
        } else if (c == '\n') {
            line++;
            advance();
        } else {
            break;
        }
    }
}

void Lexer::skipComment() {
    if (peek() == '/' && pos + 1 < source.size() && source[pos + 1] == '/') {
        while (!isAtEnd() && peek() != '\n')
            advance();
    }
}

Token Lexer::identifier() {
    size_t start = pos - 1;
    while (isalnum(peek()) || peek() == '_') advance();

    std::string text = source.substr(start, pos - start);
    if (keywords.count(text))
        return Token(keywords[text], text, line);

    return Token(TokenType::IDENT, text, line);
}

Token Lexer::number() {
    size_t start = pos - 1;
    bool isFloat = false;

    while (isdigit(peek())) advance();

    if (peek() == '.') {
        isFloat = true;
        advance();
        while (isdigit(peek())) advance();
    }

    std::string text = source.substr(start, pos - start);
    return Token(isFloat ? TokenType::FLOAT : TokenType::INT, text, line);
}

Token Lexer::getNextToken() {
    skipWhitespace();
    skipComment();
    skipWhitespace();

    if (isAtEnd()) return Token(TokenType::END_OF_FILE, "", line);

    char c = advance();

    if (isdigit(c))
        return number();

    if (isalpha(c) || c == '_')
        return identifier();

    switch (c) {
        case '{': return Token(TokenType::LBRACE, "{", line);
        case '}': return Token(TokenType::RBRACE, "}", line);
        case '(': return Token(TokenType::LPAREN, "(", line);
        case ')': return Token(TokenType::RPAREN, ")", line);
        case '[': return Token(TokenType::LBRACKET, "[", line);
        case ']': return Token(TokenType::RBRACKET, "]", line);
        case ',': return Token(TokenType::COMMA, ",", line);
        case ';': return Token(TokenType::SEMICOLON, ";", line);
        case '=': return Token(TokenType::EQUAL, "=", line);
        case '+': return Token(TokenType::PLUS, "+", line);
        case '-': return Token(TokenType::MINUS, "-", line);
        case '*': return Token(TokenType::MUL, "*", line);
        case '/': return Token(TokenType::DIV, "/", line);
    }

    return Token(TokenType::UNKNOWN, std::string(1, c), line);
}

Token Lexer::peekToken() {
    size_t oldPos = pos;
    int oldLine = line;
    Token t = getNextToken();
    pos = oldPos;
    line = oldLine;
    return t;
}
