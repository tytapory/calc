#ifndef CALC_H
#define CALC_H

#define NO_CALCULATION_RESULT NAN
#define ILLEGAL_EXPRESSION INFINITY

#include <math.h>
#include <string.h>

#include "../stack/stack.h"

typedef struct calculationStacks_t {
  s21_stack_t* nums;
  s21_stack_t* signs;
  int illegalExpression;
} calculationStacks_t;

double solve(char* expression);
void calculate_two_nums_in_stack_bracket_check(
    calculationStacks_t* calculationStacks);
void initialize_calculation_stacks(int size,
                                   calculationStacks_t* calculationStacks,
                                   s21_stack_t* nums, s21_stack_t* signs);
void destroy_calculation_stacks(calculationStacks_t* calculationStacks);
int is_number(char src);
double get_number(char* expression, int* index);
void calculate_two_nums_in_stack(calculationStacks_t* calculationStacks);
void calculate_until_bracket(calculationStacks_t* calculationStacks);
double get_calculation_result(double num_1, double num_2, double sign,
                              int* got_result);
void get_nums_and_sign(double* num_1, double* num_2, double* sign,
                       calculationStacks_t* calculationStacks);
void initialize_priority_array(char* priority);
int is_need_to_push(char* priority, double ch, s21_stack_t* sign);
void solve_and_push_to_stack(char* expression,
                             calculationStacks_t* calculationStacks);
void push_and_seek_for_bracket(char* expression, int* i,
                               calculationStacks_t* calculationStacks);
void calculate_and_push(char currentChar,
                        calculationStacks_t* calculationStacks);
double get_func_calculation_result(double num_1, double sign, int* got_result);
int is_function(char ch);

#endif
