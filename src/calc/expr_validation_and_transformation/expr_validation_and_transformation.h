#ifndef VALIDATION_H
#define VALIDATION_H

#include "../../dictionary/dictionary.h"
#include "../calc.h"

int validate(char* expression);
void get_short_func_from_full(char* expression, int* index, char* shortFunc);
int is_signs_valid(char* expression);
int is_brackets_valid(char* expression);
void add_mul_sign(char* expression, int* i);
void create_func_dictionary(s21_dictionary_t* dictionary);
int is_sign(char ch);
int is_functions_valid(char* expression);
int is_expected_sign_or_num_or_bracket(char ch);
int is_numbers_valid(char* expression);
int check_number_and_skip_it(char* expression, int* index);
int is_next_sign_is_consecutive(char* expression, int i);
int is_sign_prefix(char* expression, int index);
void get_long_func(char* expression, int* index, char* longFunc);
void replase_full_func_with_short(char* expression, int* i);
void add_zero_to_prefix_sign(char* expression, int* i);
void transform(char* expression);
int get_x_index(char* expression);
int is_at_least_one_num_or_x(char* expression);
int is_x_valid(char* expression);
int is_every_brackets_contain_something(char* expression);
#endif