#include "dictionary.h"

void initialize_dictionary(s21_dictionary_t *dictionary, int maxWordSize,
                           int dictionaryLen) {
  dictionary->key = calloc(dictionaryLen, sizeof(char *));
  dictionary->values = calloc(dictionaryLen, sizeof(char *));
  for (int i = 0; i < dictionaryLen; i++) {
    dictionary->key[i] = calloc(maxWordSize, sizeof(char));
    dictionary->values[i] = calloc(maxWordSize, sizeof(char));
  }
  dictionary->lastElement = DICTIONARY_FLOOR;
  dictionary->len = dictionaryLen;
}

void destroy_dictionary(s21_dictionary_t *dictionary) {
  for (int i = 0; i < dictionary->len; i++) {
    free(dictionary->key[i]);
    free(dictionary->values[i]);
  }
  free(dictionary->key);
  free(dictionary->values);
  dictionary->key = NULL;
  dictionary->values = NULL;
}

void dictionary_assign_key_value(s21_dictionary_t *dictionary, char *key,
                                 char *value) {
  int index, keyIndex = dictionary_seek_for_key(dictionary, key);
  if (keyIndex != -1)
    index = keyIndex;
  else {
    dictionary->lastElement = dictionary->lastElement + 1;
    index = dictionary->lastElement;
  }
  sprintf(dictionary->key[index], "%s", key);
  sprintf(dictionary->values[index], "%s", value);
}

int dictionary_seek_for_key(s21_dictionary_t *dictionary, char *key) {
  int result = -1;
  for (int i = 0; i <= dictionary->lastElement && result == -1; i++) {
    if (strcmp(dictionary->key[i], key) == 0) result = i;
  }
  return result;
}

char *dictionary_get_value(s21_dictionary_t *dictionary, char *key) {
  int index = dictionary_seek_for_key(dictionary, key);
  return index != -1 ? dictionary->values[index] : NULL;
}