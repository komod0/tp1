#ifndef DYN_BUFFER_H
#define DYN_BUFFER_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct dynamic_buffer {
  char* arr; 
  size_t size;
  size_t n_of_elements;
} buff_t;

bool buff_init(buff_t* buff, size_t size);

bool buff_append(buff_t* dest, const char* src, size_t n);

bool buff_redim(buff_t* buff, size_t new_size);

void buff_empty(buff_t* buff);

#endif // DYN_BUFFER_H