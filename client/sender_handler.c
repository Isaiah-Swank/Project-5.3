#include "sender_handler.h"
#include "message.h"
#include <stdio.h>
#include <string.h>

void *send_to_server(void *arg) {
    Properties *props = (Properties *)arg;
    char input[256];
    int client_socket = setup_client_socket(props->ip, props->port);

    while (1) {
        fgets(input, sizeof(input), stdin);

        if (strncmp(input, "JOIN", 4) == 0) {
            send_join_request(client_socket, input);
        } else if (strncmp(input, "LEAVE", 5) == 0) {
            send_leave_request(client_socket);
        } else if (strncmp(input, "SHUTDOWN", 8) == 0) {
            send_shutdown_request(client_socket);
            break;
        } else if (strncmp(input, "SHUTDOWN ALL", 13) == 0) {
            send_shutdown_all_request(client_socket);
            break;
        } else {
            send_note(client_socket, input);
        }
    }

    close(client_socket);
    return NULL;
}
