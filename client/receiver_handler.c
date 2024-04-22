#include "receiver_handler.h"
#include "message.h"
#include <stdio.h>

void *receive_from_server(void *arg) {
    Properties *props = (Properties *)arg;
    int client_socket = setup_client_socket(props->ip, props->port);
    Message message;

    while (1) {
        if (!receive_message(client_socket, &message)) {
            switch (message.type) {
                case NOTE:
                    printf("Note from %s: %s\n", message.sender, message.content);
                    break;
                case JOIN:
                    printf("%s has joined the chat\n", message.sender);
                    break;
                case LEAVE:
                    printf("%s has left the chat\n", message.sender);
                    break;
                case SHUTDOWN:
                    printf("Server is shutting down.\n");
                    close(client_socket);
                    return NULL;
                default:
                    printf("Received unknown type of message.\n");
            }
        } else {
            fprintf(stderr, "Failed to receive message. Exiting.\n");
            close(client_socket);
            return NULL;
        }
    }
}
