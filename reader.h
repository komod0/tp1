#ifndef READER_H
#define READER_H

#define BUFF_SIZE 32

#include <stdio.h>
#include "dyn_buffer.h"

typedef struct reader{
  FILE* input;
  size_t start;
  size_t eol_offset;
  d_buff_t* d_buff;
  char buffer[BUFF_SIZE]; // Sino hay que usar el heap
} reader_t;

bool reader_init(reader_t* reader, FILE* input);

char* reader_readline(reader_t* reader);

bool reader_is_at_eof(reader_t* reader);


#endif