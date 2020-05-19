#ifndef VD_H
#define VD_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct dynamic_vector {
  char** arr;
  size_t* str_lenghts;
  size_t size;
  size_t n_elements;
} vector_t;

// Inicializa los miembros del vector, crea un arreglo de tamaño inicial
// INITIAL_SIZE
bool str_vector_init(vector_t* vector);

// Agrega una cadena de str_len bytes comenzando en element
// El usuario debe asegurarse de pasar correctamente
// la longitud y el inicio de manera de tener direcciones permitidas
// Devuelve un boolean indicando si se pudo agregar el elemento, false
// en caso contrario(como por ejemplo que falle la redimension)
bool str_vector_append(vector_t* vector, char* element, size_t str_len);

// Devuelve un puntero al inicio de la cadena almacenada en la posicion
// index del vector
char* str_vector_get(vector_t* vector, size_t index);

// Redimensiona el vector para poder almacenar new_size elementos
// Devuelve un booleano indicando si fue exitoso el realocamiento de memoria
bool str_vector_rsz(vector_t* vector, size_t new_size);

// Devuelve la cantidad de elementos del vector
size_t str_vector_len(vector_t* vector);

// Devuelve la longitud del elemento del vector ubicado en index
// Tener en cuenta que puede diferir del contenido de strlen dado
// que se permiten caracteres nulos como parte de los elementos
size_t str_vector_elem_len(vector_t* vector, size_t index);

// Joinea los elemntos del vector uniendolos mediante separadores vacios
// (concatena los elementos en una sola cadena), la cadena devuelta es
// null-terminated
char* str_vector_join(vector_t* vector);

// Libera los recursos del vector
void str_vector_destroy(vector_t* vector);

// Vacia el vector de manera que quede con 0 elementos
void str_vector_clear(vector_t* vector);

#endif
