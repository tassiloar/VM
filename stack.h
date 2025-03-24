#ifndef STACK_H
#define STACK_H

/* Opaque type for the stack */
typedef struct Stack_T *Stack_T;

/*
 * Stack_new
 *  Purpose:    Creates a new, empty stack.
 *  Arguments:  none
 *  Returns:    A new stack.
 */
Stack_T Stack_new(void);

/*
 * Stack_empty
 *  Purpose:    Determines if the stack is empty.
 *  Arguments:  The stack.
 *  Returns:    Non-zero if empty; 0 otherwise.
 */
int Stack_empty(Stack_T stack);

/*
 * Stack_push
 *  Purpose:    Push an element onto the top of the stack.
 *  Arguments:  The stack and the element to push.
 *  Returns:    none.
 */
void Stack_push(Stack_T stack, void *elem);

/*
 * Stack_pop
 *  Purpose:    Removes and returns the top element from the stack.
 *  Arguments:  The stack.
 *  Returns:    The element that was on top.
 */
void *Stack_pop(Stack_T stack);

/*
 * Stack_free
 *  Purpose:    Frees all memory associated with the stack.
 *  Arguments:  A pointer to the stack.
 *  Returns:    none.
 */
void Stack_free(Stack_T *stack);

#endif
