#ifndef IR_H
#define IR_H

#include "ast.h"
#include <vector>
#include <string>
#include <memory>
#include <variant>
#include <map>

// IR Instruction types
enum class IROpcode {
    DEFINE_MAP,
    DEFINE_ENEMY,
    DEFINE_TOWER,
    DEFINE_WAVE,
    SPAWN_ENEMY,
    PLACE_TOWER,
    SET_VALUE,
    LOAD_CONST,
    NOP  // No operation (for dead code elimination)
};

// IR Instruction structure
struct IRInstruction {
    IROpcode opcode;
    std::vector<std::string> operands;
    std::map<std::string, std::variant<int, double, std::string>> metadata;
    
    IRInstruction() : opcode(IROpcode::NOP) {}
    IRInstruction(IROpcode op) : opcode(op) {}
};

class IRGenerator {
public:
    // Generate intermediate code from AST
    std::vector<IRInstruction> generate(std::shared_ptr<Program> program);
    
    // Convert IR instructions to readable format
    std::vector<std::string> toString(const std::vector<IRInstruction>& instructions);

private:
    std::vector<IRInstruction> code;
    
    // Add an instruction to the IR
    void emit(const IRInstruction& instr) { code.push_back(instr); }
};

#endif // IR_H
