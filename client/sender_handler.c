#include "sender_handler.h"


// Function to set up the client socket
int setup_client_socket(const char *ip, int port) 
{
    int sock;
    struct sockaddr_in server_addr;

    // create empty socket to connect to server
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) 
    {
        perror("socket");
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    // get machine readable IP address
    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) 
    {
        perror("inet_pton");
        close(sock);
        return -1;
    }

    // connect to the socket
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) 
    {
        perror("connect");
        close(sock);
        return -1;
    }

    return sock;
}

void *send_to_server(void *arg) 
{
    // initialize variables
    Properties *props = (Properties *)arg;
    char input[256];
    char* ip = property_get_property(props, "SERVER_IP");
    char* client_port = property_get_property(props, "CLIENT_PORT");
    char* client_name = property_get_property(props, "MY_NAME");
    char* port_str = property_get_property(props, "SERVER_PORT");
    int port = atoi(port_str); // Convert port string to an integer
    int client_socket = 0;

    // bad IP or PORT read
    if (!ip || !port_str) 
    {
        fprintf(stderr, "IP address or port not specified in properties.\n");
        return NULL;
    }

    // no socket created
    if (client_socket == -1) 
    {
        fprintf(stderr, "Failed to setup client socket.\n");
        return NULL;
    }

    printf("Ready to connect!\n");

    // Create a new chat node for the client
    int client_ip_int = atoi(client_ip); 
    int client_port_int = atoi(client_port);
    ChatNode* new_chat_node = chat_node_new(client_ip_int, client_port_int, client_name);

    // input reading loop
    while (fgets(input, sizeof(input), stdin)) 
    {
        // read input
        input[strcspn(input, "\n")] = 0;  // Strip newline character

        Message* message = NULL;
        ssize_t sent_message = -1;

        // send JOIN message to the server
        if (strncmp(input, "JOIN", 4) == 0) 
        {
             new_chat_node = chat_node_new(client_ip_int, client_port_int, client_name);
             client_socket = setup_client_socket(ip, port);
             if (client_socket == -1) 
             {
             fprintf(stderr, "Failed to setup client socket.\n");
             return NULL;
             } 
            message = new_message(JOIN, new_chat_node, input);
            sent_message = send_message(client_socket, message);
        } 
        // send LEAVE message to the server
        else if (strncmp(input, "LEAVE", 5) == 0) 
        {
             client_socket = setup_client_socket(ip, port);
             if (client_socket == -1) 
             {
             fprintf(stderr, "Failed to setup client socket.\n");
             return NULL;
             } 
            message = new_message(LEAVE, new_chat_node, input);
            sent_message = send_message(client_socket, message);
            close(client_socket);
        } 
        // send SHUTDOWN message to the server
        else if (strncmp(input, "SHUTDOWN", 8) == 0) 
        {
             client_socket = setup_client_socket(ip, port);
             if (client_socket == -1) 
             {
             fprintf(stderr, "Failed to setup client socket.\n");
             return NULL;
             } 
            message = new_message(SHUTDOWN, new_chat_node, input);
            sent_message = send_message(client_socket, message);
            close(client_socket);
            exit(EXIT_SUCCESS);
            break;  // Exit after SHUTDOWN
        } 
        // send SHUTDOWN_ALL message to the server
        else if (strncmp(input, "SHUTDOWN ALL", 13) == 0) {
             client_socket = setup_client_socket(ip, port);
             if (client_socket == -1) 
             {
             fprintf(stderr, "Failed to setup client socket.\n");
             return NULL;
             } 
            message = new_message(SHUTDOWN_ALL, new_chat_node, input);
            sent_message = send_message(client_socket, message);
            close(client_socket);
            exit(EXIT_SUCCESS);
            break; 
        } 
        // else treat the input as a NOTE
        else 
        {
             client_socket = setup_client_socket(ip, port);
             if (client_socket == -1) 
             {
             fprintf(stderr, "Failed to setup client socket.\n");
             return NULL;
             } 
            message = new_message(NOTE, new_chat_node, input);
            sent_message = send_message(client_socket, message);
        }

        // bad message send
        if (sent_message < 0) 
        {
            perror("Failed to send message");
            break;  // Exit on send error
        }
    }  
    
    // clean up
    close(client_socket);
    return NULL;
}


