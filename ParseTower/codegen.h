#ifndef CODEGEN_H
#define CODEGEN_H

#include "ir.h"
#include <string>
#include <vector>

class CodeGenerator {
public:
    // Generate final code from optimized IR
    std::string generateJSON(const std::vector<IRInstruction>& instructions);
    
    // Alternative output formats
    std::string generateReadable(const std::vector<IRInstruction>& instructions);
    
private:
    // Helper functions for JSON generation
    std::string escapeJSON(const std::string& str);
    std::string generateMapJSON(const IRInstruction& instr);
    std::string generateEnemyJSON(const IRInstruction& instr);
    std::string generateTowerJSON(const IRInstruction& instr);
    std::string generateWaveJSON(const std::vector<IRInstruction>& instructions, size_t& index);
    std::string generatePlacementJSON(const IRInstruction& instr);
};

#endif // CODEGEN_H

