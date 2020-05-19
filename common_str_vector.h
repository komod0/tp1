#ifndef VD_H
#define VD_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct dynamic_vector {
  char** arr;
  size_t* str_lenghts;
  size_t size;
  size_t n_elements;
} vector_t;

bool str_vector_init(vector_t* vector);

bool str_vector_append(vector_t* vector, char* element, size_t str_len);

char* str_vector_get(vector_t* vector, size_t index);

bool str_vector_rsz(vector_t* vector, size_t new_size);

size_t str_vector_len(vector_t* vector);

size_t str_vector_elem_len(vector_t* vector, size_t index);

char* str_vector_join(vector_t* vector);

void str_vector_destroy(vector_t* vector);

void str_vector_clear(vector_t* vector);

#endif
