#include "stack.h"
int stack_create(s21_stack_t *stack, int size) {
  int res = size < 1;
  if (res == 0) {
    stack->array = calloc(size, sizeof(double));
    if (stack == NULL) res = 2;
    stack->top_index = STACK_FLOOR;
  }
  return res;
}

int stack_delete(s21_stack_t *stack) {
  int res = stack->array == NULL;
  if (res == 0) {
    free(stack->array);
    stack->array = NULL;
  }
  return res;
}

void stack_push(s21_stack_t *stack, double src) {
  stack->top_index = stack->top_index + 1;
  stack->array[stack->top_index] = src;
}

int stack_pop(s21_stack_t *stack) {
  int res = stack->top_index == STACK_FLOOR;
  if (res == 0) stack->top_index--;
  return res;
}

double stack_peek(s21_stack_t *stack) { return stack->array[stack->top_index]; }

double stack_peek_and_pop(s21_stack_t *stack) {
  double result = stack_peek(stack);
  stack_pop(stack);
  return result;
}
