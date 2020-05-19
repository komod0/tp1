#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "common_utils.h"

void vectorize_msg(vector_t* vector, char* msg) {
  char* next;
  int pos_act = 0;
  char* act_chr = msg;
  size_t str_length = strlen(msg);
  // Obtengo destino, path, interfaz
  for (int i = 0; i < 3; i++) {
    next = strpbrk(act_chr, " ");
    str_vector_append(vector, strndup(act_chr, next-act_chr), next-act_chr);
    pos_act += next - act_chr + 1; // +1 por el separador
    act_chr = next + 1;
  }

  // Obtengo el metodo y los N parametros
  while (pos_act < str_length){
    next = strpbrk(act_chr, "(,)");
    pos_act += next-act_chr + 1; // Por el separador el +1
    if (next-act_chr == 0) break;
    str_vector_append(vector, strndup(act_chr, next-act_chr), next-act_chr);
    act_chr = msg + pos_act;
  }
}

void free_vector_elems(vector_t* vector) {
  for (int i = 0; i < str_vector_len(vector); i++) {
    free(str_vector_get(vector, i));
  }
}

size_t str_vec_total_size(vector_t* vector) {
  size_t total_size = 0;
  for (int i = 0; i < str_vector_len(vector); i++) {
    total_size += str_vector_elem_len(vector, i);
  }
  return total_size;
}

char* strndup(const char* src, size_t n) {
  char* result = malloc(n + 1);
  memcpy(result, src, n);
  result[n] = '\0';
  return result;
}

uint32_t get_8_aligned_padding(size_t len) {
  return (8 - len % 8) % 8;
}

uint32_t get_8_aligned_size(int i) {
  i += get_8_aligned_padding(i);
  return i;
}

uint32_t get_8_aligned_len(char* s) {
  uint32_t size = strlen(s) + 1;
  return get_8_aligned_size(size);
}

char* concat_n_times(char* s, int n) {
  int aux = 0;
  char* new_str = malloc(sizeof(char)*strlen(s)*n + 1);
  if (new_str == NULL) return NULL;
  for (int i = 0; i < n; i++) {
    memcpy(new_str + aux, s, strlen(s));
    aux += strlen(s);
  }
  new_str[aux] = '\0';
  return new_str;
}
