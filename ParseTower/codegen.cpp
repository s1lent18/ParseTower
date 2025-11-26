#include "codegen.h"
#include <sstream>
#include <iomanip>

std::string CodeGenerator::escapeJSON(const std::string& str) {
    std::ostringstream escaped;
    for (char c : str) {
        switch (c) {
            case '"': escaped << "\\\""; break;
            case '\\': escaped << "\\\\"; break;
            case '\n': escaped << "\\n"; break;
            case '\r': escaped << "\\r"; break;
            case '\t': escaped << "\\t"; break;
            default: escaped << c; break;
        }
    }
    return escaped.str();
}

std::string CodeGenerator::generateMapJSON(const IRInstruction& instr) {
    std::ostringstream json;
    json << "    \"map\": {\n";
    json << "      \"name\": \"" << escapeJSON(instr.operands[0]) << "\",\n";
    
    if (instr.metadata.count("width")) {
        json << "      \"width\": " << std::get<int>(instr.metadata.at("width")) << ",\n";
    }
    if (instr.metadata.count("height")) {
        json << "      \"height\": " << std::get<int>(instr.metadata.at("height")) << ",\n";
    }
    
    if (instr.metadata.count("path")) {
        std::string pathStr = std::get<std::string>(instr.metadata.at("path"));
        json << "      \"path\": [\n";
        
        std::istringstream pathStream(pathStr);
        std::string coord;
        bool first = true;
        
        while (std::getline(pathStream, coord, ';')) {
            if (!first) json << ",\n";
            first = false;
            
            size_t commaPos = coord.find(',');
            if (commaPos != std::string::npos) {
                int x = std::stoi(coord.substr(0, commaPos));
                int y = std::stoi(coord.substr(commaPos + 1));
                json << "        {\"x\": " << x << ", \"y\": " << y << "}";
            }
        }
        
        json << "\n      ]\n";
    }
    
    json << "    }";
    return json.str();
}

std::string CodeGenerator::generateEnemyJSON(const IRInstruction& instr) {
    std::ostringstream json;
    json << "      {\n";
    json << "        \"name\": \"" << escapeJSON(instr.operands[0]) << "\",\n";
    
    if (instr.metadata.count("hp")) {
        json << "        \"hp\": " << std::get<int>(instr.metadata.at("hp")) << ",\n";
    }
    if (instr.metadata.count("speed")) {
        json << "        \"speed\": " << std::fixed << std::setprecision(2) 
             << std::get<double>(instr.metadata.at("speed")) << ",\n";
    }
    if (instr.metadata.count("reward")) {
        json << "        \"reward\": " << std::get<int>(instr.metadata.at("reward")) << "\n";
    }
    
    json << "      }";
    return json.str();
}

std::string CodeGenerator::generateTowerJSON(const IRInstruction& instr) {
    std::ostringstream json;
    json << "      {\n";
    json << "        \"name\": \"" << escapeJSON(instr.operands[0]) << "\",\n";
    
    if (instr.metadata.count("range")) {
        json << "        \"range\": " << std::get<int>(instr.metadata.at("range")) << ",\n";
    }
    if (instr.metadata.count("damage")) {
        json << "        \"damage\": " << std::get<int>(instr.metadata.at("damage")) << ",\n";
    }
    if (instr.metadata.count("fire_rate")) {
        json << "        \"fireRate\": " << std::fixed << std::setprecision(2)
             << std::get<double>(instr.metadata.at("fire_rate")) << ",\n";
    }
    if (instr.metadata.count("cost")) {
        json << "        \"cost\": " << std::get<int>(instr.metadata.at("cost"));
    }
    
    // Include optimized DPS if available
    if (instr.metadata.count("dps")) {
        json << ",\n        \"dps\": " << std::fixed << std::setprecision(2)
             << std::get<double>(instr.metadata.at("dps"));
    }
    
    json << "\n      }";
    return json.str();
}

std::string CodeGenerator::generateWaveJSON(const std::vector<IRInstruction>& instructions, size_t& index) {
    std::ostringstream json;
    const IRInstruction& waveInstr = instructions[index];
    
    json << "      {\n";
    json << "        \"name\": \"" << escapeJSON(waveInstr.operands[0]) << "\",\n";
    json << "        \"spawns\": [\n";
    
    bool firstSpawn = true;
    size_t i = index + 1;
    
    // Collect all spawns for this wave
    while (i < instructions.size() && instructions[i].opcode == IROpcode::SPAWN_ENEMY) {
        const IRInstruction& spawnInstr = instructions[i];
        
        if (!firstSpawn) json << ",\n";
        firstSpawn = false;
        
        json << "          {\n";
        json << "            \"enemyType\": \"" << escapeJSON(spawnInstr.operands[1]) << "\",\n";
        
        if (spawnInstr.metadata.count("count")) {
            json << "            \"count\": " << std::get<int>(spawnInstr.metadata.at("count")) << ",\n";
        }
        if (spawnInstr.metadata.count("start")) {
            json << "            \"start\": " << std::get<int>(spawnInstr.metadata.at("start")) << ",\n";
        }
        if (spawnInstr.metadata.count("interval")) {
            json << "            \"interval\": " << std::get<int>(spawnInstr.metadata.at("interval"));
        }
        
        // Include optimized total duration if available
        if (spawnInstr.metadata.count("total_duration")) {
            json << ",\n            \"totalDuration\": " << std::get<int>(spawnInstr.metadata.at("total_duration"));
        }
        
        json << "\n          }";
        i++;
    }
    
    json << "\n        ]\n";
    json << "      }";
    
    index = i - 1; // Update index to last processed spawn
    return json.str();
}

