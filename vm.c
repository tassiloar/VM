#include <stdio.h>
#include <stdint.h>
#include "memory.h"
#include "cpu.h"

/*                          get_num_words
 *  Purpose:    Parse a file to get the number of int32_t's within the FILE
 *  Arguments:  take in an opened and checked FILE*
 *  Returns:    returns an amount of words
 */
int get_num_words(FILE *in)
{

    uint32_t instruction;
    int num_instructions = 0;

    while (fread(&instruction, sizeof(uint32_t), 1, in) == 1)
    {
        num_instructions++;
    }

    return num_instructions;
}

/*                          getWord
 *  Purpose:    Read a int32_t from a FILE in big endian and store it within a
                int32_t
 *  Arguments:  a checked and opened FILE*
 *  Returns:    an int32_t storing the correct number
 */
uint32_t getWord(FILE *in)
{
    int32_t word = 0;
    int num_bytes = sizeof(uint32_t);
    int c;
    for (int i = num_bytes - 1; i >= 0; i--)
    {
        c = fgetc(in);
        word |= (uint32_t)c << (i * 8);
    }
    return word;
}

/*                          read_segment
 *  Purpose:    read in a file and store it within a segment of memory in
                general memory
 *  Arguments:  a checked and opened FILE*
 *  Returns:    none
 */
void read_segment(FILE *in)
{
    /* Get the number of instructions */
    int size = get_num_words(in);

    rewind(in);

    uint32_t address = allocate_mem(size);
    assert(address == 0);

    for (int i = 0; i < size; i++)
    {
        uint32_t *mem = get_mem(address, i);
        *mem = getWord(in);
    }
}

/*The main entry point to the program*/
int main(int argc, char *argv[])
{
    /*Must accept only one input */
    if (argc != 2)
    {
        fprintf(stderr, "invalid arguments\n");
        exit(1);
    }

    /*Open the input file in rb mode*/
    FILE *instructions = fopen(argv[1], "rb");

    /*Check if file can be opened*/
    if (instructions == NULL)
    {
        fprintf(stderr, "file cannot be opened \n");
        exit(1);
    }

    /*Initialize memory objects*/
    initialize_mem();

    /*Read instructions from file into segment 0*/
    read_segment(instructions);

    program_loop();

    free_mem();

    fclose(instructions);

    return 0;
}