
#ifndef MEMORY_H
#define MEMORY_H

#include "seq.h"
#include "stack.h"
#include <stdint.h>
#include <stdlib.h>
#include "assert.h"

/*Public*/

/*                          allocate_mem
 *  Purpose:    take in a number of words and allocate a segment within mem of
 *              that size. stores that size along with the segment
 *  Arguments:  the number of words to store within the segment
 *  Returns:    the address of this segment within general memory (address
 *              functions as an index)
 */
uint32_t allocate_mem(int size);

/*                          deallocate_mem
 *  Purpose:    take in an address and free the memory stored within
 *  Arguments:  an address to free
 *  Returns:    none
 */
void deallocate_mem(uint32_t address);

/*                          get_mem
 *  Purpose:    get a pointer to a single word within memory at the given
 *              address and offset
 *  Arguments:  an address of the segment in memory, an offset (index) of the
 *              word within the segment
 *  Returns:    a pointer to the specific item
 */
uint32_t *get_mem(uint32_t address, int offset);

/*                          get_seg
 *  Purpose:    return a pointer to a segment within memory at the given
 *              address
 *  Arguments:  the address of the segment within memory
 *  Returns:    a pointer to the start of the segment, the size of the segment
 */
uint32_t *get_seg(uint32_t address, int *size);

/*                          print_memory
 *  Purpose:    testing function, prints all memory to standard output
 *  Arguments:  none
 *  Returns:    none
 */
void print_memory();

/*                          free_mem
 *  Purpose:     Frees all segments in the memory sequence and the sequence
                 itself
 *  Arguments:   none
 *  Returns:     none
 */
void free_mem();

/*                          initialize_mem
 *  Purpose:    initialize the memory at startup, create the first seq and the
 *              address stack
 *  Arguments:  none
 *  Returns:    none
 */
void initialize_mem();

#endif
