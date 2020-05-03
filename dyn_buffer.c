#include "dyn_buffer.h"
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
    if (!buff_redim(dest, dest->size*REDIM_FACTOR)) return false;
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

void d_buff_empty(d_buff_t* buff) {
  memset(buff->arr, 0, buff->size);
  buff->n_of_elements = 0;
}