std::string CodeGenerator::generatePlacementJSON(const IRInstruction& instr) {
    std::ostringstream json;
    json << "      {\n";
    json << "        \"towerType\": \"" << escapeJSON(instr.operands[0]) << "\",\n";
    
    if (instr.metadata.count("x")) {
        json << "        \"x\": " << std::get<int>(instr.metadata.at("x")) << ",\n";
    }
    if (instr.metadata.count("y")) {
        json << "        \"y\": " << std::get<int>(instr.metadata.at("y")) << "\n";
    }
    
    json << "      }";
    return json.str();
}

std::string CodeGenerator::generateJSON(const std::vector<IRInstruction>& instructions) {
    std::ostringstream json;
    
    json << "{\n";
    json << "  \"gameConfig\": {\n";
    
    bool hasMap = false;
    std::vector<size_t> enemyIndices;
    std::vector<size_t> towerIndices;
    std::vector<size_t> waveIndices;
    std::vector<size_t> placementIndices;
    
    // Categorize instructions
    for (size_t i = 0; i < instructions.size(); i++) {
        switch (instructions[i].opcode) {
            case IROpcode::DEFINE_MAP:
                if (!hasMap) {
                    json << generateMapJSON(instructions[i]);
                    hasMap = true;
                }
                break;
            case IROpcode::DEFINE_ENEMY:
                enemyIndices.push_back(i);
                break;
            case IROpcode::DEFINE_TOWER:
                towerIndices.push_back(i);
                break;
            case IROpcode::DEFINE_WAVE:
                waveIndices.push_back(i);
                break;
            case IROpcode::PLACE_TOWER:
                placementIndices.push_back(i);
                break;
            default:
                break;
        }
    }
    
    // Generate enemies array
    if (!enemyIndices.empty()) {
        if (hasMap) json << ",\n";
        json << "    \"enemies\": [\n";
        
        for (size_t i = 0; i < enemyIndices.size(); i++) {
            json << generateEnemyJSON(instructions[enemyIndices[i]]);
            if (i + 1 < enemyIndices.size()) json << ",";
            json << "\n";
        }
        
        json << "    ]";
    }
    
    // Generate towers array
    if (!towerIndices.empty()) {
        if (hasMap || !enemyIndices.empty()) json << ",\n";
        json << "    \"towers\": [\n";
        
        for (size_t i = 0; i < towerIndices.size(); i++) {
            json << generateTowerJSON(instructions[towerIndices[i]]);
            if (i + 1 < towerIndices.size()) json << ",";
            json << "\n";
        }
        
        json << "    ]";
    }
    
    // Generate waves array
    if (!waveIndices.empty()) {
        if (hasMap || !enemyIndices.empty() || !towerIndices.empty()) json << ",\n";
        json << "    \"waves\": [\n";
        
        for (size_t i = 0; i < waveIndices.size(); i++) {
            size_t idx = waveIndices[i];
            json << generateWaveJSON(instructions, idx);
            if (i + 1 < waveIndices.size()) json << ",";
            json << "\n";
        }
        
        json << "    ]";
    }
    
    // Generate placements array
    if (!placementIndices.empty()) {
        if (hasMap || !enemyIndices.empty() || !towerIndices.empty() || !waveIndices.empty()) {
            json << ",\n";
        }
        json << "    \"initialPlacements\": [\n";
        
        for (size_t i = 0; i < placementIndices.size(); i++) {
            json << generatePlacementJSON(instructions[placementIndices[i]]);
            if (i + 1 < placementIndices.size()) json << ",";
            json << "\n";
        }
        
        json << "    ]";
    }
    
    json << "\n  }\n";
    json << "}\n";
    
    return json.str();
}

std::string CodeGenerator::generateReadable(const std::vector<IRInstruction>& instructions) {
    IRGenerator irGen;
    std::vector<std::string> lines = irGen.toString(instructions);
    
    std::ostringstream result;
    result << "=== ParseTower Compiled Output ===\n\n";
    
    for (const auto& line : lines) {
        result << line << "\n";
    }
    
    return result.str();
}

