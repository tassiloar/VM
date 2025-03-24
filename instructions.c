#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <seq.h>
#include <bitpack.h>

typedef uint32_t Um_instruction;
typedef enum Um_opcode
{
        CMOV = 0,
        SLOAD,
        SSTORE,
        ADD,
        MUL,
        DIV,
        NAND,
        HALT,
        ACTIVATE,
        INACTIVATE,
        OUT,
        IN,
        LOADP,
        LV
} Um_opcode;

typedef enum Um_register
{
        r0 = 0,
        r1,
        r2,
        r3,
        r4,
        r5,
        r6,
        r7
} Um_register;

/* Functions that return the two instruction types */

Um_instruction three_register(Um_opcode op, int ra, int rb, int rc);
Um_instruction loadval(unsigned ra, unsigned val);

/* Wrapper functions for each of the instructions */

static inline Um_instruction halt()
{
        return three_register(HALT, 0, 0, 0);
}

static inline Um_instruction output(Um_register c)
{
        return three_register(OUT, 0, 0, c);
}

static inline Um_instruction add(Um_register a, Um_register b, Um_register c)
{
        return three_register(ADD, a, b, c);
}

static inline Um_instruction con_move(Um_register a, Um_register b,
                                      Um_register c)
{
        return three_register(CMOV, a, b, c);
}

static inline Um_instruction segLoad(Um_register a, Um_register b,
                                     Um_register c)
{
        return three_register(SLOAD, a, b, c);
}
static inline Um_instruction segStore(Um_register a, Um_register b,
                                      Um_register c)
{
        return three_register(SSTORE, a, b, c);
}

static inline Um_instruction divide(Um_register a, Um_register b,
                                    Um_register c)
{
        return three_register(DIV, a, b, c);
}

static inline Um_instruction mult(Um_register a, Um_register b,
                                  Um_register c)
{
        return three_register(MUL, a, b, c);
}

static inline Um_instruction nand(Um_register a, Um_register b,
                                  Um_register c)
{
        return three_register(NAND, a, b, c);
}

static inline Um_instruction map(Um_register b, Um_register c)
{
        return three_register(ACTIVATE, 0, b, c);
}

static inline Um_instruction unmap(Um_register c)
{
        return three_register(INACTIVATE, 0, 0, c);
}

static inline Um_instruction in(Um_register a, Um_register b, Um_register c)
{
        return three_register(IN, a, b, c);
}

static inline Um_instruction loadp(Um_register b, Um_register c)
{
        return three_register(LOADP, 0, b, c);
}

/* Functions for working with streams */

static inline void append(Seq_T stream, Um_instruction inst)
{
        assert(sizeof(inst) <= sizeof(uintptr_t));
        Seq_addhi(stream, (void *)(uintptr_t)inst);
}

const uint32_t Um_word_width = 32;

void Um_write_sequence(FILE *output, Seq_T stream)
{
        assert(output != NULL && stream != NULL);
        int stream_length = Seq_length(stream);
        for (int i = 0; i < stream_length; i++)
        {
                Um_instruction inst = (uintptr_t)Seq_remlo(stream);
                for (int lsb = Um_word_width - 8; lsb >= 0; lsb -= 8)
                {
                        fputc(Bitpack_getu(inst, 8, lsb), output);
                }
        }
}

// implementation of 3 registers
Um_instruction three_register(Um_opcode op, int ra, int rb, int rc)
{
        uint64_t word = 0;
        word = Bitpack_newu(word, 3, 0, rc);  // C register
        word = Bitpack_newu(word, 3, 3, rb);  // B register
        word = Bitpack_newu(word, 3, 6, ra);  // A register
        return Bitpack_newu(word, 4, 28, op); // OP code
}

// implementation of load val
Um_instruction loadval(unsigned ra, unsigned val)
{
        uint64_t word = 0;
        word = Bitpack_newu(word, 4, 28, 13); // OP code
        word = Bitpack_newu(word, 3, 25, ra); // B register
        return Bitpack_newu(word, 25, 0, val);
}

/* Unit tests for the UM */

/* tests that the correct address is deposited in r2 */
void build_map_seg_test(Seq_T stream)
{
        append(stream, loadval(r1, 50));
        append(stream, loadval(r3, 48));
        append(stream, map(r2, r1));
        append(stream, add(r2, r2, r3));
        append(stream, output(r2));
        append(stream, halt());
}

void build_seg_store_test_extensive(Seq_T stream)
{
        append(stream, loadval(r1, 8));
        append(stream, map(r2, r1));
        append(stream, map(r2, r1));
        append(stream, map(r2, r1));
        append(stream, map(r2, r1));
        append(stream, map(r2, r1));
        append(stream, loadval(r3, 5));
        append(stream, segStore(r2, r3, r3));
        append(stream, loadval(r3, 2));
        append(stream, loadval(r4, 3));
        append(stream, segStore(r3, r4, r4));
        append(stream, loadval(r3, 1));
        append(stream, loadval(r4, 1));
        append(stream, segStore(r3, r4, r4));
        append(stream, loadval(r3, 3));
        append(stream, loadval(r4, 3));
        append(stream, segStore(r3, r4, r4));
        append(stream, loadval(r3, 4));
        append(stream, loadval(r4, 4));
        append(stream, segStore(r3, r4, r4));
        append(stream, halt());
}

