#include "ir.h"
#include <sstream>

std::vector<IRInstruction> IRGenerator::generate(std::shared_ptr<Program> program) {
    code.clear(); // Clear previous IR

    for (const auto& decl : program->declarations) {

        if (auto m = dynamic_cast<MapDecl*>(decl.get())) {
            IRInstruction instr(IROpcode::DEFINE_MAP);
            instr.operands.push_back(m->name);
            instr.metadata["width"] = m->width;
            instr.metadata["height"] = m->height;
            
            // Store path as metadata
            std::stringstream pathStr;
            for (size_t i = 0; i < m->path.size(); i++) {
                pathStr << m->path[i].first << "," << m->path[i].second;
                if (i + 1 < m->path.size()) pathStr << ";";
            }
            instr.metadata["path"] = pathStr.str();
            emit(instr);
        }

        else if (auto e = dynamic_cast<EnemyDecl*>(decl.get())) {
            IRInstruction instr(IROpcode::DEFINE_ENEMY);
            instr.operands.push_back(e->name);
            instr.metadata["hp"] = e->hp;
            instr.metadata["speed"] = e->speed;
            instr.metadata["reward"] = e->reward;
            emit(instr);
        }

        else if (auto t = dynamic_cast<TowerDecl*>(decl.get())) {
            IRInstruction instr(IROpcode::DEFINE_TOWER);
            instr.operands.push_back(t->name);
            instr.metadata["range"] = t->range;
            instr.metadata["damage"] = t->damage;
            instr.metadata["fire_rate"] = t->fire_rate;
            instr.metadata["cost"] = t->cost;
            emit(instr);
        }

        else if (auto w = dynamic_cast<WaveDecl*>(decl.get())) {
            IRInstruction instr(IROpcode::DEFINE_WAVE);
            instr.operands.push_back(w->name);
            emit(instr);
            
            for (const auto& s : w->spawns) {
                IRInstruction spawnInstr(IROpcode::SPAWN_ENEMY);
                spawnInstr.operands.push_back(w->name);
                spawnInstr.operands.push_back(s.enemyType);
                spawnInstr.metadata["count"] = s.count;
                spawnInstr.metadata["start"] = s.start;
                spawnInstr.metadata["interval"] = s.interval;
                emit(spawnInstr);
            }
        }

        else if (auto p = dynamic_cast<PlaceStmt*>(decl.get())) {
            IRInstruction instr(IROpcode::PLACE_TOWER);
            instr.operands.push_back(p->towerType);
            instr.metadata["x"] = p->x;
            instr.metadata["y"] = p->y;
            emit(instr);
        }
    }

    return code;
}

std::vector<std::string> IRGenerator::toString(const std::vector<IRInstruction>& instructions) {
    std::vector<std::string> result;
    
    for (const auto& instr : instructions) {
        std::stringstream ss;
        
        switch (instr.opcode) {
            case IROpcode::DEFINE_MAP:
                ss << "DEFINE_MAP " << instr.operands[0];
                if (instr.metadata.count("width"))
                    ss << " WIDTH=" << std::get<int>(instr.metadata.at("width"));
                if (instr.metadata.count("height"))
                    ss << " HEIGHT=" << std::get<int>(instr.metadata.at("height"));
                if (instr.metadata.count("path"))
                    ss << " PATH=[" << std::get<std::string>(instr.metadata.at("path")) << "]";
                break;
                
            case IROpcode::DEFINE_ENEMY:
                ss << "DEFINE_ENEMY " << instr.operands[0];
                if (instr.metadata.count("hp"))
                    ss << " HP=" << std::get<int>(instr.metadata.at("hp"));
                if (instr.metadata.count("speed"))
                    ss << " SPEED=" << std::get<double>(instr.metadata.at("speed"));
                if (instr.metadata.count("reward"))
                    ss << " REWARD=" << std::get<int>(instr.metadata.at("reward"));
                break;
                
            case IROpcode::DEFINE_TOWER:
                ss << "DEFINE_TOWER " << instr.operands[0];
                if (instr.metadata.count("range"))
                    ss << " RANGE=" << std::get<int>(instr.metadata.at("range"));
                if (instr.metadata.count("damage"))
                    ss << " DAMAGE=" << std::get<int>(instr.metadata.at("damage"));
                if (instr.metadata.count("fire_rate"))
                    ss << " FIRERATE=" << std::get<double>(instr.metadata.at("fire_rate"));
                if (instr.metadata.count("cost"))
                    ss << " COST=" << std::get<int>(instr.metadata.at("cost"));
                break;
                
            case IROpcode::DEFINE_WAVE:
                ss << "DEFINE_WAVE " << instr.operands[0];
                break;
                
            case IROpcode::SPAWN_ENEMY:
                ss << "  SPAWN_ENEMY " << instr.operands[1] << " IN_WAVE=" << instr.operands[0];
                if (instr.metadata.count("count"))
                    ss << " COUNT=" << std::get<int>(instr.metadata.at("count"));
                if (instr.metadata.count("start"))
                    ss << " START=" << std::get<int>(instr.metadata.at("start"));
                if (instr.metadata.count("interval"))
                    ss << " INTERVAL=" << std::get<int>(instr.metadata.at("interval"));
                break;
                
            case IROpcode::PLACE_TOWER:
                ss << "PLACE_TOWER " << instr.operands[0];
                if (instr.metadata.count("x"))
                    ss << " X=" << std::get<int>(instr.metadata.at("x"));
                if (instr.metadata.count("y"))
                    ss << " Y=" << std::get<int>(instr.metadata.at("y"));
                break;
                
            case IROpcode::NOP:
                ss << "NOP";
                break;
                
            default:
                ss << "UNKNOWN_OPCODE";
        }
        
        result.push_back(ss.str());
    }
    
    return result;
}
