#include <check.h>
#include <string.h>

#include "../s21_smart_calc.h"
#include "../stack/stack.h"

#define EPSILON 1e-6

START_TEST(stack_test) {
  s21_stack_t test_stack;
  double test_double = 5;
  stack_create(&test_stack, 10);
  stack_push(&test_stack, test_double);
  double result = stack_peek_and_pop(&test_stack);
  int topIndex = test_stack.top_index;
  stack_delete(&test_stack);
  ck_assert((int)result == test_double && test_stack.array == NULL &&
            topIndex == STACK_FLOOR);
}
END_TEST

START_TEST(dictionary_test) {
  s21_dictionary_t dictionary_test;
  char test_key[10] = "Hello";
  char test_word[10] = "World";
  char test_word_2[10] = "Verter";
  initialize_dictionary(&dictionary_test, 10, 3);
  dictionary_assign_key_value(&dictionary_test, test_key, test_word);
  int seekTestFound = dictionary_seek_for_key(&dictionary_test, test_key);
  int seekTestNotFound = dictionary_seek_for_key(&dictionary_test, test_word);
  char* getValueGot = dictionary_get_value(&dictionary_test, test_key);
  char* getValueFailed = dictionary_get_value(&dictionary_test, test_word);
  int difference = strcmp(getValueGot, test_word);
  dictionary_assign_key_value(&dictionary_test, test_key, test_word_2);
  char* getValueGot_2 = dictionary_get_value(&dictionary_test, test_key);
  int difference_2 = strcmp(getValueGot_2, test_word_2);
  destroy_dictionary(&dictionary_test);
  int destroyed = dictionary_test.key == NULL;
  ck_assert(seekTestFound != -1 && seekTestNotFound == -1 && difference == 0 &&
            difference_2 == 0 && getValueFailed == NULL && destroyed == 1);
}
END_TEST

START_TEST(calc_test_1) {
  char expression[] = "1+2";
  ck_assert(fabs(solve(expression) - 3) < EPSILON);
}

START_TEST(calc_test_2) {
  char expression[] = "1-2";
  ck_assert(fabs(solve(expression) - (-1)) < EPSILON);
}

START_TEST(calc_test_3) {
  char expression[] = "2/4";
  ck_assert(fabs(solve(expression) - 0.5) < EPSILON);
}

START_TEST(calc_test_4) {
  char expression[] = "2/0";
  ck_assert(solve(expression) == INFINITY);
}

START_TEST(calc_test_5) {
  char expression[] = "2*4";
  ck_assert(fabs(solve(expression) - 8) < EPSILON);
}

START_TEST(calc_test_6) {
  char expression[] = "5%2";
  ck_assert(fabs(solve(expression) - 1) < EPSILON);
}

START_TEST(calc_test_7) {
  char expression[] = "5^2";
  ck_assert(fabs(solve(expression) - 25) < EPSILON);
}

START_TEST(calc_test_dot_1) {
  char expression[] = "1.1+2.2";
  ck_assert(fabs(solve(expression) - 3.3) < EPSILON);
}

START_TEST(calc_test_dot_2) {
  char expression[] = "1.1-2";
  ck_assert(fabs(solve(expression) - (-0.9)) < EPSILON);
}

START_TEST(calc_test_dot_3) {
  char expression[] = "2.99/4.2";
  ck_assert(fabs(solve(expression) - 2.99 / 4.2) < EPSILON);
}

START_TEST(calc_test_dot_4) {
  char expression[] = "2.32*4.45";
  ck_assert(fabs(solve(expression) - 2.32 * 4.45) < EPSILON);
}

START_TEST(calc_test_dot_5) {
  char expression[] = "2.32^4.45";
  ck_assert(fabs(solve(expression) - pow(2.32, 4.45)) < EPSILON);
}

START_TEST(calc_test_functions_1) {
  char expression[] = "s(1)";
  ck_assert(fabs(solve(expression) - sin(1)) < EPSILON);
}

START_TEST(calc_test_functions_2) {
  char expression[] = "s(1.5)";
  ck_assert(fabs(solve(expression) - sin(1.5)) < EPSILON);
}

