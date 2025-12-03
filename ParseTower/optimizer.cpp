#include "optimizer.h"
#include <iostream>
#include <algorithm>

std::vector<IRInstruction> Optimizer::optimize(const std::vector<IRInstruction>& instructions) {
    auto result = instructions;
    
    // Apply optimization passes in sequence
    std::cout << "Running optimization passes...\n";
    
    // Pass 1: Remove duplicate definitions (keep first occurrence)
    result = duplicateDefinitionRemoval(result);
    
    // Pass 2: Merge redundant spawns in same wave
    result = redundantSpawnMerging(result);
    
    // Pass 3: Constant folding (for any computed values)
    result = constantFolding(result);
    
    // Pass 4: Dead code elimination
    result = deadCodeElimination(result);
    
    std::cout << "Optimization complete.\n";
    return result;
}

std::vector<IRInstruction> Optimizer::constantFolding(const std::vector<IRInstruction>& instructions) {
    std::vector<IRInstruction> optimized;
    
    for (const auto& instr : instructions) {
        IRInstruction newInstr = instr;
        
        // Optimize constant expressions in metadata
        // For tower defense game, we can pre-calculate DPS, total wave spawn times, etc.
        if (instr.opcode == IROpcode::DEFINE_TOWER) {
            // Calculate and store DPS (Damage Per Second)
            if (instr.metadata.count("damage") && instr.metadata.count("fire_rate")) {
                int damage = std::get<int>(instr.metadata.at("damage"));
                double fireRate = std::get<double>(instr.metadata.at("fire_rate"));
                double dps = damage * fireRate;
                newInstr.metadata["dps"] = dps;
            }
        }
        
        if (instr.opcode == IROpcode::SPAWN_ENEMY) {
            // Calculate total spawn duration
            if (instr.metadata.count("count") && instr.metadata.count("interval")) {
                int count = std::get<int>(instr.metadata.at("count"));
                int interval = std::get<int>(instr.metadata.at("interval"));
                int totalDuration = count * interval;
                newInstr.metadata["total_duration"] = totalDuration;
            }
        }
        
        optimized.push_back(newInstr);
    }
    
    return optimized;
}

std::vector<IRInstruction> Optimizer::deadCodeElimination(const std::vector<IRInstruction>& instructions) {
    std::vector<IRInstruction> optimized;
    std::set<std::string> referencedEnemies;
    std::set<std::string> referencedTowers;
    std::set<std::string> definedEnemies;
    std::set<std::string> definedTowers;
    
    // First pass: collect all references and definitions
    for (const auto& instr : instructions) {
        if (instr.opcode == IROpcode::DEFINE_ENEMY && !instr.operands.empty()) {
            definedEnemies.insert(instr.operands[0]);
        }
        if (instr.opcode == IROpcode::DEFINE_TOWER && !instr.operands.empty()) {
            definedTowers.insert(instr.operands[0]);
        }
        if (instr.opcode == IROpcode::SPAWN_ENEMY && instr.operands.size() > 1) {
            referencedEnemies.insert(instr.operands[1]);
        }
        if (instr.opcode == IROpcode::PLACE_TOWER && !instr.operands.empty()) {
            referencedTowers.insert(instr.operands[0]);
        }
    }
    
    // Second pass: keep only referenced definitions
    for (const auto& instr : instructions) {
        bool keep = true;
        
        // Remove unreferenced enemy definitions
        if (instr.opcode == IROpcode::DEFINE_ENEMY && !instr.operands.empty()) {
            if (referencedEnemies.find(instr.operands[0]) == referencedEnemies.end()) {
                std::cout << "  DCE: Removing unreferenced enemy: " << instr.operands[0] << "\n";
                keep = false;
            }
        }
        
        // Remove unreferenced tower definitions
        if (instr.opcode == IROpcode::DEFINE_TOWER && !instr.operands.empty()) {
            if (referencedTowers.find(instr.operands[0]) == referencedTowers.end()) {
                std::cout << "  DCE: Removing unreferenced tower: " << instr.operands[0] << "\n";
                keep = false;
            }
        }
        
        // Skip NOP instructions
        if (instr.opcode == IROpcode::NOP) {
            keep = false;
        }
        
        if (keep) {
            optimized.push_back(instr);
        }
    }
    
    return optimized;
}

std::vector<IRInstruction> Optimizer::duplicateDefinitionRemoval(const std::vector<IRInstruction>& instructions) {
    std::vector<IRInstruction> optimized;
    std::set<std::string> seenDefinitions;
    
    for (const auto& instr : instructions) {
        bool keep = true;
        
        if (isDefinitionInstruction(instr.opcode) && !instr.operands.empty()) {
            std::string key = getDefinitionKey(instr);
            
            if (seenDefinitions.find(key) != seenDefinitions.end()) {
                std::cout << "  Optimization: Removing duplicate definition: " << key << "\n";
                keep = false;
            } else {
                seenDefinitions.insert(key);
            }
        }
        
        if (keep) {
            optimized.push_back(instr);
        }
    }
    
    return optimized;
}

std::vector<IRInstruction> Optimizer::redundantSpawnMerging(const std::vector<IRInstruction>& instructions) {
    std::vector<IRInstruction> optimized;
    std::map<std::string, size_t> spawnGroupIndex; // Key -> index in optimized
    
    for (const auto& instr : instructions) {
        if (instr.opcode == IROpcode::SPAWN_ENEMY && instr.operands.size() >= 2) {
            std::string wave = instr.operands[0];
            std::string enemy = instr.operands[1];
            int start = std::get<int>(instr.metadata.at("start"));
            int interval = std::get<int>(instr.metadata.at("interval"));
            
            std::string key = wave + "_" + enemy + "_" + std::to_string(start) + "_" + std::to_string(interval);
            
            if (spawnGroupIndex.find(key) != spawnGroupIndex.end()) {
                // Merge: update existing spawn in-place
                size_t idx = spawnGroupIndex[key];
                int existingCount = std::get<int>(optimized[idx].metadata.at("count"));
                int newCount = std::get<int>(instr.metadata.at("count"));
                optimized[idx].metadata["count"] = existingCount + newCount;
                std::cout << "  Optimization: Merged redundant spawn in wave " << wave << "\n";
            } else {
                spawnGroupIndex[key] = optimized.size();
                optimized.push_back(instr);
            }
        } else {
            optimized.push_back(instr);
        }
    }
    
    return optimized;
}

bool Optimizer::isDefinitionInstruction(IROpcode opcode) {
    return opcode == IROpcode::DEFINE_MAP ||
           opcode == IROpcode::DEFINE_ENEMY ||
           opcode == IROpcode::DEFINE_TOWER ||
           opcode == IROpcode::DEFINE_WAVE;
}

std::string Optimizer::getDefinitionKey(const IRInstruction& instr) {
    std::string prefix;
    switch (instr.opcode) {
        case IROpcode::DEFINE_MAP: prefix = "MAP:"; break;
        case IROpcode::DEFINE_ENEMY: prefix = "ENEMY:"; break;
        case IROpcode::DEFINE_TOWER: prefix = "TOWER:"; break;
        case IROpcode::DEFINE_WAVE: prefix = "WAVE:"; break;
        default: prefix = "UNKNOWN:"; break;
    }
    return prefix + (instr.operands.empty() ? "" : instr.operands[0]);
}

