#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>

struct ASTNode {
    virtual ~ASTNode() {}
};

struct MapDecl : ASTNode {
    std::string name;
    int width, height;
    std::vector<std::pair<int,int>> path;
};

struct EnemyDecl : ASTNode {
    std::string name;
    int hp;
    double speed;
    int reward;
};

struct TowerDecl : ASTNode {
    std::string name;
    int range, damage, cost;
    double fire_rate;
};

struct SpawnStmt {
    std::string enemyType;
    int count;
    int start;
    int interval;
};

struct WaveDecl : ASTNode {
    std::string name;
    std::vector<SpawnStmt> spawns;
};

struct PlaceStmt : ASTNode {
    std::string towerType;
    int x;
    int y;
};

struct Program : ASTNode {
    std::vector<std::shared_ptr<ASTNode>> declarations;
};

#endif
