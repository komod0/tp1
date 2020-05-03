#ifndef DYN_BUFFER_H
#define DYN_BUFFER_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct dynamic_buffer {
  char* arr;
  size_t size;
  size_t n_of_elements;
} d_buff_t;

bool d_buff_init(d_buff_t* buff, size_t size);

bool d_buff_append(d_buff_t* dest, const char* src, size_t n);

bool d_buff_redim(d_buff_t* buff, size_t new_size);

char* d_buff_generate_str(d_buff_t* buff);

void d_buff_empty(d_buff_t* buff);

void d_buff_destroy(d_buff_t* buff);

#endif  // DYN_BUFFER_H