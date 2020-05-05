#include "dyn_buffer.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define REDIM_FACTOR 2

bool d_buff_init(d_buff_t* buff, size_t size) {
  buff->arr = calloc(size, sizeof(char));
  if (buff->arr == NULL) return false;
  buff->n_of_elements = 0;
  buff->size = size;
  return true;
}

bool d_buff_append(d_buff_t* dest, const char* src, size_t n) {
  if ((dest->n_of_elements + n) >= dest->size) {
    if (!d_buff_redim(dest, dest->size * REDIM_FACTOR)) return false;
  }
  memcpy(dest->arr + dest->n_of_elements, src, n);
  dest->n_of_elements += n;
  return true;
}

bool d_buff_redim(d_buff_t* buff, size_t new_size) {
  char* new_arr = realloc(buff->arr, new_size);
  if (new_arr == NULL) return false;

  buff->arr = new_arr;
  buff->size = new_size;
  return true;
}

char* d_buff_generate_str(d_buff_t* buff) {
  char* new_str = malloc(sizeof(char) * (buff->n_of_elements + 1));
  if (new_str == NULL) return NULL;
  memcpy(new_str, buff->arr, buff->n_of_elements);
  new_str[buff->n_of_elements] = '\0';
  d_buff_empty(buff);
  return new_str;
}

void d_buff_empty(d_buff_t* buff) {
  memset(buff->arr, 0, buff->size);
  buff->n_of_elements = 0;
}

size_t d_buff_get_len(d_buff_t* buff) {
  return buff->n_of_elements;
}

void d_buff_destroy(d_buff_t* buff) { free(buff->arr); }
