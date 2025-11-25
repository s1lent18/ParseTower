#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "ir.h"
#include <vector>
#include <set>
#include <map>

class Optimizer {
public:
    // Main optimization entry point
    std::vector<IRInstruction> optimize(const std::vector<IRInstruction>& instructions);

private:
    // Optimization passes
    std::vector<IRInstruction> constantFolding(const std::vector<IRInstruction>& instructions);
    std::vector<IRInstruction> deadCodeElimination(const std::vector<IRInstruction>& instructions);
    std::vector<IRInstruction> duplicateDefinitionRemoval(const std::vector<IRInstruction>& instructions);
    std::vector<IRInstruction> redundantSpawnMerging(const std::vector<IRInstruction>& instructions);
    
    // Helper functions
    bool isDefinitionInstruction(IROpcode opcode);
    bool isRedundantInstruction(const IRInstruction& instr);
    std::string getDefinitionKey(const IRInstruction& instr);
};

#endif // OPTIMIZER_H

