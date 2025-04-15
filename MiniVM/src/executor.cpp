#include "executor.h"
#include "parser.h"
#include <iostream>

void executeInstruction(int instr, std::vector<int>& program, int& pc, int& accumulator, std::vector<int>& memory) {
    switch (instr) {
        case LOAD: accumulator = program[++pc]; break;
        case ADD:  accumulator += program[++pc]; break;
        case SUB:  accumulator -= program[++pc]; break;
        case MUL:  accumulator *= program[++pc]; break;
        case DIV:
            if (program[pc + 1] == 0) { std::cerr << "Divide by zero\n"; return; }
            accumulator /= program[++pc]; break;
        case STORE: memory[program[++pc]] = accumulator; break;
        case STOREM: memory[program[++pc]] = accumulator; break;
        case LOADM: accumulator = memory[program[++pc]]; break;
        case PRINT: std::cout << "Accumulator: " << accumulator << std::endl; break;
        case JMP: pc = program[++pc] - 1; break;
        case JZ: pc = (accumulator == 0) ? program[++pc] - 1 : pc + 1; break;
        case JNZ: pc = (accumulator != 0) ? program[++pc] - 1 : pc + 1; break;
        case HALT: std::cout << "Program halted.\n"; break;
        default: std::cerr << "Unknown instruction\n"; return;
    }
}
