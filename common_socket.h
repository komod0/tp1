#ifndef SOCKET_H
#define SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

typedef struct socket { 
  int fd;
} socket_t;

int socket_connect(socket_t* self, const char* host, const char* service);

void socket_init(socket_t* self);

int socket_bind(socket_t* self, const char* serv);

int socket_listen(socket_t* self);

int socket_accept(socket_t* self, socket_t* peer);

int socket_send(socket_t* self, const char* buffer, size_t length);

int socket_recv(socket_t* self, char* buffer, size_t length);

int socket_destroy(socket_t* socket);

#endif // SOCKET_H
