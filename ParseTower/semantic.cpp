#include "semantic.h"
#include <iostream>
#include <set>

void SemanticAnalyzer::analyze(std::shared_ptr<Program> program) {
    for (auto decl : program->declarations) {
        if (auto m = dynamic_cast<MapDecl*>(decl.get())) {
            checkMap(m);
        }
        else if (auto e = dynamic_cast<EnemyDecl*>(decl.get())) {
            checkEnemy(e);
        }
        else if (auto t = dynamic_cast<TowerDecl*>(decl.get())) {
            checkTower(t);
        }
        else if (auto w = dynamic_cast<WaveDecl*>(decl.get())) {
            checkWave(w);
        }
        else if (auto p = dynamic_cast<PlaceStmt*>(decl.get())) {
            checkPlace(p);
        }
    }
}

void SemanticAnalyzer::checkMap(MapDecl* map) {
    if (maps.count(map->name)) {
        std::cerr << "Semantic Error: Duplicate map name " << map->name << "\n";
        exit(1);
    }
    maps[map->name] = map;
    currentMap = map;

    if (map->width <= 0 || map->height <= 0) {
        std::cerr << "Invalid map size.\n";
        exit(1);
    }

    // Validate path coordinates
    for (auto& p : map->path) {
        if (p.first < 0 || p.first >= map->width ||
            p.second < 0 || p.second >= map->height) {
            std::cerr << "Path coordinate out of map bounds.\n";
            exit(1);
        }
    }
}

void SemanticAnalyzer::checkEnemy(EnemyDecl* enemy) {
    if (enemies.count(enemy->name)) {
        std::cerr << "Duplicate enemy: " << enemy->name << "\n";
        exit(1);
    }
    enemies[enemy->name] = enemy;

    if (enemy->hp <= 0) {
        std::cerr << "Enemy HP invalid.\n";
        exit(1);
    }
    if (enemy->speed <= 0) {
        std::cerr << "Enemy speed invalid.\n";
        exit(1);
    }
    if (enemy->reward < 0) {
        std::cerr << "Enemy reward invalid.\n";
        exit(1);
    }
}

void SemanticAnalyzer::checkTower(TowerDecl* tower) {
    if (towers.count(tower->name)) {
        std::cerr << "Duplicate tower: " << tower->name << "\n";
        exit(1);
    }
    towers[tower->name] = tower;

    if (tower->range <= 0 || tower->damage <= 0 || tower->cost < 0) {
        std::cerr << "Invalid tower stats.\n";
        exit(1);
    }
    if (tower->fire_rate <= 0) {
        std::cerr << "Invalid fire rate.\n";
        exit(1);
    }
}

void SemanticAnalyzer::checkWave(WaveDecl* wave) {
    if (waves.count(wave->name)) {
        std::cerr << "Duplicate wave: " << wave->name << "\n";
        exit(1);
    }
    waves[wave->name] = wave;

    for (auto& s : wave->spawns) {
        if (!enemies.count(s.enemyType)) {
            std::cerr << "Wave uses undefined enemy: " << s.enemyType << "\n";
            exit(1);
        }
        if (s.count <= 0 || s.start < 0 || s.interval <= 0) {
            std::cerr << "Invalid spawn parameters.\n";
            exit(1);
        }
    }
}

void SemanticAnalyzer::checkPlace(PlaceStmt* place) {
    if (!towers.count(place->towerType)) {
        std::cerr << "Placing undefined tower type: " << place->towerType << "\n";
        exit(1);
    }

    if (!currentMap) {
        std::cerr << "Place statement appears before map definition.\n";
        exit(1);
    }

    if (place->x < 0 || place->x >= currentMap->width ||
        place->y < 0 || place->y >= currentMap->height) {
        std::cerr << "Tower placement out of map bounds.\n";
        exit(1);
    }
}
