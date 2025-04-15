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

    if (!parseVMFile(filename, program)) {
        std::cerr << "Failed to load program: " << filename << "\n";
        return 1;
    }

    while (pc < program.size()) {
        int instr = program[pc];
        switch (instr) {
            case LOAD:   accumulator = program[++pc]; break;
            case ADD:    accumulator += program[++pc]; break;
            case SUB:    accumulator -= program[++pc]; break;
            case MUL:    accumulator *= program[++pc]; break;
            case DIV:
                if (program[pc + 1] == 0) {
                    std::cerr << "Runtime Error: Divide by zero\n";
                    return 1;
                }
                accumulator /= program[++pc];
                break;
            case STORE:
            case STOREM: memory[program[++pc]] = accumulator; break;
            case LOADM:  accumulator = memory[program[++pc]]; break;
            case PRINT:  std::cout << accumulator << std::endl; break;
            case JMP:    pc = program[++pc] - 1; break;
            case JZ:     pc = (accumulator == 0) ? program[++pc] - 1 : pc + 1; break;
            case JNZ:    pc = (accumulator != 0) ? program[++pc] - 1 : pc + 1; break;
            case HALT:
                std::cout << "Program halted.\n";
                return 0;
            default:
                std::cerr << "Unknown instruction at pc = " << pc << ": " << instr << "\n";
                return 1;
        }
        pc++;
    }

    return 0;
}
