#ifndef SERVER_H
#define SERVER_H

#include <stdlib.h> 

#include "common_socket.h"
#include "common_protocol.h"

typedef struct server {
  socket_t socket;
  socket_t peer;
  protocol_t protocol;
}server_t;

// Acepta una conexion entrante
// Devuelve 0 en caso de exito, 1 en caso de error
int server_accept(server_t* server);

// Inicializa el servidor
void server_init(server_t* server);

// Enlaza al servidor al servicio especificado por service y comienza
// a escuchar conexiones en el mismo
// Devuelve 0 en caso de exito, 1 en caso de error.
int server_bind_listen(server_t* server, const char* service);

// Manda len bytes desde msg, en caso de exito devuelve 0, 1 en caso contrario
int server_send(server_t* server, const char* msg, size_t len);

// Recibe un mensaje (con el formato especificado) de a trozos de buff_size
// bytes, parsea la longitud total del mismo y va guardando
// las partes en un vector
int server_recv_call(server_t* server, char* msg, size_t buff_size);

// Maneja el flujo principal del servidor, recibe mensajes, los decodifica
// responde y manda a destruir el servidor una vez terminado
int server_run(server_t* server);

// Envia un mensaje determinado al cliente, devuelve 0 en caso de exito
// 1 en caso contrario
int server_respond(server_t* server);

// Desconecta el servidor, desconcetando los sockets y sus files descriptors
// asociados
int server_disconnect(server_t* server);

#endif
