#include "parser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

Instruction getOpcode(const std::string& instr) {
    static std::unordered_map<std::string, Instruction> map = {
        {"HALT", HALT},
        {"LOAD", LOAD}, {"ADD", ADD}, {"SUB", SUB}, {"MUL", MUL}, {"DIV", DIV},
        {"STORE", STORE}, {"PRINT", PRINT},
        {"LOADM", LOADM}, {"STOREM", STOREM},
        {"JMP", JMP}, {"JZ", JZ}, {"JNZ", JNZ}
    };
    if (map.find(instr) != map.end()) {
        return map[instr];
    } else {
        std::cerr << "Unknown instruction: " << instr << std::endl;
        return HALT;  // Default to HALT if instruction is not found
    }
}

bool parseVMFile(const std::string& filename, std::vector<int>& program) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return false;
    }

    std::string line;
    std::unordered_map<std::string, int> labels;
    std::vector<std::pair<int, std::string>> unresolved;

    int address = 0;

    // Pass 1: Identify labels and build the program
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        if (!(iss >> token)) continue;  // Skip empty lines

        // Check for label
        if (token.back() == ':') {
            std::string label = token.substr(0, token.size() - 1);  // Remove ':'
            labels[label] = address;
            continue;
        }

        // Parse instruction
        program.push_back(getOpcode(token));
        address++;

        // Skip instructions that don't have arguments (e.g., PRINT, HALT)
        if (token == "PRINT" || token == "HALT") continue;

        std::string arg;
        if (iss >> arg) {
            try {
                program.push_back(std::stoi(arg));  // Add the argument
            } catch (...) {
                unresolved.push_back({address, arg});  // Store unresolved label
                program.push_back(0);  // Placeholder
            }
            address++;
        }
    }

    // Pass 2: Resolve labels
    for (auto& entry : unresolved) {
        int addr = entry.first;
        std::string label = entry.second;

        if (labels.find(label) != labels.end()) {
            program[addr] = labels[label];
        } else {
            std::cerr << "Unknown label: " << label << std::endl;
            return false;
        }
    }

    return true;
}
