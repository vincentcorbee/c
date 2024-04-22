#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

typedef void *Entry;

typedef struct Stack Stack;

void stack_push(Stack *stack, void *value);

void *stack_pop(Stack *stack);

void *stack_peek(Stack *stack);

Stack *stack_create(size_t size);

void stack_destroy(Stack *stack);

void stack_print(Stack *stack);

#endif