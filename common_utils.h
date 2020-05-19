#ifndef UTILS_H
#define UTILS_H

#include<stdint.h>

#include "common_str_vector.h"

void vectorize_msg(vector_t* vector, char* msg);

uint32_t get_8_aligned_len(char* s);

uint32_t get_8_aligned_size(int i);

uint32_t get_8_aligned_padding(size_t len);

char* strndup(const char* src, size_t n);

void free_vector_elems(vector_t* vector);

size_t str_vec_total_size(vector_t* vector);

char* concat_n_times(char* s, int n);

#endif
