#include "server.h"


int main(int argc, const char* argv[]) {
  if(argc < 3) return 1;
  server_t server;
  server_init(&server);
  server_bind_listen(&server, argv[1]);
  server_accept(&server);
  server_run()
  return 0;
}