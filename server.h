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

int server_accept(server_t* server);

void server_init(server_t* server);

int server_bind_listen(server_t* server, const char* service);

int server_send(server_t* server, const char* msg, size_t len);

int server_recv_call(server_t* server, char* msg);

int server_run(server_t* server);

int server_respond(server_t* server);

int server_disconnect(server_t* server);

#endif