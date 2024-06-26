#include <stdbool.h>
#include <WinSock2.h>

bool initialise_winsock();

bool initialise_server();

bool handle_connection(SOCKET client_socket);
