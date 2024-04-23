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







void send_join_request(int socket, const char* message) {
    if (message == NULL) {
        fprintf(stderr, "send_join_request: message parameter is NULL\n");
        return;
    }

    // Prepare the message for sending; you might want to add additional formatting
    char buffer[1024];
    int len = snprintf(buffer, sizeof(buffer), "%s\n", message);  // Append newline if protocol requires it

    if (len <= 0 || len >= sizeof(buffer)) {
        fprintf(stderr, "send_join_request: message formatting error or buffer overflow\n");
        return;
    }

    // Send the message to the server
    ssize_t bytes_sent = send(socket, buffer, strlen(buffer), 0);
    if (bytes_sent < 0) {
        perror("send_join_request: failed to send message");
    } else {
        printf("send_join_request: sent '%s' to server\n", buffer);
    }
}







void send_leave_request(int socket) {
    const char* cmd = "LEAVE\n";  // LEAVE command, assuming newline is needed

    // Send the LEAVE command to the server
    ssize_t bytes_sent = send(socket, cmd, strlen(cmd), 0);
    if (bytes_sent < 0) {
        perror("send_leave_request: failed to send LEAVE command");
    } else {
        printf("send_leave_request: 'LEAVE' command sent to server\n");
    }
}




void send_shutdown_request(int socket) {
    const char* cmd = "SHUTDOWN\n";  // Example shutdown command

    ssize_t bytes_sent = send(socket, cmd, strlen(cmd), 0);
    if (bytes_sent < 0) {
        perror("send_shutdown_request: Failed to send SHUTDOWN command");
    } else {
        printf("send_shutdown_request: 'SHUTDOWN' command sent\n");
    }
}






void send_shutdown_all_request(int socket) {
    const char* cmd = "SHUTDOWN ALL\n";  // SHUTDOWN ALL command

    ssize_t bytes_sent = send(socket, cmd, strlen(cmd), 0);
    if (bytes_sent < 0) {
        perror("send_shutdown_all_request: Failed to send SHUTDOWN ALL command");
    } else {
        printf("send_shutdown_all_request: 'SHUTDOWN ALL' command sent\n");
    }
}






void send_note(int socket, const char* message) {
    if (message == NULL) {
        fprintf(stderr, "send_note: Provided message is NULL\n");
        return;
    }

    ssize_t bytes_sent = send(socket, message, strlen(message), 0);
    if (bytes_sent < 0) {
        perror("send_note: Failed to send note");
    } else {
        printf("send_note: Note sent successfully\n");
    }
}
