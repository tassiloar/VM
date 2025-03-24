
#include "cpu.h"

/*
 * Instruction
 * contains opcode and all registers used
 * holds value as well
 */

/* Holds all register values */
uint32_t registers[8];

/* dictates where the next command will be looked for in memory */
int program_counter;

/*                          decode_instruction
 *  Purpose:    take in a word and an instruction and fill the instruction
 *              struct with all the information needed
 *  Arguments:  take in a word and a struct of information to fill
 *  Returns:    none
 */
void decode_instruction(uint32_t instruction, struct Instruction *info)
{
    info->opcode = (int)Bitpack_getu(instruction, 4, 28);
    assert(info->opcode >= 0 && info->opcode <= 13);
    if (info->opcode == 13)
    {
        info->rA = (int)Bitpack_getu(instruction, 3, 25);
        info->rB = -1;
        info->rC = -1;
        info->value = (int)Bitpack_getu(instruction, 25, 0);
    }
    else
    {
        info->rA = (int)Bitpack_getu(instruction, 3, 6);
        info->rB = (int)Bitpack_getu(instruction, 3, 3);
        assert(info->rB <= 7);
        info->rC = (int)Bitpack_getu(instruction, 3, 0);
        assert(info->rC <= 7);
        info->value = -1;
    }
    assert(info->rA <= 7);
}

/*                          print_registers
 *  Purpose:    testing function, prints all registers to standard output
 *  Arguments:  none
 *  Returns:    none
 */
void print_registers()
{
    printf("REGISTERS: \n");
    for (int i = 0; i < 8; i++)
    {
        printf("r[%i] = %i\n", i, registers[i]);
    }
}

/*                          program_loop
 *  Purpose:    iterate through the program, execute instructions as needed
 *              halt loop if pointer is ever -1
 *  Arguments:  none
 *  Returns:    none
 */
void program_loop()
{
    program_counter = 0;
    while (true)
    {
        int seg_size;
        get_seg(0, &seg_size);

        if (program_counter <= -1 || program_counter >= seg_size)
        {
            break;
        }
        assert(program_counter >= 0 && program_counter < seg_size);
        uint32_t *current_instruction = get_mem(0, program_counter);
        struct Instruction i;
        decode_instruction(*current_instruction, &i);
        execute(i.opcode, &registers[i.rA], &registers[i.rB],
                &registers[i.rC], i.value);
        program_counter++;
        /* print_registers(); */
    }
    /* print_memory(); */
}

/*                          set_pointer
 *  Purpose:    take in a value and set the program
 *  Arguments:  an integer value to set the program counter
 *  Returns:    none
 */
void set_pointer(int val)
{
    program_counter = val;
}
