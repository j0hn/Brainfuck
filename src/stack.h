#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

typedef struct stack_t *Stack;

Stack stack_empty(int max_size);

void stack_push(Stack self, int value);
int stack_pop(Stack self);
int stack_top(Stack self);
bool stack_full(Stack self);
bool stack_is_empty(Stack self);
void stack_destroy(Stack self);

#endif /* STACK_H */
