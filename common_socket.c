#define _POSIX_C_SOURCE 200112L

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "common_socket.h"

#define LISTEN_QUEUE_SIZE 10

enum exit_codes{SUCCESS, ERROR};

int _socket_get_addr_list(socket_t* self, struct addrinfo **addr_list,
    const char* host, const char* service, int ai_flags) {
  int status = 0;
  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_flags = ai_flags;
  hints.ai_socktype = SOCK_STREAM;

  status = getaddrinfo(host, service, &hints, addr_list);
  if(status != 0) {
    fprintf(stderr, "Error in gettaddrinfo: %s", gai_strerror(status));
    return ERROR;
  }
  return SUCCESS;
}

int socket_connect(socket_t* self, const char* host, const char* service) {
  struct addrinfo *adr_l, *ptr;
  if (_socket_get_addr_list(self, &adr_l, host, service, 0) != SUCCESS) {
    return ERROR;
  }
  bool connected = false;
  int status = 0;
  for(ptr=adr_l; !connected && ptr; ptr = ptr->ai_next) {
    self->fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (self->fd == -1) {
      fprintf(stderr, "self error: %s\n", strerror(errno));
    } else {
      status = connect(self->fd, ptr->ai_addr, ptr->ai_addrlen);
      if (status == -1) {
        fprintf(stderr, "Connect error: %s\n", strerror(errno));
      }
      connected = (status == 0); // 0 --> Todo bien
    }
  }
  freeaddrinfo(adr_l);
  return (connected ? SUCCESS : ERROR);
}

void socket_init(socket_t* self) {
  self->fd = -1; // No hay
}

int socket_bind(socket_t* self, const char* serv) {
  struct addrinfo *adr_l, *ptr;
  int status;
  bool binded = false;
  if(_socket_get_addr_list(self, &adr_l, NULL, serv, AI_PASSIVE) != SUCCESS) {
    return ERROR;
  }
  for(ptr=adr_l; !binded && ptr; ptr = ptr->ai_next) {
    self->fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (self->fd == -1) {
      fprintf(stderr, "Socket error: %s\n", strerror(errno));
    } else {
      status = bind(self->fd, ptr->ai_addr, ptr->ai_addrlen);
      if (status == -1) {
        fprintf(stderr, "Binding error: %s\n", strerror(errno));
      }
      binded = (status == 0); // 0 --> Todo bien
    }
  }
  freeaddrinfo(adr_l);
  return (binded ? SUCCESS : ERROR);
}

int socket_listen(socket_t* self) {
  if(listen(self->fd, LISTEN_QUEUE_SIZE) == -1) {
    fprintf(stderr, "Listen error: %s\n", strerror(errno));
    return ERROR;
  }
  return SUCCESS;
}

int socket_accept(socket_t* self, socket_t* peer) {
  peer->fd = accept(self->fd, NULL, NULL);
  if(peer->fd == -1) {
    fprintf(stderr, "Accept error: %s\n", strerror(errno));
    return ERROR;
  }
  return SUCCESS;
}

int socket_send(socket_t* self, const char* buffer, size_t length) {
  size_t bytes_sent = 0;
  int s;
  while(bytes_sent < length) {
    s = send(self->fd, buffer + bytes_sent, length - bytes_sent, MSG_NOSIGNAL);
    if (s < 0) {
      fprintf(stderr, "Send error: %s\n", strerror(errno));
      return -1;
    } else if(s == 0) {
      return 0;
    } else {
      bytes_sent += s;
    }
  }
  return bytes_sent;
}

int socket_recv(socket_t* self, char* buffer, size_t length) {
  size_t bytes_recv = 0;
  int s;
  while(bytes_recv < length) {
    s = recv(self->fd, buffer + bytes_recv, length - bytes_recv, 0);
    if (s < 0) {
      fprintf(stderr, "Recv error: %s\n", strerror(errno));
      return -1;
    } else if(s == 0) {
      return 0;
    } else {
      bytes_recv += s;
    }
  }
  return bytes_recv;
}

int socket_destroy(socket_t* socket) {
  int status = shutdown(socket->fd, SHUT_RDWR);
  return close(socket->fd) || status;
}