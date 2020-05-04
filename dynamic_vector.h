#ifndef VD_H
#define VD_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct dynamic_vector {
  void** arr;
  size_t size;
  size_t n_elements;
} vector_t;

bool vector_init(vector_t* vector);

bool vector_append(vector_t* vector, void* element);

void* vector_get(vector_t* vector, size_t index);

bool vector_resize(vector_t* vector, size_t new_size);

#endif