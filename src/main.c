#include <stdlib.h>
#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#include "server.h"

int main() {
    if (!initialise_winsock()) {
        return EXIT_FAILURE;
    }

    if (!initialise_server()) {
        printf("Server failed to start.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
