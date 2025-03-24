#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <assert.h>
#include "seq.h"

/* Prototype for Um_write_sequence from your UM implementation */
extern void Um_write_sequence(FILE *output, Seq_T instructions);

extern void build_halt_test(Seq_T instructions);
extern void build_verbose_halt_test(Seq_T instructions);
extern void build_print6_test(Seq_T instructions);
extern void build_add_test(Seq_T instructions);
extern void build_map_seg_test(Seq_T instructions);
extern void access_seg_test(Seq_T stream);
extern void build_seg_store(Seq_T stream);
extern void build_prog_load_simple(Seq_T stream);
extern void build_nand_test(Seq_T stream);
extern void map_unmap(Seq_T stream);
extern void build_mult_test(Seq_T stream);
extern void build_div_test(Seq_T stream);
extern void build_cond_move(Seq_T stream);
extern void build_prog_load_zero_seg(Seq_T stream);
extern void build_add_overflow_test(Seq_T stream);
extern void build_seg_store_test_extensive(Seq_T stream);
extern void map_unmap_extensive(Seq_T stream);
extern void build_simple_os_loop(Seq_T stream);

/*
 * my_fmt_string: allocates a new string containing the formatted result.
 * The caller is responsible for freeing the returned string.
 */
char *my_fmt_string(const char *fmt, ...)
{
        va_list args;
        va_start(args, fmt);
        int len = vsnprintf(NULL, 0, fmt, args);
        va_end(args);

        char *buffer = malloc(len + 1);
        if (buffer == NULL)
        {
                perror("malloc failed");
                exit(1);
        }

        va_start(args, fmt);
        vsnprintf(buffer, len + 1, fmt, args);
        va_end(args);

        return buffer;
}

extern struct test_info
{
        const char *name;
        const char *test_input; /* NULL means no input needed */
        const char *expected_output;
        void (*build_test)(Seq_T stream);
} tests[];

static struct test_info tests[] = {
    {"halt", NULL, "", build_halt_test},
    {"halt-verbose", NULL, "", build_verbose_halt_test},
    {"add", NULL, "", build_add_test},
    {"print-six", NULL, "6", build_print6_test},
    {"map_seg_test", NULL, "1", build_map_seg_test},
    {"access_seg_test", NULL, "00000", access_seg_test},
    {"seg_store", NULL, "11111", build_seg_store},
    {"prog_load_simple", NULL, "0", build_prog_load_simple},
    {"nand_test", NULL, "1s", build_nand_test},
    {"map_unmap", NULL, "1s", map_unmap},
    {"mult_test", NULL, "2", build_mult_test},
    {"div_test", NULL, "2", build_div_test},
    {"cond_move", NULL, "2", build_cond_move},
    {"prog_load_zero_seg", NULL, "0", build_prog_load_zero_seg},
    {"add_overflow_test", NULL, "0", build_add_overflow_test},
    {"seg_store_extensive_test", NULL, "0", build_seg_store_test_extensive},
    {"map_unmap_extensive", NULL, "0", map_unmap_extensive},
    {"build_simple_os_loop", NULL, "0", build_simple_os_loop},
};

#define NTESTS (sizeof(tests) / sizeof(tests[0]))

/*
 * open_and_free_pathname:
 * Open file 'path' for writing, then free the pathname.
 * If anything fails, we abort via an assertion.
 */
static FILE *open_and_free_pathname(char *path)
{
        FILE *fp = fopen(path, "wb");
        assert(fp != NULL);
        free(path);
        return fp;
}

/*
 * write_or_remove_file:
 * If contents is NULL or empty, remove the given 'path',
 * otherwise write 'contents' into 'path'. Either way, free 'path'.
 */
static void write_or_remove_file(char *path, const char *contents)
{
        if (contents == NULL || *contents == '\0')
        {
                remove(path);
        }
        else
        {
                FILE *input = fopen(path, "wb");
                assert(input != NULL);
                fputs(contents, input);
                fclose(input);
        }
        free(path);
}

static void write_test_files(struct test_info *test)
{
        FILE *binary = open_and_free_pathname(my_fmt_string("%s.um", test->name));
        Seq_T instructions = Seq_new(0);
        test->build_test(instructions);
        Um_write_sequence(binary, instructions);
        Seq_free(&instructions);
        fclose(binary);

        write_or_remove_file(my_fmt_string("%s.0", test->name), test->test_input);
        write_or_remove_file(my_fmt_string("%s.1", test->name), test->expected_output);
}

int main(int argc, char *argv[])
{
        bool failed = false;
        if (argc == 1)
        {
                for (unsigned i = 0; i < NTESTS; i++)
                {
                        printf(" Writing test '%s'.\n", tests[i].name);
                        write_test_files(&tests[i]);
                }
        }
        else
        {
                for (int j = 1; j < argc; j++)
                {
                        bool tested = false;
                        for (unsigned i = 0; i < NTESTS; i++)
                        {
                                if (!strcmp(tests[i].name, argv[j]))
                                {
                                        tested = true;
                                        write_test_files(&tests[i]);
                                }
                        }
                        if (!tested)
                        {
                                failed = true;
                                fprintf(stderr, " No test named %s \n", argv[j]);
                        }
                }
        }
        return failed;
}
