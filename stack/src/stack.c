#include "stack.h"
#include <stdio.h>

struct Stack
{
  size_t size;
  size_t count;
  float load_factor;
  Entry **elements;
};

struct Entry
{
  void *value;
};

static void stack_resize(Stack *stack, size_t new_size)
{
  if (stack == NULL)
    return;

  size_t index = 0;

  Entry **newElements = calloc(new_size, sizeof(Entry));

  while (index <= stack->size)
  {
    Entry *e = stack->elements[index];

    if (e != NULL)
      newElements[index] = e;

    index++;
  }

  free(stack->elements);

  stack->size = new_size;

  stack->elements = newElements;
}

void stack_push(Stack *stack, void *value)
{
  if (stack->size * stack->load_factor < stack->count + 1)
  {
    stack_resize(stack, stack->size * 2);
  }

  stack->elements[stack->count++] = value;
}

void *stack_pop(Stack *stack)
{
  if (stack->size == 0)
    return NULL;

  Entry *e = stack->elements[stack->size - 1];

  if (e == NULL)
    return NULL;

  stack->size--;

  return e;
}

void *stack_peek(Stack *stack)
{
  if (stack->size == 0)
    return NULL;

  Entry *e = stack->elements[stack->size - 1];

  if (e == NULL)
    return NULL;

  return e;
}

void stack_print(Stack *stack)
{
  printf("------ Stack ------\n");

  printf("count -> %zu\n", stack->count);

  printf("size -> %zu\n\n", stack->size);

  for (size_t i = 0; i < stack->count; i++)
  {
    printf("Entry: %lld\n", stack->elements[i]);
  }

  printf("-------------\n\n");
}

void stack_destroy(Stack *stack)
{
  free(stack->elements);
  free(stack);
}

Stack *stack_create(size_t size)
{
  Stack *stack = malloc(sizeof(Stack));

  stack->size = size;
  stack->count = 0;
  stack->load_factor = 0.75;
  stack->elements = calloc(size, sizeof(Entry));

  return stack;
}

typedef enum
{
  INST_PUSH,
  INST_PLUS
} Instruction_Type;

typedef struct Instruction
{
  uint64_t operand;
  Instruction_Type type;
} Instruction;

int main(void)
{
  Stack *stack = stack_create(1024);

  stack_push(stack, (int *)100);

  stack_push(stack, (int *)1);

  stack_print(stack);

  stack_destroy(stack);

  void **s = calloc(20, sizeof(void *));

  s[0] = (int *)10000;

  printf("%d\n", s[0]);

  return 0;
}