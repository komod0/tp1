#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>

#include "server.h"
#include "common_utils.h"
#include "common_str_vector.h"

#define CHUNK_SIZE 16
#define RESPONSE "OK\n"
#define RESPONSE_LEN 3

enum exit_codes{SUCCESS, ERROR, FINISHED};

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
  if(socket_send(&server->peer, msg, len) == -1) {
    return ERROR;
  }
  return SUCCESS;
}

int server_recv_call(server_t* server, char* msg, size_t buff_size) {
  int bytes_read;
  bytes_read = socket_recv(&server->peer, msg, buff_size);
  if(bytes_read == -1) {
    return ERROR;
  } else if(bytes_read == 0) return FINISHED;

  str_vector_append(&server->protocol.vector, 
                    strndup(msg, buff_size), buff_size);
  
  uint32_t arr_len = protocol_decode_arr_len(msg);
  uint32_t body_len = protocol_decode_body_len(msg);

  arr_len += get_8_aligned_padding(arr_len); // Sumo tam del padding
  arr_len += body_len; // Tam total
  size_t to_read;

  while(arr_len > 0) {
    to_read = (arr_len >= buff_size) ? buff_size : arr_len;
    arr_len -= to_read;
    if(socket_recv(&server->peer, msg, to_read) == -1) {
      return ERROR;
    }
    str_vector_append(&server->protocol.vector, strndup(msg, to_read), to_read);
  }
  return SUCCESS;
}

int server_accept(server_t* server) {
  return socket_accept(&server->socket, &server->peer);
}

int server_run(server_t* server) {
  char buff[CHUNK_SIZE];
  int status = SUCCESS;
  while(status == SUCCESS) {
    status = server_recv_call(server, buff, CHUNK_SIZE);
    if(status != SUCCESS) break;
    protocol_decode_and_print(&server->protocol);
    status = server_respond(server);
    if(status != SUCCESS) break;

  }
  if (server_disconnect(server) != SUCCESS || status == ERROR) {
    return ERROR;
  }
  return SUCCESS;
}

int server_respond(server_t* server) {
  return server_send(server, RESPONSE, RESPONSE_LEN);
}

int server_disconnect(server_t* server) {
  protocol_destroy(&server->protocol);
  int status = socket_destroy(&server->peer);
  return socket_destroy(&server->socket) || status;
}

