#include "receiver_handler.h"

// Function to set up the client socket
int rec_setup_client_socket(const char *ip, int port) {
    int sock;
    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sock);
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(sock);
        return -1;
    }

    return sock;
}

// Function to receive messages from the server
void *receive_from_server(void *arg) {
    Properties *props = (Properties *)arg;
    char *ip = property_get_property(props, "IP");
    char *port_str = property_get_property(props, "PORT");
    if (!ip || !port_str) {
        fprintf(stderr, "IP or Port is not specified in properties.\n");
        return NULL;
    }
    int port = atoi(port_str); // Convert port to integer
    if (port <= 0) {
        fprintf(stderr, "Invalid port number.\n");
        return NULL;
    }

    int client_socket = setup_client_socket(ip, port);
    if (client_socket == -1) {
        fprintf(stderr, "Failed to set up client socket.\n");
        return NULL;
    }

    Message message;

    // Main loop to receive messages
    while (1) {
        if (!receive_message(client_socket, &message)) {  // Using the correct spelling as in your header file
            switch (message.type) {
                case NOTE:
                    printf("Note from %s: %s\n", message.chat_node.name, message.note);
                    break;
                case JOINING:
                    printf("%s has joined the chat\n", message.chat_node.name);
                    break;
                case LEFT:
                    printf("%s has left the chat\n", message.chat_node.name);
                    break;
                case SHUTDOWN:
                    printf("Server is shutting down.\n");
                    close(client_socket);
                    return NULL;
                default:
                    printf("Received unknown type of message: %d.\n", message.type);
            }
        } else {
            fprintf(stderr, "Failed to receive message. Exiting.\n");
            close(client_socket);
            return NULL;
        }
    }
}
