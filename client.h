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

void client_init(client_t* client);

int client_connect(client_t* client, const char* host, const char* service);

int client_send(client_t* client, const void* buffer, size_t msg_len);

int client_recv(client_t* client, char* buffer, size_t len);

int client_prueba(client_t* client, FILE* input);

int client_run(client_t* client, FILE* input);

#endif // CLIENT_H