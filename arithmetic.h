

#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include "memory.h"
#include "assert.h"
#include "cpu.h"
#include <stdio.h>
#include <stdint.h>

void execute(uint32_t opcode, uint32_t *rA, uint32_t *rB, uint32_t *rC,
             uint32_t value);

#endif