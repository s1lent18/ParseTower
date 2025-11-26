#include "parser.h"
#include <iostream>

Parser::Parser(Lexer& lx) : lexer(lx) {
    current = lexer.getNextToken();
}

void Parser::advance() {
    current = lexer.getNextToken();
}

// Returns true and advances if the current token matches
bool Parser::match(TokenType type) {
    if (current.type == type) {
        advance();
        return true;
    }
    return false;
}

// Expects a token of a certain type, advances, and returns it
Token Parser::expect(TokenType type, const std::string& msg) {
    if (current.type != type) {
        std::cerr << "Parser Error: expected " << msg
                  << " at line " << current.line << std::endl;
        exit(1);
    }
    Token t = current;
    advance(); // advance AFTER storing the token
    return t;
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
    Token nameTok = expect(TokenType::IDENT, "map name");
    node->name = nameTok.lexeme;

    expect(TokenType::LBRACE, "{");

    // size
    expect(TokenType::SIZE, "size");
    expect(TokenType::EQUAL, "=");
    expect(TokenType::LPAREN, "(");
    Token wTok = expect(TokenType::INT, "map width");
    node->width = std::stoi(wTok.lexeme);
    expect(TokenType::COMMA, ",");
    Token hTok = expect(TokenType::INT, "map height");
    node->height = std::stoi(hTok.lexeme);
    expect(TokenType::RPAREN, ")");
    expect(TokenType::SEMICOLON, ";");

    // path
    expect(TokenType::PATH, "path");
    expect(TokenType::EQUAL, "=");
    expect(TokenType::LBRACKET, "[");
    while (!match(TokenType::RBRACKET)) {
        expect(TokenType::LPAREN, "(");
        Token xTok = expect(TokenType::INT, "x coordinate");
        int x = std::stoi(xTok.lexeme);
        expect(TokenType::COMMA, ",");
        Token yTok = expect(TokenType::INT, "y coordinate");
        int y = std::stoi(yTok.lexeme);
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
    Token nameTok = expect(TokenType::IDENT, "enemy name");
    node->name = nameTok.lexeme;

    expect(TokenType::LBRACE, "{"); // Open brace for body

    // hp
    Token hpTok = expect(TokenType::IDENT, "hp");
    expect(TokenType::EQUAL, "=");
    Token hpValTok = expect(TokenType::INT, "hp value");
    node->hp = std::stoi(hpValTok.lexeme);
    expect(TokenType::SEMICOLON, ";");

    // speed
    Token speedTokName = expect(TokenType::IDENT, "speed");
    expect(TokenType::EQUAL, "=");
    Token speedValTok = expect(TokenType::FLOAT, "speed value");
    node->speed = std::stod(speedValTok.lexeme);
    expect(TokenType::SEMICOLON, ";");

    // reward
    Token rewardTokName = expect(TokenType::IDENT, "reward");
    expect(TokenType::EQUAL, "=");
    Token rewardValTok = expect(TokenType::INT, "reward value");
    node->reward = std::stoi(rewardValTok.lexeme);
    expect(TokenType::SEMICOLON, ";");

    expect(TokenType::RBRACE, "}"); // Close brace for enemy
    return node;
}

std::shared_ptr<TowerDecl> Parser::parseTowerDecl() {
    auto node = std::make_shared<TowerDecl>();
    Token nameTok = expect(TokenType::IDENT, "tower name");
    node->name = nameTok.lexeme;

    expect(TokenType::LBRACE, "{");

    Token rangeTokName = expect(TokenType::IDENT, "range");  // Should get 'range'
    expect(TokenType::EQUAL, "=");
    Token rangeTokVal = expect(TokenType::INT, "range value");
    node->range = std::stoi(rangeTokVal.lexeme);
    expect(TokenType::SEMICOLON, ";");

    Token dmgTokName = expect(TokenType::IDENT, "damage");
    expect(TokenType::EQUAL, "=");
    Token dmgTokVal = expect(TokenType::INT, "damage value");
    node->damage = std::stoi(dmgTokVal.lexeme);
    expect(TokenType::SEMICOLON, ";");

    Token frTokName = expect(TokenType::IDENT, "fire_rate");
    expect(TokenType::EQUAL, "=");
    Token frTokVal = expect(TokenType::FLOAT, "fire_rate value");
    node->fire_rate = std::stod(frTokVal.lexeme);
    expect(TokenType::SEMICOLON, ";");

    Token costTokName = expect(TokenType::IDENT, "cost");
    expect(TokenType::EQUAL, "=");
    Token costTokVal = expect(TokenType::INT, "cost value");
    node->cost = std::stoi(costTokVal.lexeme);
    expect(TokenType::SEMICOLON, ";");

    expect(TokenType::RBRACE, "}");
    return node;
}


std::shared_ptr<WaveDecl> Parser::parseWaveDecl() {
    auto node = std::make_shared<WaveDecl>();
    Token nameTok = expect(TokenType::IDENT, "wave name");
    node->name = nameTok.lexeme;

    expect(TokenType::LBRACE, "{");

    while (match(TokenType::SPAWN)) {
        SpawnStmt s;
        expect(TokenType::LPAREN, "(");

        Token eTok = expect(TokenType::IDENT, "enemy type");
        s.enemyType = eTok.lexeme;

        expect(TokenType::COMMA, ",");
        expect(TokenType::COUNT, "count");
        expect(TokenType::EQUAL, "=");
        Token cTok = expect(TokenType::INT, "count");
        s.count = std::stoi(cTok.lexeme);

        expect(TokenType::COMMA, ",");
        expect(TokenType::START, "start");
        expect(TokenType::EQUAL, "=");
        Token startTok = expect(TokenType::INT, "start");
        s.start = std::stoi(startTok.lexeme);

        expect(TokenType::COMMA, ",");
        expect(TokenType::INTERVAL, "interval");
        expect(TokenType::EQUAL, "=");
        Token intvTok = expect(TokenType::INT, "interval");
        s.interval = std::stoi(intvTok.lexeme);

        expect(TokenType::RPAREN, ")");
        expect(TokenType::SEMICOLON, ";");

        node->spawns.push_back(s);
    }

    expect(TokenType::RBRACE, "}");
    return node;
}

std::shared_ptr<PlaceStmt> Parser::parsePlaceStmt() {
    auto node = std::make_shared<PlaceStmt>();
    Token tTok = expect(TokenType::IDENT, "tower type");
    node->towerType = tTok.lexeme;

    expect(TokenType::AT, "at");
    expect(TokenType::LPAREN, "(");

    Token xTok = expect(TokenType::INT, "x coordinate");
    node->x = std::stoi(xTok.lexeme);

    expect(TokenType::COMMA, ",");

    Token yTok = expect(TokenType::INT, "y coordinate");
    node->y = std::stoi(yTok.lexeme);

    expect(TokenType::RPAREN, ")");
    expect(TokenType::SEMICOLON, ";");

    return node;
}