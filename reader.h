#ifndef READER_H
#define READER_H

#define BUFF_SIZE 32

#include <stdio.h>

#include "dyn_buffer.h"

typedef struct reader {
  FILE* input;
  char* start;
  char* end;
  size_t bytes_read;
  d_buff_t d_buff;
  char buffer[BUFF_SIZE];  // Sino hay que usar el heap
} reader_t;

bool reader_init(reader_t* reader, FILE* input);

char* reader_readline(reader_t* reader);

bool reader_is_at_eof(reader_t* reader);

#endif