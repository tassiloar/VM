#ifndef BITPACK_H
#define BITPACK_H

#include <stdint.h>
#include <stdbool.h>

/*
 * Bitpack_fitsu:
 *   Checks whether the unsigned integer n can be represented in width bits.
 *   Returns true if it fits; false otherwise.
 */
bool Bitpack_fitsu(uint64_t n, unsigned width);

/*
 * Bitpack_getu:
 *   Extracts an unsigned value from the word.
 *   The bit-field has width bits, starting at lsb (least significant bit).
 *   Returns the extracted unsigned value.
 */
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb);

/*
 * Bitpack_newu:
 *   Returns a new word in which the bit-field of width 'width' starting at lsb is
 *   replaced by the given unsigned value.
 *   All other bits remain unchanged.
 *   Exits (via exit(1)) if value does not fit in the specified width.
 */
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, uint64_t value);

#endif