void build_nand_test(Seq_T stream)
{
        append(stream, loadval(r1, 2));
        append(stream, loadval(r2, 1));
        append(stream, nand(r3, r1, r2));
        append(stream, output(r3));
        append(stream, halt());
}

/* tests that all mapped segments are correctly loaded to be 0 */
void access_seg_test(Seq_T stream)
{
        append(stream, loadval(r1, 5));
        append(stream, loadval(r6, 1));
        append(stream, loadval(r7, 0));
        append(stream, loadval(r3, 48));

        append(stream, map(r2, r1));
        append(stream, segLoad(r4, r2, r7));
        append(stream, add(r4, r4, r3));
        append(stream, output(r4));

        append(stream, map(r2, r1));
        append(stream, segLoad(r4, r2, r7));
        append(stream, add(r4, r4, r3));
        append(stream, output(r4));

        append(stream, map(r2, r1));
        append(stream, segLoad(r4, r2, r7));
        append(stream, add(r4, r4, r3));
        append(stream, output(r4));

        append(stream, map(r2, r1));
        append(stream, segLoad(r4, r2, r7));
        append(stream, add(r4, r4, r3));
        append(stream, output(r4));

        append(stream, map(r2, r1));
        append(stream, segLoad(r4, r2, r7));
        append(stream, add(r4, r4, r3));
        append(stream, output(r4));

        append(stream, halt());
}

/* stores a segment in a mapped segment then retrieves it */
void build_seg_store(Seq_T stream)
{
        append(stream, loadval(r1, 5));
        append(stream, loadval(r6, 1));
        append(stream, loadval(r7, 0));
        append(stream, loadval(r3, 48));

        append(stream, map(r2, r1));
        append(stream, segStore(r2, r7, r1));
        append(stream, segLoad(r4, r2, r7));
        append(stream, add(r4, r4, r3));
        append(stream, output(r4));
        append(stream, add(r7, r7, r6));

        append(stream, map(r2, r1));
        append(stream, segStore(r2, r7, r1));
        append(stream, segLoad(r4, r2, r7));
        append(stream, add(r4, r4, r3));
        append(stream, output(r4));
        append(stream, add(r7, r7, r6));

        append(stream, segStore(r2, r7, r1));
        append(stream, segLoad(r4, r2, r7));
        append(stream, add(r4, r4, r3));
        append(stream, output(r4));
        append(stream, add(r7, r7, r6));

        append(stream, segStore(r2, r7, r1));
        append(stream, segLoad(r4, r2, r7));
        append(stream, add(r4, r4, r3));
        append(stream, output(r4));
        append(stream, add(r7, r7, r6));

        append(stream, segStore(r2, r7, r1));
        append(stream, segLoad(r4, r2, r7));
        append(stream, add(r4, r4, r3));
        append(stream, output(r4));
        append(stream, add(r7, r7, r6));

        append(stream, segStore(r2, r7, r1));
        append(stream, segLoad(r4, r2, r7));
        append(stream, add(r4, r4, r3));
        append(stream, output(r4));
        append(stream, add(r7, r7, r6));

        append(stream, halt());
}

void build_prog_load_simple(Seq_T stream)
{
        append(stream, loadval(r7, 1));
        append(stream, loadval(r2, 2));

        append(stream, map(r3, r2));
        append(stream, loadval(r1, 0));
        append(stream, segLoad(r6, r1, r1));
        append(stream, segStore(r3, r1, r6));

        append(stream, loadval(r7, 11));
        append(stream, loadp(r3, r1));
}

void build_prog_load_zero_seg(Seq_T stream)
{
        append(stream, loadval(r1, 48));
        append(stream, loadval(r2, 6));
        append(stream, add(r3, r1, r2));
        append(stream, loadval(r5, 0));
        append(stream, loadval(r6, 7));
        append(stream, loadp(r5, r6));
        append(stream, halt());
        append(stream, loadval(r3, '0'));
        append(stream, output(r3));
        append(stream, halt());
}

