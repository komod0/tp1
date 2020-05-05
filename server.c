#include<errno.h>
#include<stdio.h>
#include<string.h>

#include "server.h"

enum exit_codes{SUCCESS, ERROR};

void server_init(server_t* server) {
  socket_init(&server->socket);
  socket_init(&server->peer);
}

int server_bind_listen(server_t* server, const char* service) {
  if(socket_bind(&server->socket, service) != SUCCESS) {
    return ERROR;
  }
  return socket_listen(&server->socket);
}

int server_send(server_t* server, const char* msg, size_t len) {
  if(socket_send(&server->socket, msg, len) == -1) {
    return ERROR;
  }
  return SUCCESS;
}

int server_recv(server_t* server, char* msg, size_t len) {
  if(socket_recv(&server->peer, msg, len) == -1) {
    return ERROR;
  }
  return SUCCESS;
}

int server_accept(server_t* server) {
  return socket_accept(&server->socket, &server->peer);
}

int server_run(server_t* server) {
  
}

int server_disconnect(server_t* server) {
  return socket_destroy(&server->peer) || socket_destroy(&server->peer);
}

