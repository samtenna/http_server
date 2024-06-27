#include <WS2tcpip.h>
#include <WinSock2.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server.h"

HttpServer *create_server(PCSTR port) {
  HttpServer *server = malloc(sizeof(HttpServer));
  if (server == NULL) {
    printf("Failed to allocate server.\n");
    return NULL;
  }

  if (port == NULL) {
    port = DEFAULT_PORT;
  }
  server->port = port;
  server->socket = SOCKET_ERROR;

  return server;
}

void cleanup_server(HttpServer **server) {
  closesocket((*server)->socket);

  free(*server);
  *server = NULL;
}

bool start_server(HttpServer *server) {
  WSADATA wsa_data;
  if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
    printf("WSAStartup failed. Does your machine support winsock?: %d\n",
           WSAGetLastError());
    return false;
  }

  server->socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server->socket == SOCKET_ERROR) {
    printf("Failed to open socket: %d\n", WSAGetLastError());
    return false;
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = 0;
  addr.sin_port = htons(atoi(server->port));
  if (bind(server->socket, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
    printf("Failed to bind socket: %d\n", WSAGetLastError());
    return false;
  }

  if (listen(server->socket, CONNECTION_QUEUE_LENGTH) != 0) {
    printf("Failed to begin listening: %d\n", WSAGetLastError());
    return false;
  }

  SOCKET client_socket = accept(server->socket, NULL, NULL);
  if (client_socket == INVALID_SOCKET) {
    // TODO: Change this so the server doesn't crash if the connection fails.
    printf("Failed to accept client connection: %d\n", WSAGetLastError());
    return false;
  }

  char request[512] = {0};
  int bytes_received = recv(client_socket, request, 512, 0);
  printf("Request: %s\n", request);

  char *response = malloc(sizeof(char) * 512);
  if (!process_request(request, response)) {
    printf("Failed to read requested file.\n");
    return false;
  }

  printf("Seinding:\n%s", response);

  send(client_socket, response, 512, 0);

  return true;
}

bool process_request(const char *request, char *response) {
  if (memcmp(request, "GET / ", 6)) {
    // Return server root.
    FILE *file;
    if (fopen_s(&file, "index.html", "r") != 0) {
      printf("Failed to open index.html.\n");
      return false;
    }

    // TODO: some error checking here since fread doesn't differentiate between
    // EOF and error
    fread(response, 1, 512, file);
  } else {
    // Return server root.
    FILE *file;
    if (fopen_s(&file, "404.html", "r") != 0) {
      printf("Failed to open 404.html.\n");
      return false;
    }

    // TODO: some error checking here since fread doesn't differentiate between
    // EOF and error
    fread(response, 1, 512, file);
  }

  return true;
}
