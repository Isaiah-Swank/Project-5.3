#include "receiver_handler.h"

// Function to set up the client socket
int rec_setup_client_socket(const char *ip, int port)
{
    int sock;
    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("socket");
        return -1;
    }

    //set server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    //convert ip from string to binary 
    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0)
    {
        perror("inet_pton");
        close(sock);
        return -1;
    }

    //connect to the socket
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("connect");
        close(sock);
        return -1;
    }

    return sock;
}


void *receive_from_server(void *arg)
{
    Properties *props = (Properties *)arg;

    // Retrieve and validate necessary properties
    char *server_ip = property_get_property(props, "SERVER_IP");
    char *server_port_str = property_get_property(props, "SERVER_PORT");
    if (!server_ip || !server_port_str)
    {
        fprintf(stderr, "Server IP or Port is not specified in properties.\n");
        return NULL;
    }

    //convert port from string to int
    int server_port = atoi(server_port_str);
    if (server_port <= 0)
    {
        fprintf(stderr, "Invalid server port number.\n");
        return NULL;
    }

    //get and connect to socket
    int client_socket = rec_setup_client_socket(server_ip, server_port);
    if (client_socket == -1)
    {
        fprintf(stderr, "Failed to set up client socket.\n");
        return NULL;
    }

    while (1)   //always be ready to receive
    {
        Message message;
        int bytes_read = 0;
        int total_bytes_to_read = sizeof(Message);

        // Read the entire message
        char *buffer = (char *)&message;
        while (bytes_read < total_bytes_to_read)
        {
            int result = read(client_socket, buffer + bytes_read, total_bytes_to_read - bytes_read);
            if (result == 0) // Connection closed
            {
                printf("Server closed the connection.\n");
                break;
            }
            else if (result == -1)
            {
                perror("Read error");
                break;
            }
            bytes_read += result;
        }

        if (bytes_read < total_bytes_to_read) // Incomplete message read or read error
        {
            break;
        }

        // Convert network byte order to host byte order
        message.chat_node.ip = ntohl(message.chat_node.ip);
        message.chat_node.port = ntohs(message.chat_node.port);

        // print message on client side
        switch (message.type)
        {
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
                break;
        }
    }

    close(client_socket); // Close the socket at the end if not already closed
    return NULL;
}
