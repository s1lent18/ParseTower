#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include <unordered_map>
#include <string>

class SemanticAnalyzer {
public:
    void analyze(std::shared_ptr<Program> program);

private:
    std::unordered_map<std::string, MapDecl*> maps;
    std::unordered_map<std::string, EnemyDecl*> enemies;
    std::unordered_map<std::string, TowerDecl*> towers;
    std::unordered_map<std::string, WaveDecl*> waves;

    MapDecl* currentMap = nullptr;

    void checkMap(MapDecl* map);
    void checkEnemy(EnemyDecl* enemy);
    void checkTower(TowerDecl* tower);
    void checkWave(WaveDecl* wave);
    void checkPlace(PlaceStmt* place);
};

#endif
