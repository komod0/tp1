#include "dynamic_vector.h"

#define INITIAL_SIZE 32
#define REDIM_FACTOR 2

bool vector_init(vector_t* vector) {
  vector->n_elements = 0;
  vector->size = INITIAL_SIZE;
  vector->arr = malloc(sizeof(void*) * INITIAL_SIZE);
  if (vector->arr == NULL) return false;
}

bool vector_append(vector_t* vector, void* element) {
  if (vector->n_elements + 1 > vector->size) {
    if (!vector_resize(vector, vector->size * REDIM_FACTOR)) {
      return false;
    }
  }
  vector->arr[vector->size] = element;
  vector->n_elements++;
}

void* vector_get(vector_t* vector, size_t index) {
  return vector->arr[index]; 
}


bool vector_resize(vector_t* vector, size_t new_size) {
  void** new_arr = realloc(vector->arr, new_size);
  if (new_arr == NULL) return false;
  vector->arr = new_arr;
  vector->size = new_size;
  return true;
}