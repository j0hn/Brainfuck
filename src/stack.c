#include <stdlib.h>
#include <assert.h>
#include "stack.h"

struct stack_t{
    int length;
    int max_size;
    int *stack;
};

Stack stack_empty(int max_size){
    Stack result;

    assert(max_size > 0);

    result = (Stack) malloc(sizeof(struct stack_t));
    result->length = 0;
    result->max_size = max_size;
    result->stack = (int *) malloc(max_size * sizeof(int));

    return result;
}

void stack_push(Stack self, int value){
    assert(self != NULL);

    self->stack[self->length] = value;
    self->length++;
}

int stack_pop(Stack self){
    assert(!stack_is_empty(self));

    int result = stack_top(self);
    self->length--;

    return result;

}

int stack_top(Stack self){
    assert(!stack_is_empty(self));

    return self->stack[self->length - 1];
}

bool stack_full(Stack self){
    return self->length == self->max_size ? true : false;
}

bool stack_is_empty(Stack self){
    return self->length == 0 ? true : false;
}

void stack_destroy(Stack self){
    if(self != NULL){
        free(self->stack);
        free(self); self = NULL;
    }
}
