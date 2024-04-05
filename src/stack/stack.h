#ifndef STACK_H
#define STACK_H

#define STACK_FLOOR -1

#include <math.h>
#include <stdlib.h>

typedef struct s21_stack_t {
  double *array;
  int top_index;
} s21_stack_t;

int stack_pop(s21_stack_t *stack);
void stack_push(s21_stack_t *stack, double src);
double stack_peek(s21_stack_t *stack);
int stack_create(s21_stack_t *stack, int size);
int stack_delete(s21_stack_t *stack);
double stack_peek_and_pop(s21_stack_t *stack);
#endif
