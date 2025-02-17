#include "client_reader.h"
#include "common_str_vector.h"
#include "common_utils.h"

#include <string.h>
#include <stdbool.h>

#define DYN_BUFFER_SIZE 64
#define BUFF_SIZE 32


bool reader_init(reader_t* reader, FILE* input) {
  memset(reader->buffer, 0, BUFF_SIZE);
  reader->start = reader->buffer;
  reader->end = reader->buffer + (BUFF_SIZE - 1);
  reader->bytes_read = 0;
  reader->input = input;
  return str_vector_init(&reader->vector);
}

char* reader_readline(reader_t* reader) {
  bool is_there_eol = false;
  char* result;
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
    result = strndup(reader->start, reader->end - reader->start);
    str_vector_append(&reader->vector, result, reader->end - reader->start);
    if (is_there_eol && (reader->end - reader->buffer) < BUFF_SIZE - 1) {
      reader->start = reader->end + 1; // Para saltearse el \n
    } else {
      reader->start = reader->buffer;
    }
    reader->end = reader->buffer + reader->bytes_read;
  }
  char* line = str_vector_join(&reader->vector);
  free_vector_elems(&reader->vector);
  str_vector_clear(&reader->vector);
  return line;
}

bool reader_is_at_eof(reader_t* reader) {
  return (feof(reader->input) != 0);
}

void reader_destroy(reader_t* reader) {
  str_vector_destroy(&reader->vector);
}
