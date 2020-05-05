#ifndef SERVER_H
#define SERVER_H

#include <stdlib.h> 

#include "socket.h"

typedef struct server {
  socket_t socket;
  socket_t peer;
}server_t;

int server_accept(server_t* server);

void server_init(server_t* server);

int server_bind_listen(server_t* server, const char* service);

int server_send(server_t* server, const char* msg, size_t len);

int server_recv(server_t* server, char* msg, size_t len);

int server_run(server_t* server);


#endif