START_TEST(calc_test_functions_3) {
  char expression[] = "c(1)";
  ck_assert(fabs(solve(expression) - cos(1)) < EPSILON);
}

START_TEST(calc_test_functions_4) {
  char expression[] = "c(1.5)";
  ck_assert(fabs(solve(expression) - cos(1.5)) < EPSILON);
}

START_TEST(calc_test_functions_5) {
  char expression[] = "t(1)";
  ck_assert(fabs(solve(expression) - tan(1)) < EPSILON);
}

START_TEST(calc_test_functions_6) {
  char expression[] = "t(1.5)";
  ck_assert(fabs(solve(expression) - tan(1.5)) < EPSILON);
}

START_TEST(calc_test_functions_7) {
  char expression[] = "C(1)";
  ck_assert(fabs(solve(expression) - acos(1)) < EPSILON);
}

START_TEST(calc_test_functions_8) {
  char expression[] = "C(1.5)";
  ck_assert(isnan(solve(expression)));
}

START_TEST(calc_test_functions_9) {
  char expression[] = "S(1)";
  ck_assert(fabs(solve(expression) - asin(1)) < EPSILON);
}

START_TEST(calc_test_functions_10) {
  char expression[] = "S(1.5)";
  ck_assert(isnan(solve(expression)));
}

START_TEST(calc_test_functions_11) {
  char expression[] = "T(1)";
  ck_assert(fabs(solve(expression) - atan(1)) < EPSILON);
}

START_TEST(calc_test_functions_12) {
  char expression[] = "T(1.5)";
  ck_assert(fabs(solve(expression) - atan(1.5)) < EPSILON);
}

START_TEST(calc_test_functions_13) {
  char expression[] = "q(1)";
  ck_assert(fabs(solve(expression) - sqrt(1)) < EPSILON);
}

START_TEST(calc_test_functions_14) {
  char expression[] = "q(1.5)";
  ck_assert(fabs(solve(expression) - sqrt(1.5)) < EPSILON);
}

START_TEST(calc_test_functions_15) {
  char expression[] = "l(1)";
  ck_assert(fabs(solve(expression) - log(1)) < EPSILON);
}

START_TEST(calc_test_functions_16) {
  char expression[] = "l(1.5)";
  ck_assert(fabs(solve(expression) - log(1.5)) < EPSILON);
}

START_TEST(calc_test_functions_17) {
  char expression[] = "L(1)";
  ck_assert(fabs(solve(expression) - log10(1)) < EPSILON);
}

START_TEST(calc_test_functions_18) {
  char expression[] = "L(1.5)";
  ck_assert(fabs(solve(expression) - log10(1.5)) < EPSILON);
}

START_TEST(calc_test_long) {
  char expression[] =
      "15/(7-(1+1))*3-(2+(1+1))*15/(7-(200+1))*3-(2+(1+1))*(15/"
      "(7-(1+1))*3-(2+(1+1))+15/(7-(1+1))*3-(2+(1+1)))";
  ck_assert(fabs(solve(expression) - (-30.072164) < EPSILON));
}

START_TEST(validation_test_1) {
  char expression[] = "()";
  ck_assert(validate(expression) == 0);
}

START_TEST(validation_test_2) {
  char expression[] = "2()";
  ck_assert(validate(expression) == 0);
}

START_TEST(validation_test_3) {
  char expression[] = "2(3)";
  ck_assert(validate(expression) == 1);
}

START_TEST(validation_test_4) {
  char expression[] = " ";
  ck_assert(validate(expression) == 0);
}

START_TEST(validation_test_5) {
  char expression[] = "2sin(3)";
  ck_assert(validate(expression) == 1);
}

START_TEST(validation_test_6) {
  char expression[] = "(5((())))";
  ck_assert(validate(expression) == 0);
}

START_TEST(validation_test_7) {
  char expression[] = "5++5";
  ck_assert(validate(expression) == 0);
}

START_TEST(validation_test_8) {
  char expression[] = "5+5";
  ck_assert(validate(expression) == 1);
}

START_TEST(validation_test_9) {
  char expression[] = "-5";
  ck_assert(validate(expression) == 1);
}

