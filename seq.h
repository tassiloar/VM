#ifndef SEQ_H
#define SEQ_H

#include <stdlib.h>

/* Define the opaque type for a sequence */
typedef struct Seq_T *Seq_T;

/*
 * Seq_new
 *  Purpose: Creates a new sequence with an initial hint for the capacity.
 *  Arguments: An integer hint for the initial allocation (can be 0 if unknown).
 *  Returns: A new sequence.
 */
Seq_T Seq_new(int hint);

/*
 * Seq_length
 *  Purpose: Returns the number of elements currently stored in the sequence.
 *  Arguments: The sequence.
 *  Returns: The number of elements.
 */
int Seq_length(Seq_T seq);

/*
 * Seq_get
 *  Purpose: Retrieves the element at the specified index in the sequence.
 *  Arguments: The sequence and the index.
 *  Returns: The element at that index.
 */
void *Seq_get(Seq_T seq, int i);

/*
 * Seq_addhi
 *  Purpose: Appends an element to the end of the sequence.
 *  Arguments: The sequence and the element to add.
 */
void Seq_addhi(Seq_T seq, void *elem);

/*
 * Seq_put
 *  Purpose: Replaces the element at the specified index with a new element.
 *           Returns the old element.
 *  Arguments: The sequence, the index, and the new element.
 *  Returns: The old element at that index.
 */
void *Seq_put(Seq_T seq, int i, void *elem);

/*
 * Seq_free
 *  Purpose: Frees all memory associated with the sequence.
 *  Arguments: A pointer to the sequence variable.
 *  Returns: None.
 */
void Seq_free(Seq_T *seq);

#endif
