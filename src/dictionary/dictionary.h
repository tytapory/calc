#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DICTIONARY_FLOOR -1

typedef struct s21_dictionary_t {
  char** key;
  char** values;
  int lastElement;
  int len;
} s21_dictionary_t;

void initialize_dictionary(s21_dictionary_t* dictionary, int maxWordSize,
                           int dictionaryLen);
void destroy_dictionary(s21_dictionary_t* dictionary);
void dictionary_assign_key_value(s21_dictionary_t* dictionary, char* key,
                                 char* value);
int dictionary_seek_for_key(s21_dictionary_t* dictionary, char* key);
char* dictionary_get_value(s21_dictionary_t* dictionary, char* key);

#endif