#include "seq.h"
#include <stdlib.h>
#include <assert.h>

struct Seq_T
{
    void **array; // Dynamic array to hold elements
    int length;   // Current number of elements
    int capacity; // Allocated capacity of the array
};

Seq_T Seq_new(int hint)
{
    // Ensure a minimum capacity of 1
    if (hint < 1)
    {
        hint = 1;
    }
    Seq_T seq = malloc(sizeof(*seq));
    assert(seq != NULL);
    seq->array = malloc(hint * sizeof(void *));
    assert(seq->array != NULL);
    seq->length = 0;
    seq->capacity = hint;
    return seq;
}

int Seq_length(Seq_T seq)
{
    assert(seq != NULL);
    return seq->length;
}

void *Seq_get(Seq_T seq, int i)
{
    assert(seq != NULL);
    assert(i >= 0 && i < seq->length);
    return seq->array[i];
}

void Seq_addhi(Seq_T seq, void *elem)
{
    assert(seq != NULL);
    // Resize the array if needed
    if (seq->length >= seq->capacity)
    {
        int new_capacity = seq->capacity * 2;
        void **new_array = realloc(seq->array, new_capacity * sizeof(void *));
        assert(new_array != NULL);
        seq->array = new_array;
        seq->capacity = new_capacity;
    }
    seq->array[seq->length] = elem;
    seq->length++;
}

void *Seq_put(Seq_T seq, int i, void *elem)
{
    assert(seq != NULL);
    assert(i >= 0 && i < seq->length);
    void *old_elem = seq->array[i];
    seq->array[i] = elem;
    return old_elem;
}

void Seq_free(Seq_T *seq)
{
    if (seq != NULL && *seq != NULL)
    {
        free((*seq)->array);
        free(*seq);
        *seq = NULL;
    }
}
