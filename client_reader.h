#ifndef READER_H
#define READER_H

#define BUFF_SIZE 32

#include <stdio.h>

#include "common_str_vector.h"

typedef struct reader {
  FILE* input;
  char* start;
  char* end;
  size_t bytes_read;
  vector_t vector;
  char buffer[BUFF_SIZE];  // Sino hay que usar el heap
} reader_t;

bool reader_init(reader_t* reader, FILE* input);

char* reader_readline(reader_t* reader);

bool reader_is_at_eof(reader_t* reader);

void reader_destroy(reader_t* reader);

#endif