START_TEST(validation_test_10) {
  char expression[] = "2+(-5)";
  ck_assert(validate(expression) == 1);
}

START_TEST(validation_test_11) {
  char expression[] = "fdf(3)";
  ck_assert(validate(expression) == 0);
}

START_TEST(transformation_test_1) {
  char expression[] = "sin(3)";
  transform(expression);
  ck_assert(strcmp(expression, "s(3)") == 0);
}

START_TEST(transformation_test_2) {
  char expression[] = "2(4)";
  transform(expression);
  ck_assert(strcmp(expression, "2*(4)") == 0);
}

START_TEST(transformation_test_3) {
  char expression[] = "-1";
  transform(expression);
  ck_assert(strcmp(expression, "0-1") == 0);
}

START_TEST(transformation_test_4) {
  char expression[] = "(3)4";
  transform(expression);
  ck_assert(strcmp(expression, "(3)*4") == 0);
}

int main() {
  Suite* s;
  TCase* tc_core;
  s = suite_create("calc");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, stack_test);
  tcase_add_test(tc_core, dictionary_test);
  tcase_add_test(tc_core, calc_test_1);
  tcase_add_test(tc_core, calc_test_2);
  tcase_add_test(tc_core, calc_test_3);
  tcase_add_test(tc_core, calc_test_4);
  tcase_add_test(tc_core, calc_test_5);
  tcase_add_test(tc_core, calc_test_6);
  tcase_add_test(tc_core, calc_test_7);
  tcase_add_test(tc_core, calc_test_dot_1);
  tcase_add_test(tc_core, calc_test_dot_2);
  tcase_add_test(tc_core, calc_test_dot_3);
  tcase_add_test(tc_core, calc_test_dot_4);
  tcase_add_test(tc_core, calc_test_dot_5);
  tcase_add_test(tc_core, calc_test_functions_1);
  tcase_add_test(tc_core, calc_test_functions_2);
  tcase_add_test(tc_core, calc_test_functions_3);
  tcase_add_test(tc_core, calc_test_functions_4);
  tcase_add_test(tc_core, calc_test_functions_5);
  tcase_add_test(tc_core, calc_test_functions_6);
  tcase_add_test(tc_core, calc_test_functions_7);
  tcase_add_test(tc_core, calc_test_functions_8);
  tcase_add_test(tc_core, calc_test_functions_9);
  tcase_add_test(tc_core, calc_test_functions_10);
  tcase_add_test(tc_core, calc_test_functions_11);
  tcase_add_test(tc_core, calc_test_functions_12);
  tcase_add_test(tc_core, calc_test_functions_13);
  tcase_add_test(tc_core, calc_test_functions_14);
  tcase_add_test(tc_core, calc_test_functions_15);
  tcase_add_test(tc_core, calc_test_functions_16);
  tcase_add_test(tc_core, calc_test_functions_17);
  tcase_add_test(tc_core, calc_test_functions_18);
  tcase_add_test(tc_core, calc_test_long);
  tcase_add_test(tc_core, validation_test_1);
  tcase_add_test(tc_core, validation_test_2);
  tcase_add_test(tc_core, validation_test_3);
  tcase_add_test(tc_core, validation_test_4);
  tcase_add_test(tc_core, validation_test_5);
  tcase_add_test(tc_core, validation_test_6);
  tcase_add_test(tc_core, validation_test_7);
  tcase_add_test(tc_core, validation_test_8);
  tcase_add_test(tc_core, validation_test_9);
  tcase_add_test(tc_core, validation_test_10);
  tcase_add_test(tc_core, validation_test_11);
  tcase_add_test(tc_core, transformation_test_1);
  tcase_add_test(tc_core, transformation_test_2);
  tcase_add_test(tc_core, transformation_test_3);
  tcase_add_test(tc_core, transformation_test_4);

  suite_add_tcase(s, tc_core);

  SRunner* sr;
  sr = srunner_create(s);
  tcase_set_timeout(tc_core, 100);
  srunner_run_all(sr, CK_NORMAL);
  int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? 0 : 1;
}
