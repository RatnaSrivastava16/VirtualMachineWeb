#pragma once
#include <unordered_map>
#include <string>
#include <vector>

enum Instruction {
    HALT = 0,
    LOAD, ADD, SUB, MUL, DIV,
    STORE, PRINT,
    LOADM, STOREM,
    JMP, JZ, JNZ
};

Instruction getOpcode(const std::string& instr);

bool parseVMFile(const std::string& filename, std::vector<int>& program);
