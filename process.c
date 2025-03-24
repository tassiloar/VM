
#include <stdint.h>
#include <cpu.h>

int instruction_decoder(uint32_t instruction, struct Instruction *info)
{
    info->opcode = (int)Bitpack_getu(instruction, 4, 29);

    if (info->opcode == 13)
    {
        info->rA = (int)Bitpack_getu(instruction, 3, 26);
        info->rB = -1;
        info->rC = -1;
        info->value = (int)Bitpack_getu(instruction, 25, 0);
    }
    else
    {
        info->rA = (int)Bitpack_getu(instruction, 3, 7);
        info->rB = (int)Bitpack_getu(instruction, 3, 4);
        info->rC = (int)Bitpack_getu(instruction, 3, 0);
        info->value = -1;
    }
}

int program_loop()
{
}
