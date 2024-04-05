#include "calc.h"

double solve(char *expression) {
  s21_stack_t nums, signs;
  calculationStacks_t calculationStacks;
  double result;
  initialize_calculation_stacks((int)strlen(expression), &calculationStacks,
                                &nums, &signs);
  solve_and_push_to_stack(expression, &calculationStacks);
  result = stack_peek(calculationStacks.nums);
  destroy_calculation_stacks(&calculationStacks);
  return result;
}

void initialize_calculation_stacks(int size,
                                   calculationStacks_t *calculationStacks,
                                   s21_stack_t *nums, s21_stack_t *signs) {
  stack_create(nums, size);
  stack_create(signs, size);
  stack_push(nums, 0);
  calculationStacks->nums = nums;
  calculationStacks->signs = signs;
  calculationStacks->illegalExpression = 0;
}

void destroy_calculation_stacks(calculationStacks_t *calculationStacks) {
  stack_delete(calculationStacks->nums);
  stack_delete(calculationStacks->signs);
}

void solve_and_push_to_stack(char *expression,
                             calculationStacks_t *calculationStacks) {
  for (int i = 0; expression[i] != 0; i++)
    push_and_seek_for_bracket(expression, &i, calculationStacks);
  while (calculationStacks->signs->top_index > STACK_FLOOR)
    calculate_two_nums_in_stack_bracket_check(calculationStacks);
  if (calculationStacks->illegalExpression)
    stack_push(calculationStacks->nums, NO_CALCULATION_RESULT);
}

void push_and_seek_for_bracket(char *expression, int *i,
                               calculationStacks_t *calculationStacks) {
  char currentChar = expression[*i];
  if (is_number(currentChar))
    stack_push(calculationStacks->nums, get_number(expression, i));
  else if (currentChar != ' ') {
    calculate_and_push(currentChar, calculationStacks);
  }
}

void calculate_and_push(char currentChar,
                        calculationStacks_t *calculationStacks) {
  char priority[256];
  initialize_priority_array(priority);
  while (!is_need_to_push(priority, currentChar, calculationStacks->signs))
    calculate_two_nums_in_stack_bracket_check(calculationStacks);
  stack_push(calculationStacks->signs, currentChar);
}

void calculate_two_nums_in_stack_bracket_check(
    calculationStacks_t *calculationStacks) {
  if ((char)stack_peek(calculationStacks->signs) == ')') {
    stack_pop(calculationStacks->signs);
    calculate_until_bracket(calculationStacks);
  } else {
    calculate_two_nums_in_stack(calculationStacks);
  }
}

void calculate_two_nums_in_stack(calculationStacks_t *calculationStacks) {
  double num_1, num_2, sign, calculationResult = NO_CALCULATION_RESULT;
  get_nums_and_sign(&num_1, &num_2, &sign, calculationStacks);
  int got_result = 0;
  if (is_function((char)sign))
    calculationResult = get_func_calculation_result(num_1, sign, &got_result);
  else
    calculationResult = get_calculation_result(num_1, num_2, sign, &got_result);
  if (got_result)
    stack_push(calculationStacks->nums, calculationResult);
  else {
    if (!is_function(sign)) stack_push(calculationStacks->nums, num_2);
    stack_push(calculationStacks->nums, num_1);
  }
}

void get_nums_and_sign(double *num_1, double *num_2, double *sign,
                       calculationStacks_t *calculationStacks) {
  *sign = stack_peek_and_pop(calculationStacks->signs);
  *num_1 = stack_peek_and_pop(calculationStacks->nums);
  if (!is_function((char)*sign))
    *num_2 = stack_peek_and_pop(calculationStacks->nums);
}

double get_func_calculation_result(double num_1, double sign,
                                   int *got_result) {  // cstCSTqLl
  double result;
  *got_result = 1;
  switch ((int)sign) {
    case 'c':
      result = cos(num_1);
      break;
    case 's':
      result = sin(num_1);
      break;
    case 't':
      result = tan(num_1);
      break;
    case 'C':
      result = acos(num_1);
      break;
    case 'S':
      result = asin(num_1);
      break;
    case 'T':
      result = atan(num_1);
      break;
    case 'q':
      result = sqrt(num_1);
      break;
    case 'l':
      result = log(num_1);
      break;
    case 'L':
      result = log10(num_1);
      break;
    default:
      *got_result = 0;
      break;
  }
  return result;
}

double get_calculation_result(double num_1, double num_2, double sign,
                              int *got_result) {
  double result;
  *got_result = 1;
  switch ((int)sign) {
    case '-':
      result = num_2 - num_1;
      break;
    case '+':
      result = num_2 + num_1;
      break;
    case '*':
      result = num_2 * num_1;
      break;
    case '/':
      result = num_1 == 0 ? ILLEGAL_EXPRESSION : num_2 / num_1;
      break;
    case '^':
      result = pow(num_2, num_1);
      break;
    case '%':
      result = fmod(num_2, num_1);
      break;
    default:
      *got_result = 0;
      break;
  }
  return result;
}

void calculate_until_bracket(calculationStacks_t *calculationStacks) {
  while ((char)stack_peek(calculationStacks->signs) != '(')
    calculate_two_nums_in_stack_bracket_check(calculationStacks);
  stack_pop(calculationStacks->signs);
}

int is_number(char src) { return src >= '0' && src <= '0' + 9; }

int is_need_to_push(char *priority, double ch, s21_stack_t *sign) {
  return (((char)ch == '(' || (char)ch == ')' ||
           sign->top_index == STACK_FLOOR ||
           priority[(int)ch] > priority[(int)stack_peek(sign)]) &&
          stack_peek(sign) != ')');
}

double get_number(char *expression, int *index) {
  double result = 0;
  int dotCounter = 0;
  for (; expression[*index] == '.' || is_number(expression[*index]);
       *index = *index + 1)
    if (expression[*index] == '.')
      dotCounter = 1;
    else if (!dotCounter)
      result = result * 10 + (expression[*index] - '0');
    else
      result = result + ((expression[*index] - '0') * pow(0.1, dotCounter)),
      dotCounter++;
  *index = *index - 1;
  return result;
}

int is_function(char ch) {
  int result = 0;
  char funcs[] = "cstCSTqLl\0";
  for (int i = 0; i < (int)strlen(funcs); i++) result += ch == funcs[i];
  return result;
}

void initialize_priority_array(char *priority) {
  char priority_from_zero_to_four[5][10] = {"()", "+-", "*/%", "^",
                                            "cstCSTqLl"};
  for (int i = 0; i < 5; i++)
    for (int j = 0; j < (int)strlen(priority_from_zero_to_four[i]); j++) {
      priority[(int)priority_from_zero_to_four[i][j]] = i;
    }
}
