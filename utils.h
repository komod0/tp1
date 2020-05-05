#ifndef UTILS_H
#define UTILS_H

#include "dynamic_vector.h"
#include "string.h"

void vectorize_msg(vector_t* vector, char* msg);

uint32_t get_8_aligned_size(char* s);

uint32_t get_8_aligned_padding(size_t len);

char* concat_n_times(char* s, int n);

#endif