void map_unmap_extensive(Seq_T stream)
{
        append(stream, loadval(r2, 10));
        append(stream, map(r1, r2));
        append(stream, loadval(r2, 5));
        append(stream, map(r1, r2));
        append(stream, loadval(r2, 12));
        append(stream, map(r1, r2));
        append(stream, loadval(r2, 16));
        append(stream, map(r1, r2));
        append(stream, loadval(r2, 3));
        append(stream, map(r1, r2));
        append(stream, loadval(r2, 1));
        append(stream, map(r1, r2));
        append(stream, loadval(r2, 7));
        append(stream, map(r1, r2));
        append(stream, loadval(r2, 8));
        append(stream, map(r1, r2));
        append(stream, loadval(r2, 10));
        append(stream, map(r1, r2));
        append(stream, loadval(r2, 12));
        append(stream, map(r1, r2));

        append(stream, loadval(r3, 4));
        append(stream, segStore(r3, r3, r3));
        append(stream, loadval(r3, 2));
        append(stream, loadval(r4, 3));
        append(stream, segStore(r3, r4, r4));
        append(stream, loadval(r3, 1));
        append(stream, loadval(r4, 1));
        append(stream, segStore(r3, r4, r4));
        append(stream, loadval(r3, 3));
        append(stream, loadval(r4, 3));
        append(stream, segStore(r3, r4, r4));
        append(stream, loadval(r3, 4));
        append(stream, loadval(r4, 4));
        append(stream, segStore(r3, r4, r4));

        append(stream, loadval(r2, 1));
        append(stream, unmap(r2));
        append(stream, loadval(r2, 7));
        append(stream, unmap(r2));
        append(stream, loadval(r2, 9));
        append(stream, unmap(r2));
        append(stream, loadval(r2, 10));
        append(stream, unmap(r2));
        append(stream, loadval(r2, 6));
        append(stream, unmap(r2));

        append(stream, halt());
}

void map_unmap(Seq_T stream)
{
        append(stream, loadval(r2, 10));
        append(stream, map(r1, r2));
        append(stream, map(r1, r2));
        append(stream, loadval(r3, 1));
        append(stream, unmap(r3));
        append(stream, map(r1, r2));
        append(stream, halt());
}

/*  */
void build_add_test(Seq_T stream)
{
        append(stream, loadval(r1, 48));
        append(stream, loadval(r2, 6));
        append(stream, add(r3, r2, r1));
        append(stream, output(r3));
        append(stream, halt());
}

void build_add_overflow_test(Seq_T stream)
{
        append(stream, loadval(r1, 33554431));
        append(stream, add(r1, r1, r1));
        append(stream, add(r1, r1, r1));
        append(stream, add(r1, r1, r1));
        append(stream, add(r1, r1, r1));
        append(stream, add(r1, r1, r1));
        append(stream, add(r1, r1, r1));
        append(stream, add(r1, r1, r1));
        append(stream, add(r1, r1, r1));
        append(stream, add(r1, r1, r1));
        append(stream, add(r1, r1, r1));
        append(stream, halt());
}

void build_print6_test(Seq_T stream)
{
        append(stream, loadval(r1, 48));
        append(stream, loadval(r2, 6));
        append(stream, add(r3, r1, r2));
        append(stream, output(r3));
        append(stream, halt());
}

void build_halt_test(Seq_T stream)
{
        append(stream, halt());
}

void build_verbose_halt_test(Seq_T stream)
{
        append(stream, halt());
        append(stream, loadval(r1, 'B'));
        append(stream, output(r1));
        append(stream, loadval(r1, 'a'));
        append(stream, output(r1));
        append(stream, loadval(r1, 'd'));
        append(stream, output(r1));
        append(stream, loadval(r1, '!'));
        append(stream, output(r1));
        append(stream, loadval(r1, '\n'));
        append(stream, output(r1));
}

void build_mult_test(Seq_T stream)
{
        append(stream, loadval(r1, 5));
        append(stream, loadval(r2, 10));
        append(stream, mult(r3, r1, r2));
        append(stream, output(r3));
        append(stream, halt());
}

void build_div_test(Seq_T stream)
{
        append(stream, loadval(r1, 100));
        append(stream, loadval(r2, 2));
        append(stream, divide(r3, r1, r2));
        append(stream, output(r3));
        append(stream, halt());
}

void build_cond_move(Seq_T stream)
{
        append(stream, loadval(r2, 'A'));
        append(stream, loadval(r3, 1));
        append(stream, con_move(r1, r2, r3));
        append(stream, output(r1));

        append(stream, loadval(r1, 0));
        append(stream, loadval(r2, 'B'));
        append(stream, loadval(r3, 0));
        append(stream, con_move(r1, r2, r3));
        append(stream, output(r1));

        append(stream, halt());
}

void build_simple_os_loop(Seq_T stream)
{
        // Print"Simple OS v1.0\n"
        const char *banner = "Simple OS\n";
        for (int i = 0; banner[i] != '\0'; i++)
        {
                append(stream, loadval(r1, (unsigned)banner[i]));
                append(stream, output(r1));
        }

        // Print prompt: "OS> "
        const char *prompt = "OS> ";
        for (int i = 0; prompt[i] != '\0'; i++)
        {
                append(stream, loadval(r1, (unsigned)prompt[i]));
                append(stream, output(r1));
        }

        /* Input instruction: */
        append(stream, in(r0, r0, r1));

        // Print response header: "\nYou entered: "
        const char *response = "\nYou entered: ";
        for (int i = 0; response[i] != '\0'; i++)
        {
                append(stream, loadval(r1, (unsigned)response[i]));
                append(stream, output(r1));
        }

        // Output the input character (stored in r1) again.
        append(stream, output(r1));

        // Print a newline.
        append(stream, loadval(r1, '\n'));
        append(stream, output(r1));

        // Instead of halting, jump back to the beginning of the main loop.
        append(stream, loadval(r7, 0));
        append(stream, loadp(r7, r0));
}