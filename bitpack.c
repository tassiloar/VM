#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>
#include "bitpack.h"
#include <assert.h>

static inline uint64_t shl(uint64_t word, unsigned bits)
{
        assert(bits <= 64);
        if (bits == 64)
        {
                return 0;
        }
        else
        {
                return word << bits;
        }
}

static inline uint64_t shr(uint64_t word, unsigned bits)
{
        assert(bits <= 64);
        if (bits == 64)
        {
                return 0;
        }
        else
        {
                return word >> bits;
        }
}

static inline int64_t sra(uint64_t word, unsigned bits)
{
        assert(bits <= 64);
        if (bits == 64)
        {
                bits = 63;
        }
        return ((int64_t)word) >> bits;
}

bool Bitpack_fitsu(uint64_t n, unsigned width)
{
        assert(width <= 64);

        return shr(n, width) == 0;
}

uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= 64);
        unsigned hi = lsb + width;
        assert(hi <= 64);
        return shr(shl(word, 64 - hi), 64 - width);
}

uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb,
                      uint64_t value)
{
        assert(width <= 64);
        unsigned hi = lsb + width;
        assert(hi <= 64);
        if (!Bitpack_fitsu(value, width))
                exit(1);
        return shl(shr(word, hi), hi)               /* high part */
               | shr(shl(word, 64 - lsb), 64 - lsb) /* low part  */
               | (value << lsb);                    /* new part  */
}
