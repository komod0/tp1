#ifndef CLIENT_H
#define CLIENT_H

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

#include "common_socket.h"
#include "common_protocol.h"

typedef struct client {
  uint32_t msg_id;
  socket_t socket;
  protocol_t protocol;
}client_t;

// Inicializa las componentes del client_t, inicialmente setea el file
// descriptor asociado a su socket en -1
void client_init(client_t* client);

// Intenta conectar al cliente al host y servicio indicado, retorna 0 en caso
// de exito 1 en caso contrario
int client_connect(client_t* client, const char* host, const char* service);

// Intenta mandar msg_len bytes desde "buffer" mediante el socket
// del cliente, devuelve 0 en caso de exito, 1 en otro caso.
int client_send(client_t* client, const char* buffer, size_t msg_len);

// Intenta recibir len bytes almacenandolos en buffer mediante el socket
// del cliente, devuelve 0 en caso de exito, 1 en otro caso.
int client_recv(client_t* client, char* buffer, size_t len);

// Recibe una respuesta de buff_len bytes y la almacena en buff,
// devuelve 0 en caso de exito, 1 en otro caso.
int client_get_response(client_t* client, char* buff, size_t buff_len);

// Imprime por stdout la cadena almacenada en buff
void client_print_response(client_t* client, char* buff);

// Destruye el client y cierra el file descriptor asociado al socket
int client_disconnect(client_t* client);

// Entry point del cliente, se encarga de llevar el flujo principal del cliente
// leer linea encodearla, mandarla, recibir respuesta e imprimirla, asi como de
// destruir y cerrar sockets correspondientes
int client_run(client_t* client, FILE* input);

#endif // CLIENT_H
