
#include "memory.h"
#include <stdio.h>

/* Stores general memory */
Seq_T memory;

/*
 * Stores all memory that is freed, addresses are taken off this stack before
 * new ones are created
 */
Stack_T address_stack;

const int MAX_MEM = 1024;

/*
 * Segment
 * Stores the size of the segment
 * Stores a pointer to the segment
 */
struct Segment
{
    int size;
    uint32_t *ptr;
};

void initialize_mem()
{
    memory = Seq_new(1);
    address_stack = Stack_new();
}

void print_memory()
{
    int size = Seq_length(memory);
    for (int i = 0; i < size; i++)
    {
        struct Segment *seg = Seq_get(memory, i);
        printf("Memory[%i]: \n", i);
        if (seg != NULL)
        {
            for (int j = 0; j < seg->size; j++)
            {
                printf("| %i ", seg->ptr[j]);
            }
            printf("| \n");
        }
        else
        {
            printf("NULL SEG\n");
        }
    }
}

void initialize_array(uint32_t *array, int size)
{
    for (int i = 0; i < size; i++)
    {
        array[i] = 0;
    }
}

uint32_t allocate_mem(int num_words)
{
    // printf("size allocated: %i\n", num_words);
    uint32_t *array = malloc(num_words * sizeof(uint32_t));
    assert(array != NULL);

    initialize_array(array, num_words);

    uint32_t address;
    struct Segment *seg = malloc(sizeof(struct Segment));
    assert(seg != NULL);

    seg->size = num_words;
    seg->ptr = array;
    if (Stack_empty(address_stack))
    {
        Seq_addhi(memory, seg);
        address = Seq_length(memory) - 1;
    }
    else
    {
        uint32_t *index = Stack_pop(address_stack);
        Seq_put(memory, *index, seg);
        address = *index;
        free(index);
    }
    return address;
}

void deallocate_mem(uint32_t address)
{
    struct Segment *seg = Seq_put(memory, address, NULL);
    assert(seg != NULL);
    free(seg->ptr);
    free(seg);
    assert(NULL == Seq_get(memory, address));
    int *address_ptr = malloc(sizeof(uint32_t));
    assert(address_ptr != NULL);
    *address_ptr = address;
    Stack_push(address_stack, address_ptr);
}

uint32_t *get_mem(uint32_t address, int offset)
{
    int size;
    uint32_t *seg = get_seg(address, &size);
    assert(offset >= 0 && offset < size);
    assert(seg != NULL);
    return &seg[offset];
}

uint32_t *get_seg(uint32_t address, int *size)
{
    struct Segment *seg = Seq_get(memory, address);
    if (size != NULL)
    {
        *size = seg->size;
    }
    assert(seg != NULL);
    return seg->ptr;
}

void free_mem()
{
    int size = Seq_length(memory);
    for (int i = 0; i < size; i++)
    {
        struct Segment *seg = Seq_get(memory, i);
        if (seg != NULL)
        {
            free(seg->ptr);
            free(seg);
        }
    }
    Seq_free(&memory);
    while (!Stack_empty(address_stack))
    {
        uint32_t *num = Stack_pop(address_stack);
        free(num);
    }
    Stack_free(&address_stack);
}