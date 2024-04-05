#include "expr_validation_and_transformation.h"

int validate(char *expression) {
  return is_brackets_valid(expression) && is_signs_valid(expression) &&
         is_numbers_valid(expression) && is_functions_valid(expression) &&
         is_x_valid(expression) && is_at_least_one_num_or_x(expression);
}

void transform(char *expression) {
  for (int i = 0; expression[i] != 0; i++) {
    if (!is_expected_sign_or_num_or_bracket(expression[i]))
      replase_full_func_with_short(expression, &i);
    if (is_sign_prefix(expression, i)) add_zero_to_prefix_sign(expression, &i);
    if (is_number(expression[i]) && i > 0 && expression[i - 1] == ')')
      add_mul_sign(expression, &i);
    if (expression[i] == '(' && i > 0 &&
        (is_number(expression[i - 1]) || expression[i - 1] == ')'))
      add_mul_sign(expression, &i);
  }
}

int is_at_least_one_num_or_x(char *expression) {
  int result = 0;
  for (int i = 0; i < (int)strlen(expression) && result == 0; i++) {
    char currentChar = expression[i];
    if (is_number(currentChar) || currentChar == 'x') result = 1;
  }
  return result;
}

int is_x_valid(char *expression) {
  int result = 1, xIndex = 0;
  char *expressionPointerCopy = expression;
  while (xIndex != -1 && result == 1) {
    xIndex = get_x_index(expressionPointerCopy);
    expressionPointerCopy += xIndex + 1;
    while (expressionPointerCopy[0] == ' ') expressionPointerCopy++;
    if (expressionPointerCopy[0] == 'x') result = 0;
  }
  return result;
}

int get_x_index(char *expression) {
  int res = -1;
  for (int i = 0; i < (int)strlen(expression) && res == -1; i++)
    if (expression[i] == 'x') res = i;
  return res;
}

void add_mul_sign(char *expression, int *i) {
  for (int j = (int)strlen(expression); j >= *i; j--)
    expression[j + 1] = expression[j];
  expression[*i] = '*';
}

void add_zero_to_prefix_sign(char *expression, int *i) {
  for (int j = strlen(expression); j >= *i; j--)
    expression[j + 1] = expression[j];
  expression[*i] = '0';
}

void replase_full_func_with_short(char *expression, int *i) {
  char longFunc[10];
  get_long_func(expression, i, longFunc);
  *i = *i - (int)strlen(longFunc);
  char shortFunc[10];
  get_short_func_from_full(expression, i, shortFunc);
  *i = *i - (int)strlen(longFunc);
  int j;
  for (int y = 0; y < (int)strlen(longFunc) - 1; y++) {
    for (j = *i + 1; j < (int)strlen(expression) - 1; j++)
      expression[j] = expression[j + 1];
    expression[j] = 0;
  }
  expression[*i] = shortFunc[0];
}

int is_functions_valid(char *expression) {
  int result = 1;
  for (int i = 0; i < (int)strlen(expression) && result == 1; i++)
    if (!is_expected_sign_or_num_or_bracket(expression[i])) {
      char shortFunc[10];
      get_short_func_from_full(expression, &i, shortFunc);
      char longFunc[10];
      get_long_func(expression, &i, longFunc);
      if (shortFunc[0] == 0) result = 0;
    }
  return result;
}

int is_expected_sign_or_num_or_bracket(char ch) {
  int result = 0;
  char notFunctionsChars[] = " +-*/%^().\0";
  for (int i = 0; i < (int)strlen(notFunctionsChars) && result == 0; i++)
    result = ch == notFunctionsChars[i];
  return result || is_number(ch);
}

int is_numbers_valid(char *expression) {
  int result = 1;
  for (int i = 0; i < (int)strlen(expression) && result == 1; i++) {
    char currentChar = expression[i];
    if (is_number(currentChar))
      result = check_number_and_skip_it(expression, &i);
  }
  return result;
}

int check_number_and_skip_it(char *expression, int *index) {
  int dotCounter = 0;
  for (; expression[*index] == '.' || is_number(expression[*index]);
       *index = *index + 1)
    if (expression[*index] == '.') dotCounter += 1;
  *index = *index - 1;
  return dotCounter <= 1;
}

