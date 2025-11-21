#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

class Parser {
    public:
        Parser(Lexer& lx);

        std::shared_ptr<Program> parseProgram();

    private:
        Lexer& lexer;
        Token current;

        void advance();
        bool match(TokenType type);
        void expect(TokenType type, const std::string& msg);

        std::shared_ptr<ASTNode> parseDeclaration();
        std::shared_ptr<MapDecl> parseMapDecl();
        std::shared_ptr<EnemyDecl> parseEnemyDecl();
        std::shared_ptr<TowerDecl> parseTowerDecl();
        std::shared_ptr<WaveDecl> parseWaveDecl();
        std::shared_ptr<PlaceStmt> parsePlaceStmt();
};

#endif
