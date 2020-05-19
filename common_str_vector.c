#include "common_str_vector.h"
#include "common_utils.h"

#include <stdio.h>

#define INITIAL_SIZE 32
#define REDIM_FACTOR 2

bool str_vector_init(vector_t* vector) {
  vector->n_elements = 0;
  vector->size = INITIAL_SIZE;
  vector->arr = malloc(sizeof(void*) * INITIAL_SIZE);
  vector->str_lenghts = malloc(sizeof(size_t) * INITIAL_SIZE);
  if (vector->arr == NULL) return false;
  return true;
}

bool str_vector_append(vector_t* vector, char* str, size_t str_len) {
  if (vector->n_elements + 1 > vector->size) {
    if (!str_vector_rsz(vector, vector->size * REDIM_FACTOR)) {
      return false;
    }
  }
  vector->arr[vector->n_elements] = str;
  vector->str_lenghts[vector->n_elements] = str_len;
  vector->n_elements++;
  return true;
}

char* str_vector_get(vector_t* vector, size_t index) {
  if (index >= vector->n_elements) {return NULL;}
  return vector->arr[index]; 
}

size_t str_vector_len(vector_t* vector) {
  return vector->n_elements;
}

size_t str_vector_elem_len(vector_t* vector, size_t index) {
  return vector->str_lenghts[index];
}


bool str_vector_rsz(vector_t* vector, size_t new_size) {
  char** new_arr = realloc(vector->arr, new_size * sizeof(void*));
  if (new_arr == NULL) return false;
  size_t* new_lenghts = realloc(vector->str_lenghts, new_size * sizeof(size_t));
  if (new_lenghts == NULL) return false;
  vector->arr = new_arr;
  vector->str_lenghts = new_lenghts;
  vector->size = new_size;
  return true;
}

char* str_vector_join(vector_t* vector) {
  size_t substr_len;
  size_t total_size = str_vec_total_size(vector);
  total_size++; // Por el /0 final
  char* result = malloc(total_size);
  result[total_size-1] = '\0';
  int pos_act = 0;
  for (int j = 0; j < vector->n_elements; j++) {
    substr_len = str_vector_elem_len(vector, j);
    memcpy(result + pos_act, str_vector_get(vector, j), substr_len);
    pos_act += substr_len;
  }
  return result;
}

void str_vector_destroy(vector_t* vector) {
  free(vector->arr);
  free(vector->str_lenghts);
}

void str_vector_clear(vector_t* vector) {
  vector->n_elements = 0;
}
