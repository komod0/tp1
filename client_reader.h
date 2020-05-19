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

// Inicializa los miembros del reader y setea la fuente de input a utilizar
bool reader_init(reader_t* reader, FILE* input);

// Lee texto hasta encontrarse con un fin de linea(\n) o un EOF, la cadena
// devuelta debe ser liberada.
char* reader_readline(reader_t* reader);

// Devuelve un booleano indicando si se termino de recorrer el texto
bool reader_is_at_eof(reader_t* reader);

// Destruye el lector, cualquier valor que se haya almacenado dentro de el 
// vector es responsabilidad del usuario liberarlo si hace falta
void reader_destroy(reader_t* reader);

#endif
