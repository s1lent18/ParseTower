#include "ir.h"
#include <sstream>

std::vector<std::string> IRGenerator::generate(std::shared_ptr<Program> program) {
    code.clear(); // Clear previous IR

    for (const auto& decl : program->declarations) {

        if (auto m = dynamic_cast<MapDecl*>(decl.get())) {
            std::stringstream ss;
            ss << "MAP " << m->name << " SIZE (" << m->width << "," << m->height << ") PATH [";
            for (size_t i = 0; i < m->path.size(); i++) {
                ss << "(" << m->path[i].first << "," << m->path[i].second << ")";
                if (i + 1 < m->path.size()) ss << ", ";
            }
            ss << "]";
            emit(ss.str());
        }

        else if (auto e = dynamic_cast<EnemyDecl*>(decl.get())) {
            std::stringstream ss;
            ss << "ENEMY " << e->name
               << " HP=" << e->hp
               << " SPEED=" << e->speed
               << " REWARD=" << e->reward;
            emit(ss.str());
        }

        else if (auto t = dynamic_cast<TowerDecl*>(decl.get())) {
            std::stringstream ss;
            ss << "TOWER " << t->name
               << " RANGE=" << t->range
               << " DAMAGE=" << t->damage
               << " FIRERATE=" << t->fire_rate
               << " COST=" << t->cost;
            emit(ss.str());
        }

        else if (auto w = dynamic_cast<WaveDecl*>(decl.get())) {
            emit("WAVE " + w->name);
            for (const auto& s : w->spawns) {
                std::stringstream ss;
                ss << "  SPAWN " << s.enemyType
                   << " COUNT=" << s.count
                   << " START=" << s.start
                   << " INTERVAL=" << s.interval;
                emit(ss.str());
            }
        }

        else if (auto p = dynamic_cast<PlaceStmt*>(decl.get())) {
            std::stringstream ss;
            ss << "PLACE " << p->towerType
               << " AT (" << p->x << "," << p->y << ")";
            emit(ss.str());
        }
    }

    return code;
}
