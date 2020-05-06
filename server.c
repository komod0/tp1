#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>

#include "server.h"

#define CHUNK_SIZE 16
#define RESPONSE "OK\n"
#define RESPONSE_LEN 3

enum exit_codes{SUCCESS, ERROR};

void server_init(server_t* server) {
  socket_init(&server->socket);
  socket_init(&server->peer);
  protocol_init(&server->protocol);
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

int server_recv_call(server_t* server, char* msg) {
  int bytes_read;
  bytes_read = socket_recv(&server->peer, msg, CHUNK_SIZE);
  if(bytes_read == -1) {
    return -1;
  } else if(bytes_read == 0) return 0;
  d_buff_append(&server->protocol.d_buff, msg, CHUNK_SIZE);
  uint32_t arr_len = protocol_decode_arr_len(msg);
  uint32_t body_len = protocol_decode_body_len(msg);
  arr_len += (8 - arr_len % 8) % 8; // Sumo tam del padding
  arr_len += body_len; // Tam total
  size_t to_read;
  while(arr_len > 0) {
    to_read = (arr_len >= CHUNK_SIZE) ? CHUNK_SIZE : arr_len;
    arr_len -= CHUNK_SIZE;
    if(socket_recv(&server->peer, msg, to_read) == -1) {
      return ERROR;
    }
    d_buff_append(&server->protocol.d_buff, msg, to_read);
  }
  return 1;
}

int server_accept(server_t* server) {
  return socket_accept(&server->socket, &server->peer);
}

int server_run(server_t* server) {
  char buff[CHUNK_SIZE];
  int status = 1;
  while(status == 1) {
    status = server_recv_call(server, buff);
    if(status != 1) break;
    protocol_decode_and_print(&server->protocol);
    status = server_respond(server);
    if(status != 1) break;
  }
  if (server_disconnect(server) != 0 || status == -1) {
    return ERROR;
  }
  return SUCCESS;
}

int server_respond(server_t* server) {
  server_send(server, RESPONSE, RESPONSE_LEN);
}

int server_disconnect(server_t* server) {
  protocol_destroy(&server->protocol);
  return socket_destroy(&server->peer) || socket_destroy(&server->peer);
}

