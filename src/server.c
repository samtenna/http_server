#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <stdbool.h>

#include "server.h"

#define DEFAULT_PORT "8080"
#define DEFAULT_BUFFER_LENGTH 512

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

    while (true) {
        handle_connection(client_socket);
    }

    return true;
}

bool handle_connection(SOCKET client_socket) {
    char buffer[DEFAULT_BUFFER_LENGTH];
    int result;
    int send_result;
    int buffer_length = DEFAULT_BUFFER_LENGTH;

    do {
        result = recv(client_socket, buffer, buffer_length, 0);
        if (result > 0) {
            printf("%d bytes received\n", result);

            // echo the buffer back to the sender
            send_result = send(client_socket, buffer, result, 0);
            if (send_result == SOCKET_ERROR) {
                printf("Failed to send: %d\n", WSAGetLastError());
                closesocket(client_socket);
                return false;
            }

            printf("%d bytes sent.\n", send_result);
        } else if (result == 0) {
            printf("Closing connection.\n");
        } else {
            printf("recv failed: %d\n", WSAGetLastError());
            closesocket(client_socket);
            WSACleanup();
            return false;
        }
    } while (result > 0);

    return true;
}
