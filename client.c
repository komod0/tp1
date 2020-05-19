#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "client.h"
#include "common_protocol.h"
#include "client_reader.h"
#include "common_socket.h"

#define RECV_BUFFER_SIZE 3

enum EXIT_CODES{SUCCESS, ERROR};

void client_init(client_t* client) {
  socket_init(&client->socket);
  protocol_init(&client->protocol);
  client->msg_id = 1;
}

int client_send(client_t* client, const char* buffer, size_t msg_len) {
  int bytes_sent;
  bytes_sent = socket_send(&client->socket, buffer, msg_len);
  return (bytes_sent < 0 ? ERROR : SUCCESS);
}

int client_recv(client_t* client, char* buffer, size_t len) {
  int bytes_recv;
  bytes_recv = socket_recv(&client->socket, buffer, len);
  return (bytes_recv < 0) ? ERROR : SUCCESS;
}

int client_connect(client_t* client, const char* host, const char* service) {
  return socket_connect(&client->socket, host, service);
}

int client_run(client_t* client, FILE* input) {
  reader_t reader;
  char *msg, *line;
  size_t msg_len;
  char recv_buff[RECV_BUFFER_SIZE];
  if (!reader_init(&reader, input)) return ERROR;
  do {
    line = reader_readline(&reader);
    if (line == NULL) return 1;  // socket_accept(&server->socket, &server->peer)TODO
    msg = protocol_encode(&client->protocol, line, client->msg_id, &msg_len);
    free(line);

    if (client_send(client, msg, msg_len) == ERROR) return ERROR;
    free(msg);
    if(client_get_response(client, recv_buff, RECV_BUFFER_SIZE) == -1) {
      return ERROR;
    }
    client_print_response(client, recv_buff);
    client->msg_id++;
  } while (!reader_is_at_eof(&reader));

  reader_destroy(&reader);
  return client_disconnect(client);
}

int client_get_response(client_t* client, char* buff, size_t buff_len) {
  return socket_recv(&client->socket, buff, buff_len);
}

void client_print_response(client_t* client, char* buff) {
  printf("0x%08X: %s", client->msg_id, buff);
}

int client_disconnect(client_t* client) {
  protocol_destroy(&client->protocol);
  return socket_destroy(&client->socket);
}