int is_signs_valid(char *expression) {
  int result = 1;
  for (int i = 0; i < (int)strlen(expression) && result == 1; i++)
    if (is_sign(expression[i]) && is_next_sign_is_consecutive(expression, i))
      result = 0;
  return result;
}

int is_next_sign_is_consecutive(char *expression, int i) {
  int result = 1, stop = 0;
  for (int j = i + 1; j < (int)strlen(expression) && stop == 0; j++) {
    if (is_sign(expression[j]) && !is_sign_prefix(expression, j)) stop = 1;
    if (is_number(expression[j]) || expression[j] == 'x') {
      result = 0;
      stop = 1;
    }
  }
  return result;
}

int is_sign_prefix(char *expression, int index) {
  return ((index == 0 || expression[index - 1] == '(') &&
          (index < (int)strlen(expression) &&
           (is_number(expression[index + 1]) ||
            expression[index + 1] == 'x'))) &&
         (expression[index] == '+' || expression[index] == '-');
}

int is_sign(char ch) {
  char signs[] = "+-*/%^\0";
  int result = 0;
  for (int i = 0; i < (int)strlen(signs); i++) result += ch == signs[i];
  return result;
}

int is_brackets_valid(char *expression) {
  int leftBracketsCounter = 0, rightBracketsCounter = 0, bracketsLegit = 1;
  for (int i = 0; i < (int)strlen(expression) && bracketsLegit == 1; i++) {
    leftBracketsCounter += expression[i] == '(';
    rightBracketsCounter += expression[i] == ')';
    if (rightBracketsCounter > leftBracketsCounter) bracketsLegit = 0;
  }
  return bracketsLegit && is_every_brackets_contain_something(expression);
}

int is_every_brackets_contain_something(char *expression) {
  int leftBracket = 0, bracketsLegit = 1, notSpacesInBrackets = 0;
  for (int i = 0; i < (int)strlen(expression) && bracketsLegit == 1; i++) {
    if (leftBracket == 0) leftBracket = expression[i] == '(';
    if (leftBracket == 0)
      notSpacesInBrackets = 0;
    else if (expression[i] != ')' && expression[i] != '(' &&
             expression[i] != ' ')
      notSpacesInBrackets++;
    if (expression[i] == ')') {
      if (notSpacesInBrackets == 0)
        bracketsLegit = 0;
      else
        leftBracket = 0;
    }
  }
  return bracketsLegit;
}

void get_short_func_from_full(char *expression, int *index, char *shortFunc) {
  s21_dictionary_t dictionary;
  create_func_dictionary(&dictionary);
  char longFunc[10];
  get_long_func(expression, index, longFunc);
  char *value = dictionary_get_value(&dictionary, longFunc);
  if (value != NULL)
    shortFunc[0] = value[0];
  else
    shortFunc[0] = 0;
  destroy_dictionary(&dictionary);
}

void get_long_func(char *expression, int *index, char *longFunc) {
  int i = 0;
  for (; expression[*index] != 0 &&
         !is_expected_sign_or_num_or_bracket(expression[*index]);
       i++, *index = *index + 1)
    longFunc[i] = expression[*index];
  longFunc[i] = 0;
}

void create_func_dictionary(s21_dictionary_t *dictionary) {
  initialize_dictionary(dictionary, 10, 10);
  dictionary_assign_key_value(dictionary, "sin", "s");
  dictionary_assign_key_value(dictionary, "cos", "c");
  dictionary_assign_key_value(dictionary, "tan", "t");
  dictionary_assign_key_value(dictionary, "acos", "C");
  dictionary_assign_key_value(dictionary, "asin", "S");
  dictionary_assign_key_value(dictionary, "atan", "T");
  dictionary_assign_key_value(dictionary, "sqrt", "q");
  dictionary_assign_key_value(dictionary, "ln", "L");
  dictionary_assign_key_value(dictionary, "log", "l");
  dictionary_assign_key_value(dictionary, "x", "x");
}