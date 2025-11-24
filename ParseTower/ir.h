#ifndef IR_H
#define IR_H

#include "ast.h"
#include <vector>
#include <string>
#include <memory>

class IRGenerator {
public:
    // Generate intermediate code from AST
    std::vector<std::string> generate(std::shared_ptr<Program> program);

private:
    std::vector<std::string> code;

    // Add a line to the IR
    void emit(const std::string& line) { code.push_back(line); }
};

#endif // IR_H
