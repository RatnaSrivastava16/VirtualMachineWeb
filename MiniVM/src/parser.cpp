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
    return map[instr];
}

bool parseVMFile(const std::string& filename, std::vector<int>& program) {
    std::ifstream file(filename);
    std::string line;
    std::unordered_map<std::string, int> labels;
    std::vector<std::pair<int, std::string>> unresolved;

    int address = 0;

    // Pass 1: Find labels and build partial program
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        if (!(iss >> token)) continue;

        if (token.back() == ':') {
            std::string label = token.substr(0, token.size() - 1);
            labels[label] = address;
            continue;
        }

        program.push_back(getOpcode(token));
        address++;

        if (token == "PRINT" || token == "HALT") continue;

        std::string arg;
        if (iss >> arg) {
            try {
                program.push_back(std::stoi(arg));
            } catch (...) {
                unresolved.push_back({address, arg});
                program.push_back(0); // placeholder
            }
            address++;
        }
    }

    // Pass 2: Resolve labels
    for (auto it = unresolved.begin(); it != unresolved.end(); ++it) {
        int addr = it->first;
        std::string label = it->second;

        if (labels.find(label) != labels.end()) {
            program[addr] = labels[label];
        } else {
            std::cerr << "Unknown label: " << label << std::endl;
            return false;
        }
    }

    return true;
}
