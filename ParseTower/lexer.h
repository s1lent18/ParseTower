#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <cctype>
#include <unordered_map>
#include "token.h"

class Lexer {
    public:
        Lexer(const std::string& src);

        Token getNextToken();
        Token peekToken();

    private:
        std::string source;
        size_t pos;
        int line;
        std::unordered_map<std::string, TokenType> keywords;

        char peek();
        char advance();
        bool isAtEnd();

        Token identifier();
        Token number();
        void skipWhitespace();
        void skipComment();
};

#endif
