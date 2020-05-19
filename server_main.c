#include "server.h"


int main(int argc, const char* argv[]) {
  if (argc < 2) return 1;
  server_t server;
  server_init(&server);
  server_bind_listen(&server, argv[1]);
  server_accept(&server);
  return server_run(&server);
}
