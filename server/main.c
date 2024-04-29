#include "main.h"

pthread_mutex_t mutex_client_socket;
pthread_mutex_t mutex_chat_node_list;

ChatNodeList* chat_nodes;
/************************************************************************
 * MAIN
 ************************************************************************/


int main(int argc, char** argv) {
    int server_socket;                 // Descriptor of server socket
    struct sockaddr_in server_address; // For naming the server's listening socket
    int client_socket;
    int yes=1;  

    // Initialize member list
    chat_nodes = chat_nodes_new();

    char *properties_file = "server.properties";
    Properties *properties = property_read_properties(properties_file);

    // Sent when client disconnected
    signal(SIGPIPE, SIG_IGN);
    
    // Create unnamed network socket for server to listen on
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(char));
    
    // Name the socket (making sure the correct network byte ordering is observed)
    server_address.sin_family = AF_INET;  // Accept IP addresses
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);  // Accept clients on any interface
    server_address.sin_port = htons(atoi(property_get_property(properties, "SERVER_PORT")));  // Port to listen on
    
    // Bind unnamed socket to a particular port
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) != 0) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }
    
    // Listen for client connections (pending connections get put into a queue)
    if (listen(server_socket, NUM_CONNECTIONS) != 0) {
        perror("Error listening on socket");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", ntohs(server_address.sin_port));

    // Server loop
    while (TRUE) {
        pthread_mutex_lock(&mutex_client_socket);

        client_socket = accept(server_socket, NULL, NULL);
        if (client_socket < 0) {
            perror("Accept failed");
            pthread_mutex_unlock(&mutex_client_socket);
            continue;
        }
        printf("Client has connected!\n");

        // Dynamically allocate memory for the client socket to pass to the thread
        int *socket_ptr = malloc(sizeof(int));
        if (socket_ptr == NULL) {
            perror("Failed to allocate memory for client socket");
            close(client_socket);
            pthread_mutex_unlock(&mutex_client_socket);
            continue;
        }
        *socket_ptr = client_socket;

        pthread_t thread;
        if (pthread_create(&thread, NULL, talk_to_client, socket_ptr) != 0) {
            perror("Failed to create thread");
            free(socket_ptr);
            close(client_socket);
        }

        if (pthread_detach(thread) != 0) {
            perror("Failed to detach thread");
        }

        pthread_mutex_unlock(&mutex_client_socket);
    }

    close(server_socket);
    return 0;
}



/*
int main(int argc, char** argv) {
    
    int server_socket;                 // descriptor of server socket
    struct sockaddr_in server_address; // for naming the server's listening socket
    int client_socket;
    int yes=1;  
	
	//initialize member list
	chat_nodes = chat_nodes_new();

    char *properties_file = "server.properties";

    Properties *properties = property_read_properties( properties_file );

    // sent when ,client disconnected
    signal(SIGPIPE, SIG_IGN);
    
    // create unnamed network socket for server to listen on
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(char));
    
    // name the socket (making sure the correct network byte ordering is observed)
    server_address.sin_family      = AF_INET;           // accept IP addresses
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); // accept clients on any interface
    server_address.sin_port        = htons(atoi(property_get_property(properties, "SERVER_PORT")));       // port to listen on
    
    // binding unnamed socket to a particular port
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) != 0) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }
    
    // listen for client connections (pending connections get put into a queue)
    if (listen(server_socket, NUM_CONNECTIONS) != 0) {
        perror("Error listening on socket");
        exit(EXIT_FAILURE);
        printf("Server listening on port 61209");
    }

    // server loop
    while (TRUE) 
    {
	    pthread_mutex_lock(&mutex_client_socket);

	    client_socket = accept(server_socket, NULL, NULL);
	    printf("Client has connected!\n");
	    
		pthread_t thread;
		
		//handle client
		if (pthread_create(&thread, NULL, talk_to_client, (void *)&client_socket))
		{
			printf("Server with PID %d error creating thread", getpid());
			exit(EXIT_FAILURE);
		}
	
	        //save thread memory before closing
		if (pthread_detach(thread))
		{
			printf("Server with PID %d error detaching thread", getpid());
			exit(EXIT_FAILURE);
		}					
    }
}
*/