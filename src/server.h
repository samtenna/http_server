#pragma once

#include <WinSock2.h>
#include <stdbool.h>

#define DEFAULT_PORT "8080"
#define CONNECTION_QUEUE_LENGTH 10
#define MAX_THREADS 32

typedef struct HttpServer {
  SOCKET socket;
  PCSTR port;
  int thead_count;
  HANDLE thread_handles[MAX_THREADS];
} HttpServer;

HttpServer *create_server(PCSTR port);
void cleanup_server(HttpServer **server);

bool start_server(HttpServer *server);
bool process_request(char *request, char *response);
