#include <iostream>
#include <vector>
#include <string>
#include "parser.h"

// Define instruction set (if not defined in parser.h)
enum Instructions {
    LOAD = 1, ADD, SUB, MUL, DIV,
    STORE, STOREM, LOADM, PRINT,
    JMP, JZ, JNZ, HALT
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <program_file.vm>\n";
        return 1;
    }

    std::string filename = argv[1];
    std::vector<int> memory(100, 0);
    std::vector<int> program;
    int accumulator = 0, pc = 0;

    std::cout << "[DEBUG] Loading program: " << filename << "\n";

    if (!parseVMFile(filename, program)) {
        std::cerr << "Failed to load program: " << filename << "\n";
        return 1;
    }

    std::cout << "[DEBUG] Program loaded successfully. Executing...\n";

    while (pc < program.size()) {
        int instr = program[pc];
        std::cout << "[DEBUG] PC: " << pc << " Instruction: " << instr << " Acc: " << accumulator << "\n";

        switch (instr) {
            case LOAD:
                accumulator = program[++pc];
                std::cout << "[DEBUG] LOAD " << accumulator << "\n";
                break;
            case ADD:
                accumulator += program[++pc];
                std::cout << "[DEBUG] ADD -> " << accumulator << "\n";
                break;
            case SUB:
                accumulator -= program[++pc];
                std::cout << "[DEBUG] SUB -> " << accumulator << "\n";
                break;
            case MUL:
                accumulator *= program[++pc];
                std::cout << "[DEBUG] MUL -> " << accumulator << "\n";
                break;
            case DIV:
                if (program[pc + 1] == 0) {
                    std::cerr << "Runtime Error: Divide by zero\n";
                    return 1;
                }
                accumulator /= program[++pc];
                std::cout << "[DEBUG] DIV -> " << accumulator << "\n";
                break;
            case STORE:
            case STOREM:
                memory[program[++pc]] = accumulator;
                std::cout << "[DEBUG] STORE to memory[" << program[pc] << "] = " << accumulator << "\n";
                break;
            case LOADM:
                accumulator = memory[program[++pc]];
                std::cout << "[DEBUG] LOADM from memory[" << program[pc] << "] -> " << accumulator << "\n";
                break;
            case PRINT:
                std::cout << "[OUTPUT] " << accumulator << std::endl;
                break;
            case JMP:
                pc = program[++pc] - 1;
                std::cout << "[DEBUG] JMP to " << pc + 1 << "\n";
                break;
            case JZ:
                if (accumulator == 0) {
                    pc = program[++pc] - 1;
                    std::cout << "[DEBUG] JZ taken to " << pc + 1 << "\n";
                } else {
                    pc++;
                    std::cout << "[DEBUG] JZ skipped\n";
                }
                break;
            case JNZ:
                if (accumulator != 0) {
                    pc = program[++pc] - 1;
                    std::cout << "[DEBUG] JNZ taken to " << pc + 1 << "\n";
                } else {
                    pc++;
                    std::cout << "[DEBUG] JNZ skipped\n";
                }
                break;
            case HALT:
                std::cout << "[DEBUG] Program halted.\n";
                return 0;
            default:
                std::cerr << "Unknown instruction at pc = " << pc << ": " << instr << "\n";
                return 1;
        }

        pc++;
    }

    return 0;
}
