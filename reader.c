#include "reader.h"
#include <string.h>
#include <stdbool.h>

#define DYN_BUFFER_SIZE 64
#define BUFF_SIZE 32


bool reader_init(reader_t* reader, FILE* input) {
  reader->start = 0;
  reader->end = BUFF_SIZE - 1;
  reader->bytes_read = 0;
  reader->input = input;
  memset(reader->buffer, 0, BUFF_SIZE);
  return d_buff_init(reader->d_buff, DYN_BUFFER_SIZE);
}

char* reader_readline(reader_t* reader) {
  bool is_there_eol = false;
  while (!is_there_eol && !feof(reader->input)) {
    if (reader->start == reader->buffer) { // No me "quedo" nada en el buffer
      reader->bytes_read = fread(reader->buffer,1,BUFF_SIZE - 1, reader->input);
      reader->end = reader->start + reader->bytes_read;
    }
    reader->end = memchr(reader->start, '\n', reader->end - reader->start);
    is_there_eol = (reader->end != NULL);
    if (reader->end == NULL) {
      reader->end = reader->buffer + reader->bytes_read;
    }
    d_buff_append(reader->d_buff,reader->start,reader->end - reader->start - 1);
    if (is_there_eol && (reader->end - reader->buffer) < BUFF_SIZE - 1) {
      reader->start = reader->end + 1; // Para saltearse el \n
    } else {
      reader->start = reader->buffer;
    }
  }
  return d_buff_generate_str(reader->d_buff);
}

bool reader_is_at_eof(reader_t* reader) {
  return (feof(reader->input) != 0);
}
