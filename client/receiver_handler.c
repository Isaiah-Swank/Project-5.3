#include "receiver_handler.h"

// Function to set up the client socket
int rec_setup_client_socket(const char *ip, int port)
{
    int server_socket = 0;

    //write the proper code to connect to the socket created in the do while in client_handler.c 
    //avoid possible issues with using same address - mutex locks maybe?

    return server_socket;
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

    int server_port = atoi(server_port_str);
    if (server_port <= 0)
    {
        fprintf(stderr, "Invalid server port number.\n");
        return NULL;
    }
printf("calls to rec setup client\n");
    int client_socket = rec_setup_client_socket(server_ip, server_port);
    if (client_socket == -1)
    {
        fprintf(stderr, "Failed to set up client socket.\n");
        return NULL;
    }

    while (1)
    {
        Message message;
printf("gets to receive message call\n");
        // Read the entire message
        recieve_message(client_socket, &message);

        // Convert network byte order to host byte order
        //message.chat_node.ip = ntohl(message.chat_node.ip);
        //message.chat_node.port = ntohs(message.chat_node.port);
printf("in rec handler: %s %d\n", message.chat_node.name, message.type);
        // Handle the message based on its type
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
