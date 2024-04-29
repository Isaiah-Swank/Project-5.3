#include "sender_handler.h"
#include "message.h"
#include <stdio.h>
#include <string.h>

#include "sender_handler.h"
#include "message.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>  // For close function


// Function to set up the client socket
int setup_client_socket(const char *ip, int port) {
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

void *send_to_server(void *arg) {
    Properties *props = (Properties *)arg;
    char input[256];
    char* ip = property_get_property(props, "SERVER_IP");
    char* client_port = property_get_property(props, "CLIENT_PORT");  // If needed for something specific
    char* client_name = property_get_property(props, "MY_NAME");
    char* port_str = property_get_property(props, "SERVER_PORT");
    int port = atoi(port_str); // Convert port string to an integer

    if (!ip || !port_str) {
        fprintf(stderr, "IP address or port not specified in properties.\n");
        return NULL;
    }

    int client_socket = setup_client_socket(ip, port);
    if (client_socket == -1) {
        fprintf(stderr, "Failed to setup client socket.\n");
        return NULL;
    }

    printf("Ready to connect!\n");

    // Create a new chat node for the client
    // Assuming IP and port from properties need to be converted to integers
    // Typically you would fetch an actual IP address for client_ip, not use atoi directly on the IP string
    int client_ip_int = atoi(client_port); // This is likely incorrect; you would normally use inet_pton if IP needed
    int client_port_int = atoi(client_port);
    ChatNode* new_chat_node = chat_node_new(client_ip_int, client_port_int, client_name);

    while (fgets(input, sizeof(input), stdin)) {
        input[strcspn(input, "\n")] = 0;  // Strip newline character

        Message* message = NULL;
        ssize_t sent_message = -1;

        if (strncmp(input, "JOIN", 4) == 0) {
            message = new_message(JOIN, new_chat_node, input);
            printf("%d, %s, %s\n", message->type, message->chat_node.name, message->note);
            sent_message = send_message(client_socket, message);
            printf("%s: Has JOINED the chat.\n", client_name);
        } else if (strncmp(input, "LEAVE", 5) == 0) {
            message = new_message(LEAVE, new_chat_node, input);
            sent_message = send_message(client_socket, message);
            printf("%s: Has LEFT the chat.\n", client_name);
            break;  // Exit after LEAVE
        } else if (strncmp(input, "SHUTDOWN", 8) == 0) {
            message = new_message(SHUTDOWN, new_chat_node, input);
            sent_message = send_message(client_socket, message);
            printf("%s: Has SHUTDOWN.\n", client_name);
            break;  // Exit after SHUTDOWN
        } else if (strncmp(input, "SHUTDOWN ALL", 13) == 0) {
            message = new_message(SHUTDOWN_ALL, new_chat_node, input);
            sent_message = send_message(client_socket, message);
            printf("%s: Has SHUTDOWN ALL.\n", client_name);
            break;  // Exit after SHUTDOWN ALL
        } else {
            message = new_message(NOTE, new_chat_node, input);
            sent_message = send_message(client_socket, message);
            printf("%s: %s\n", client_name, input);
        }

        if (sent_message < 0) {
            perror("Failed to send message");
            break;  // Exit on send error
        }
    }  
    
    close(client_socket);  // Ensure the socket is closed on function exit
    return NULL;
}


