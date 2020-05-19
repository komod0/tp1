#ifndef UTILS_H
#define UTILS_H

#include<stdint.h>

#include "common_str_vector.h"

// Recibe un mensaje de la forma %s %s %s %s([%s, %s, ...])
// e inserta sus partes(%s) en el vector vector
void vectorize_msg(vector_t* vector, char* msg);

// Recibe una cadena y devuelve el tamaño de la misma alineado a 8 bytes
uint32_t get_8_aligned_len(char* s);

// Recibe un entero y lo alinea a 8 bytes
uint32_t get_8_aligned_size(int i);

// Devuelve el padding necesario para que una cadena de tamaño len quede
// alineada a 8 bytes
uint32_t get_8_aligned_padding(size_t len);

// Implementacion propia de la funcion strndup, devuelve una copia en memoria
// dinamica de la cadena apuntada por src, copia exactamente n bytes.
char* strndup(const char* src, size_t n);

// Recibe un vector y libera cada uno de sus elementos aplicandole free
void free_vector_elems(vector_t* vector);

// Recibe un elemento y devuelve el tamaño acumulado de sus elementos
// (el tamaño de los elementos concatenados)
size_t str_vec_total_size(vector_t* vector);

// Recibe una cadena s y la concatena consigo misma n veces el resultado 
// es una cadena en el heap que la representa
char* concat_n_times(char* s, int n);

#endif
