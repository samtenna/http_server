#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <stdbool.h>

#include "server.h"

#define DEFAULT_PORT "8080"

bool initialise_winsock() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed: %d\n", result);
        return false;
    }

    return true;
}

bool initialise_server() {
    struct addrinfo* addr_info_result = NULL, *ptr = NULL, hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    int result = getaddrinfo(NULL, DEFAULT_PORT, &hints, &addr_info_result);
    if (result != 0) {
        printf("getaddrinfo failed: %d\n", result);
        WSACleanup();
        return false;
    }

    SOCKET listen_socket = socket(addr_info_result->ai_family, addr_info_result->ai_socktype, addr_info_result->ai_protocol);
    if (listen_socket == INVALID_SOCKET) {
        printf("Error at socket(): %d\n", WSAGetLastError());
        FreeAddrInfo(addr_info_result);
        WSACleanup();
        return false;
    }

    if (bind(listen_socket, addr_info_result->ai_addr, (int)addr_info_result->ai_addrlen) == SOCKET_ERROR) {
        printf("Bind failed with error: %d\n", WSAGetLastError());
        FreeAddrInfo(addr_info_result);
        closesocket(listen_socket);
        WSACleanup();
        return false;
    }

    FreeAddrInfo(addr_info_result);

    if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listen failed with error: %d\n", WSAGetLastError());
        closesocket(listen_socket);
        WSACleanup();
        return false;
    }

    // TODO: potentially move this part into a separate function to act as the 'main loop'
    SOCKET client_socket = accept(listen_socket, NULL, NULL);
    if (client_socket == INVALID_SOCKET) {
        printf("Accept failed: %d\n", WSAGetLastError());
        closesocket(client_socket);
        WSACleanup();
        return false;
    }

    return true;
}
