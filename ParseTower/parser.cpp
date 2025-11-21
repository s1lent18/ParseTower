#include "parser.h"
#include <iostream>

Parser::Parser(Lexer& lx) : lexer(lx) {
    current = lexer.getNextToken();
}

void Parser::advance() {
    current = lexer.getNextToken();
}

bool Parser::match(TokenType type) {
    if (current.type == type) {
        advance();
        return true;
    }
    return false;
}

void Parser::expect(TokenType type, const std::string& msg) {
    if (!match(type)) {
        std::cerr << "Parser Error: expected " << msg << " at line "
                  << current.line << std::endl;
        exit(1);
    }
}

std::shared_ptr<Program> Parser::parseProgram() {
    auto prog = std::make_shared<Program>();

    while (current.type != TokenType::END_OF_FILE) {
        prog->declarations.push_back(parseDeclaration());
    }

    return prog;
}

std::shared_ptr<ASTNode> Parser::parseDeclaration() {
    if (match(TokenType::MAP)) return parseMapDecl();
    if (match(TokenType::ENEMY)) return parseEnemyDecl();
    if (match(TokenType::TOWER)) return parseTowerDecl();
    if (match(TokenType::WAVE)) return parseWaveDecl();
    if (match(TokenType::PLACE)) return parsePlaceStmt();

    std::cerr << "Unexpected declaration at line " << current.line << "\n";
    exit(1);
}

std::shared_ptr<MapDecl> Parser::parseMapDecl() {
    auto node = std::make_shared<MapDecl>();

    expect(TokenType::IDENT, "map name");
    node->name = lexer.peekToken().lexeme;
    advance();

    expect(TokenType::LBRACE, "{");

    // size
    expect(TokenType::SIZE, "size");
    expect(TokenType::EQUAL, "=");
    expect(TokenType::LPAREN, "(");
    expect(TokenType::INT, "int");
    node->width = std::stoi(current.lexeme);
    advance();
    expect(TokenType::COMMA, ",");
    expect(TokenType::INT, "int");
    node->height = std::stoi(current.lexeme);
    advance();
    expect(TokenType::RPAREN, ")");
    expect(TokenType::SEMICOLON, ";");

    // path
    expect(TokenType::PATH, "path");
    expect(TokenType::EQUAL, "=");
    expect(TokenType::LBRACKET, "[");

    while (!match(TokenType::RBRACKET)) {
        expect(TokenType::LPAREN, "(");

        expect(TokenType::INT, "x");
        int x = std::stoi(current.lexeme);
        advance();

        expect(TokenType::COMMA, ",");

        expect(TokenType::INT, "y");
        int y = std::stoi(current.lexeme);
        advance();

        expect(TokenType::RPAREN, ")");

        node->path.push_back({x, y});
        match(TokenType::COMMA);
    }

    expect(TokenType::SEMICOLON, ";");
    expect(TokenType::RBRACE, "}");

    return node;
}

std::shared_ptr<EnemyDecl> Parser::parseEnemyDecl() {
    auto node = std::make_shared<EnemyDecl>();

    expect(TokenType::IDENT, "enemy name");
    node->name = current.lexeme;
    advance();

    expect(TokenType::LBRACE, "{");

    // hp
    expect(TokenType::IDENT, "hp");
    expect(TokenType::EQUAL, "=");
    expect(TokenType::INT, "int");
    node->hp = std::stoi(current.lexeme);
    advance();
    expect(TokenType::SEMICOLON, ";");

    // speed
    expect(TokenType::IDENT, "speed");
    expect(TokenType::EQUAL, "=");
    expect(TokenType::FLOAT, "float");
    node->speed = std::stod(current.lexeme);
    advance();
    expect(TokenType::SEMICOLON, ";");

    // reward
    expect(TokenType::IDENT, "reward");
    expect(TokenType::EQUAL, "=");
    expect(TokenType::INT, "int");
    node->reward = std::stoi(current.lexeme);
    advance();
    expect(TokenType::SEMICOLON, ";");

    expect(TokenType::RBRACE, "}");

    return node;
}

std::shared_ptr<TowerDecl> Parser::parseTowerDecl() {
    auto node = std::make_shared<TowerDecl>();

    expect(TokenType::IDENT, "tower name");
    node->name = current.lexeme;
    advance();

    expect(TokenType::LBRACE, "{");

    // range
    expect(TokenType::IDENT, "range");
    expect(TokenType::EQUAL, "=");
    expect(TokenType::INT, "int");
    node->range = std::stoi(current.lexeme);
    advance();
    expect(TokenType::SEMICOLON, ";");

    // damage
    expect(TokenType::IDENT, "damage");
    expect(TokenType::EQUAL, "=");
    expect(TokenType::INT, "int");
    node->damage = std::stoi(current.lexeme);
    advance();
    expect(TokenType::SEMICOLON, ";");

    // fire_rate
    expect(TokenType::IDENT, "fire_rate");
    expect(TokenType::EQUAL, "=");
    expect(TokenType::FLOAT, "float");
    node->fire_rate = std::stod(current.lexeme);
    advance();
    expect(TokenType::SEMICOLON, ";");

    // cost
    expect(TokenType::IDENT, "cost");
    expect(TokenType::EQUAL, "=");
    expect(TokenType::INT, "int");
    node->cost = std::stoi(current.lexeme);
    advance();
    expect(TokenType::SEMICOLON, ";");

    expect(TokenType::RBRACE, "}");

    return node;
}

std::shared_ptr<WaveDecl> Parser::parseWaveDecl() {
    auto node = std::make_shared<WaveDecl>();

    expect(TokenType::IDENT, "wave name");
    node->name = current.lexeme;
    advance();

    expect(TokenType::LBRACE, "{");

    while (match(TokenType::SPAWN)) {
        SpawnStmt s;

        expect(TokenType::LPAREN, "(");
        expect(TokenType::IDENT, "enemy type");
        s.enemyType = current.lexeme;
        advance();

        expect(TokenType::COMMA, ",");
        expect(TokenType::COUNT, "count");
        expect(TokenType::EQUAL, "=");
        expect(TokenType::INT, "int");
        s.count = std::stoi(current.lexeme);
        advance();

        expect(TokenType::COMMA, ",");
        expect(TokenType::START, "start");
        expect(TokenType::EQUAL, "=");
        expect(TokenType::INT, "int");
        s.start = std::stoi(current.lexeme);
        advance();

        expect(TokenType::COMMA, ",");
        expect(TokenType::INTERVAL, "interval");
        expect(TokenType::EQUAL, "=");
        expect(TokenType::INT, "int");
        s.interval = std::stoi(current.lexeme);
        advance();

        expect(TokenType::RPAREN, ")");
        expect(TokenType::SEMICOLON, ";");

        node->spawns.push_back(s);
    }

    expect(TokenType::RBRACE, "}");

    return node;
}

std::shared_ptr<PlaceStmt> Parser::parsePlaceStmt() {
    auto node = std::make_shared<PlaceStmt>();

    expect(TokenType::IDENT, "tower type");
    node->towerType = current.lexeme;
    advance();

    expect(TokenType::AT, "at");
    expect(TokenType::LPAREN, "(");

    expect(TokenType::INT, "x");
    node->x = std::stoi(current.lexeme);
    advance();

    expect(TokenType::COMMA, ",");

    expect(TokenType::INT, "y");
    node->y = std::stoi(current.lexeme);
    advance();

    expect(TokenType::RPAREN, ")");
    expect(TokenType::SEMICOLON, ";");

    return node;
}