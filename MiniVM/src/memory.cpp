#include "memory.h"

int accumulator = 0;
std::vector<int> memory;

void initializeMemory(int memorySize) {
    memory.resize(memorySize, 0);
}
