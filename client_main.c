#include<stdio.h>

#include "client.h"

int stdin_select(FILE** input, int argc, const char* argv[]) {
  int exit_code = 0;

  if (argc == 4) {
    *input = fopen(argv[3], "r");
    if (*input == NULL) {
      fprintf(stderr, "Error al abrir el archivo\n");
      exit_code = 1;
    }
  } else if(argc == 3) {
    *input = stdin;
  } else {
    printf("Cantidad de argumentos incorrecta\n");
    exit_code = 1;
  }

  return exit_code;
}

int main(int argc, const char* argv[]) {
  FILE* input;
  if (stdin_select(&input, argc, argv) == 1) return 1;
  client_t client;
  client_init(&client);
  client_connect(&client, argv[1], argv[2]);
  client_prueba(&client, input);
  return 0;
}