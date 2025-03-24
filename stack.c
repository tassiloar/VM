#include "stack.h"
#include <stdlib.h>
#include <assert.h>

/* Internal node structure for the stack */
struct Node
{
    void *elem;
    struct Node *next;
};

/* Def of the opaque Stack_T structure */
struct Stack_T
{
    struct Node *top;
};

Stack_T Stack_new(void)
{
    Stack_T stack = malloc(sizeof(*stack));
    assert(stack != NULL);
    stack->top = NULL;
    return stack;
}

int Stack_empty(Stack_T stack)
{
    assert(stack != NULL);
    return stack->top == NULL;
}

void Stack_push(Stack_T stack, void *elem)
{
    assert(stack != NULL);
    struct Node *node = malloc(sizeof(*node));
    assert(node != NULL);
    node->elem = elem;
    node->next = stack->top;
    stack->top = node;
}

void *Stack_pop(Stack_T stack)
{
    assert(stack != NULL);
    assert(!Stack_empty(stack));
    struct Node *node = stack->top;
    void *elem = node->elem;
    stack->top = node->next;
    free(node);
    return elem;
}

void Stack_free(Stack_T *stack)
{
    assert(stack != NULL && *stack != NULL);
    while (!Stack_empty(*stack))
    {
        free(Stack_pop(*stack));
    }
    free(*stack);
    *stack = NULL;
}
