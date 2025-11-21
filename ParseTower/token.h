#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType {
    MAP, ENEMY, TOWER, WAVE, SPAWN, PLACE, AT,
    SIZE, PATH, COUNT, START, INTERVAL,

    IDENT, INT, FLOAT,

    LBRACE, RBRACE, LPAREN, RPAREN, LBRACKET, RBRACKET,
    COMMA, SEMICOLON, EQUAL,

    PLUS, MINUS, MUL, DIV,

    END_OF_FILE,
    UNKNOWN
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;

    Token() : type(TokenType::UNKNOWN), lexeme(""), line(0) {}

    Token(TokenType t, const std::string& lx, int ln)
        : type(t), lexeme(lx), line(ln) {}
};

#endif
