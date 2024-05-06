#include "main.h"

pthread_mutex_t mutex_client_socket;
pthread_mutex_t mutex_chat_node_list;

ChatNodeList* chat_nodes;



// Main function of the server
int main(int argc, char** argv) 
{
    // Variables for storing socket descriptors
    int server_socket;
    struct sockaddr_in server_address; // Struct to hold server address information
    int client_socket;
    int yes=1;  // Variable for setsockopt function to enable address reuse

    // Create a new list to manage chat nodes
    chat_nodes = chat_nodes_new();

    // Path to the server properties file
    char *properties_file = "server.properties";
    // Load server configuration properties
    Properties *properties = property_read_properties(properties_file);

    // Ignore SIGPIPE signals (sent when writing to a disconnected socket)
    signal(SIGPIPE, SIG_IGN);
    
    // Create a TCP socket to listen for incoming connections
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Allow the socket to reuse the address if the server is restarted
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(char));
    
    // Set server address parameters
    server_address.sin_family = AF_INET;  // Internet address family
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);  // Accept any incoming interface
    server_address.sin_port = htons(atoi(property_get_property(properties, "SERVER_PORT")));  // Convert port number from host to network byte order
    
    // Bind the socket to the server address
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) != 0) 
    {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }
    
    // Listen on the socket for incoming connections
    if (listen(server_socket, NUM_CONNECTIONS) != 0) 
    {
        perror("Error listening on socket");
        exit(EXIT_FAILURE);
    }

    // Output the port number the server is listening on
    printf("Server listening on port %d\n", ntohs(server_address.sin_port));

    // Main server loop to handle incoming connections
    while (TRUE) 
    {
        // Lock the mutex before accepting new connections to synchronize access to client sockets
        pthread_mutex_lock(&mutex_client_socket);

        // Accept an incoming connection
        client_socket = accept(server_socket, NULL, NULL);
        if (client_socket < 0) 
        {
            perror("Accept failed");
            pthread_mutex_unlock(&mutex_client_socket);
            continue;
        }
        //printf("Client has connected!\n");

        // Allocate memory for a new socket to handle client communication
        int *socket_ptr = malloc(sizeof(int));
        if (socket_ptr == NULL) 
        {
            perror("Failed to allocate memory for client socket");
            close(client_socket);
            pthread_mutex_unlock(&mutex_client_socket);
            continue;
        }
        *socket_ptr = client_socket;

        // Create a new thread to handle communication with the connected client
        pthread_t thread;
        if (pthread_create(&thread, NULL, talk_to_client, socket_ptr) != 0) 
        {
            perror("Failed to create thread");
            free(socket_ptr);
            close(client_socket);
        }

        // Detach the thread to allow for independent operation
        if (pthread_detach(thread) != 0) 
        {
            perror("Failed to detach thread");
        }

        // Unlock the mutex after handling the client connection
        pthread_mutex_unlock(&mutex_client_socket);
    }

    // Close the server socket when done
    close(server_socket);
    return 0;
}
