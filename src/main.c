#include <WS2tcpip.h>
#include <WinSock2.h>
#include <stdlib.h>

#include "server.h"

#pragma comment(lib, "Ws2_32.lib")

int main() {
  HttpServer *server = create_server(NULL);
  start_server(server);
  cleanup_server(&server);

  return EXIT_SUCCESS;
}
