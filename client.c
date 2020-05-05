#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "client.h"
#include "dyn_buffer.h"
#include "protocol.h"
#include "reader.h"
#include "socket.h"

#define RECV_BUFFER_SIZE 3

enum EXIT_CODES{SUCCESS, ERROR};

void client_init(client_t* client) {
  socket_init(&client->socket);
  client->msg_id = 1;
}

int client_run(client_t* client, FILE* input) {
  reader_t reader;
  char *msg, *line, *line_aux;
  size_t msg_len;
  // strsep modifica line,con line_aux libero el bloque facil despues
  if (!reader_init(&reader, input)) return ERROR;

  do {
    line = reader_readline(&reader);
    if (line == NULL) return 1;  // TODO
    line_aux = line;
    msg = protocol_encode(line, client->msg_id, &msg_len);
    if (client_send(client, msg, msg_len) == ERROR) return ERROR;
    client->msg_id++;

  } while (!reader_is_at_eof(&reader));
}

int client_send(client_t* client, const void* buffer, size_t msg_len) {
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

int client_prueba(client_t* client, FILE* input) {
  reader_t reader;
  char *msg, *line, *line_aux;
  size_t msg_len;
  // strsep modifica line,con line_aux libero el bloque facil despues
  if (!reader_init(&reader, input)) return ERROR;

  do {
    line = reader_readline(&reader);
    if (line == NULL) return 1;  // socket_accept(&server->socket, &server->peer)TODO
    line_aux = line;
    msg = protocol_encode(line, client->msg_id, &msg_len);
    free(line_aux);
    if (client_send(client, msg, msg_len) == ERROR) return ERROR;
    client->msg_id++;
  } while (!reader_is_at_eof(&reader));
}

int client_disconnect(client_t* client) {
  return socket_destroy(&client->socket);
}