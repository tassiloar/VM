

#include "arithmetic.h"

/*                          c_move
 *  Purpose:    conditional move, executes a move if rC
 *  Arguments:  takes in the opcode, all needed registers, and a possible
 *              value
 *  Returns:    none
 */
void c_move(uint32_t *rA, uint32_t *rB, uint32_t *rC, uint32_t value)
{
    if (*rC != 0)
    {
        *rA = *rB;
    }
    (void)value;
}

/*                          seg_load
 *  Purpose:    load a piece of memory from general memory
 *  Arguments:  a place to store the word, the address, the offset as stored
 *              within rA, rB and rC respectively
 *  Returns:    none
 */
void seg_load(uint32_t *rA, uint32_t *rB, uint32_t *rC, uint32_t value)
{
    *rA = *get_mem(*rB, *rC);
    (void)value;
}

/*                          seg_store
 *  Purpose:    store a register within a segment in memory
 *  Arguments:  take in pointers to all 3 registers and a value
 *  Returns:    none
 */
void seg_store(uint32_t *rA, uint32_t *rB, uint32_t *rC, uint32_t value)
{
    *get_mem(*rA, *rB) = *rC;
    (void)value;
}

/*                          add
 *  Purpose:    Adds rB and rC and places value into rA
 *  Arguments:  take in pointers to all 3 registers and a value
 *  Returns:    none
 */
void add(uint32_t *rA, uint32_t *rB, uint32_t *rC, uint32_t value)
{
    *rA = (uint32_t)(*rB + *rC);
    (void)value;
}

/*                          mult
 *  Purpose:    Multiplies rB and rC and places value into rA
 *  Arguments:  take in pointers to all 3 registers and a value
 *  Returns:    none
 */
void mult(uint32_t *rA, uint32_t *rB, uint32_t *rC, uint32_t value)
{
    *rA = (uint32_t)(*rB * *rC);
    (void)value;
}

/*                          division
 *  Purpose:    Divides rB and rC and places value into rA
 *  Arguments:  take in pointers to all 3 registers and a value
 *  Returns:    none
 */
void division(uint32_t *rA, uint32_t *rB, uint32_t *rC, uint32_t value)
{
    assert(*rC != 0);
    *rA = *rB / *rC;
    (void)value;
}

/*                          nand
 *  Purpose:    Applied the NAND operator on rB an rC and places into rA
 *  Arguments:  take in pointers to all 3 registers and a value
 *  Returns:    none
 */
void nand(uint32_t *rA, uint32_t *rB, uint32_t *rC, uint32_t value)
{
    *rA = ~(*rB & *rC);
    (void)value;
}

/*                          halt
 *  Purpose:    Stops the execution of a number
 *  Arguments:  take in pointers to all 3 registers and a value
 *  Returns:    none
 */
void halt(uint32_t *rA, uint32_t *rB, uint32_t *rC, uint32_t value)
{
    set_pointer(-2);
    (void)rA;
    (void)rB;
    (void)rC;
    (void)value;
}

/*                          seg_map
 *  Purpose:    Allocates a segment of memory of size rC and places its address
                in rB
 *  Arguments:  take in pointers to all 3 registers and a value
 *  Returns:    none
 */
void seg_map(uint32_t *rA, uint32_t *rB, uint32_t *rC, uint32_t value)
{
    *rB = allocate_mem(*rC);
    (void)rA;
    (void)value;
}

/*                          seg_unmap
 *  Purpose:    Deallocates the memory at an address
 *  Arguments:  take in pointers to all 3 registers and a value
 *  Returns:    none
 */
void seg_unmap(uint32_t *rA, uint32_t *rB, uint32_t *rC, uint32_t value)
{
    assert(*rC != 0);
    deallocate_mem(*rC);
    (void)rA;
    (void)rB;
    (void)value;
}

/*                          out
 *  Purpose:    Prints the value in rC to stdout
 *  Arguments:  take in pointers to all 3 registers and a value
 *  Returns:    none
 */
void out(uint32_t *rA, uint32_t *rB, uint32_t *rC, uint32_t value)
{
    assert(*rC < 256);
    printf("%c", *rC);
    (void)rA;
    (void)rB;
    (void)value;
}

/*                          in
 *  Purpose:    Places a character from stdin into rC
 *  Arguments:  take in pointers to all 3 registers and a value
 *  Returns:    none
 */
void in(uint32_t *rA, uint32_t *rB, uint32_t *rC, uint32_t value)
{
    char input = getchar();

    if (input == EOF)
    {
        *rC = ~0;
    }
    else
    {
        *rC = input;
    }
    (void)rA;
    (void)rB;
    (void)value;
}

/*                          prog_load
 *  Purpose:    Copies a segment of memory into the instruction segment
 *  Arguments:  take in pointers to all 3 registers and a value
 *  Returns:    none
 */
void prog_load(uint32_t *rA, uint32_t *rB, uint32_t *rC, uint32_t value)
{
    set_pointer(*rC - 1);
    if (*rB == 0)
    {
        return;
    }
    int size;
    uint32_t *seg = get_seg(*rB, &size);
    deallocate_mem(0);
    allocate_mem(size);
    uint32_t *seg_0 = get_seg(0, NULL);
    for (int i = 0; i < size; i++)
    {
        seg_0[i] = seg[i];
    }
    (void)rA;
    (void)value;
}

/*                          val_load
 *  Purpose:    Loads a value into register rA
 *  Arguments:  take in pointers to all 3 registers and a value
 *  Returns:    none
 */
void val_load(uint32_t *rA, uint32_t *rB, uint32_t *rC, uint32_t value)
{
    *rA = value;
    (void)rB;
    (void)rC;
    (void)value;
}

/*
 *  func ptr array of all functions, allows for more simple execution functions
 */
void (*operations[14])(uint32_t *, uint32_t *, uint32_t *, uint32_t) = {
    c_move, seg_load, seg_store,
    add, mult, division, nand,
    halt, seg_map, seg_unmap,
    out, in, prog_load,
    val_load};

/*                          execute
 *  Purpose:    take in an opcode and 3 ptrs to registers and then execute the
 *              correct command
 *  Arguments:  takes in the opcode, all needed registers, and a possible
 *              value
 *  Returns:    none
 */
void execute(uint32_t opcode, uint32_t *rA, uint32_t *rB, uint32_t *rC,
             uint32_t value)
{
    operations[opcode](rA, rB, rC, value);
}