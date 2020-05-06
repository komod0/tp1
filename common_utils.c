#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "common_utils.h"

void vectorize_msg(vector_t* vector, char* msg) {
  char* token;
  // Obtengo destino, path, interfaz
  for(int i = 0; i < 3; i++) {
    vector_append(vector, strsep(&msg," "));
  }

  // Obtengo el metodo y los N parametros
  while( (token = strsep(&msg,"(),")) != NULL ) {
    if(strlen((char*)token)> 0) {  // Las cadenas vacias me arruinan la vida
      vector_append(vector, token);
    }
  }
}

uint32_t get_8_aligned_padding(size_t len) {
  return (8 - len % 8) % 8;
}

uint32_t get_8_aligned_size(int i) {
  i += get_8_aligned_padding(i);
}

uint32_t get_8_aligned_len(char* s) {
  uint32_t size = strlen(s) + 1;
  return get_8_aligned_size(size);
}

char* concat_n_times(char* s, int n) {
  int aux = 0;
  char* new_str = malloc(sizeof(char)*strlen(s)*n + 1);
  if (new_str == NULL) return NULL;
  for(int i = 0; i < n; i++) {
    memcpy(new_str + aux, s, strlen(s));
    aux += strlen(s);
  }
  new_str[aux] = '\0';
  return new_str;
}
