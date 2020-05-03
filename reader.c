#include "reader.h"
#include <string.h>
#include <stdbool.h>

#define DYN_BUFFER_SIZE 64
#define BUFF_SIZE 32


bool reader_init(reader_t* reader, FILE* input) {
  reader->start = 0;
  reader->eol_offset = BUFF_SIZE - 1;
  reader->input = input;
  memset(reader->buffer, 0, BUFF_SIZE);
  reader->buffer[BUFF_SIZE-1] = '\0'; // Para facilitar strchr
  return d_buff_init(reader->d_buff, DYN_BUFFER_SIZE);
}

char* reader_readline(reader_t* reader) {
  bool is_there_eol = false;
  char* next_occurrence;
  size_t bytes_read;
  while (!is_there_eol && !feof(reader->input)) {
    if (reader->start == 0) { // Significa que no me "quedo" nada en el buffer
      bytes_read = fread(reader->buffer, 1,BUFF_SIZE-1, reader->input);
      if (bytes_read < )
    }
    next_occurrence = strchr(reader->buffer + reader->start, '\n');// USE MEMCHR!!
    is_there_eol = (next_occurrence != NULL);
    if (is_there_eol) {
        reader->eol_offset = next_occurrence - reader->buffer;
    }
    d_buff_append(reader->d_buff, reader->buffer + reader->start,
     reader->eol_offset - reader->start);
    
    if (is_there_eol && reader->eol_offset < BUFF_SIZE - 1) {
      reader->start = reader->eol_offset + 1; // Para saltearse el \n
    } else {
      reader->start = 0;
    }
  }
}

bool reader_is_at_eof(reader_t* reader) {
  return (feof(reader->input) != 0);
}
