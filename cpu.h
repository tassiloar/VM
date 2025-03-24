
#ifndef CPU_H
#define CPU_H

#include "bitpack.h"
#include "memory.h"
#include "arithmetic.h"
#include <stdint.h>
#include "assert.h"

struct Instruction
{
    int opcode;
    uint32_t rA, rB, rC;
    uint32_t value;
};

void decode_instruction(uint32_t instruction, struct Instruction *info);

void program_loop();

void set_pointer(int val);

#endif