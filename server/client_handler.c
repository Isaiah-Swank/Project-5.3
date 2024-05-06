// client_handler.c

#include "client_handler.h"
#include <errno.h>

static int client_sockets[100];
static int num_clients = 0;
static pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

// add client to the list
void add_client(int client_sock) 
{
    pthread_mutex_lock(&clients_mutex);
    if (num_clients < 100) 
    {
        client_sockets[num_clients++] = client_sock;
    }
    pthread_mutex_unlock(&clients_mutex);
}

// remove client from the list
void remove_client(int client_sock) 
{
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < num_clients; i++) 
    {
        if (client_sockets[i] == client_sock) 
        {
            client_sockets[i] = client_sockets[--num_clients];  // Move last to current
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}


// send the message to ALL the clients
void broadcast_message(const Message *msg, int sender_sock) 
{
    pthread_mutex_lock(&clients_mutex);
    if( msg->type != SHUTDOWN_ALL )
    {
	    for (int i = 0; i < num_clients; i++) 
	    {
		if (client_sockets[i] != sender_sock) {  // Do not send back to the sender
		    if (send(client_sockets[i], msg, sizeof(*msg), 0) < 0) 
		    {
		        perror("Failed to send message to client");
		        // Consider removing the client if sending fails
		        remove_client(client_sockets[i]);
		        // Note: Decrement i since we are removing current client
		        i--;  
		    }
		}
	    }
    }
    else
    {
      exit(EXIT_SUCCESS);
    }
    pthread_mutex_unlock(&clients_mutex);
}

// talk to the client function
void *talk_to_client(void *arg) 
{
    if (!arg) {
        fprintf(stderr, "Error: NULL pointer received in talk_to_client\n");
        return NULL;
    }

    int client_socket = *((int *) arg);
    free(arg);  // Free the dynamically allocated socket descriptor
    arg = NULL;  // Prevent use after free

    pthread_mutex_unlock(&mutex_client_socket);
    add_client(client_socket);

    Message message;
    while (recieve_message(client_socket, &message) > 0) 
    {
        message.chat_node.ip = ntohl(message.chat_node.ip);
        message.chat_node.port = ntohs(message.chat_node.port);

        switch (message.type) 
        {
            case JOIN:
                message.type = JOINING;
                printf("Client %s has joined.\n", message.chat_node.name);
                // Broadcast joining message
                broadcast_message(&message, client_socket);
                break;
            case LEAVE:
                printf("Client %s has left the chat.\n", message.chat_node.name);
                broadcast_message(&message, client_socket);  // Optionally notify others of the departure
                close(client_socket);  // Close the connection on LEAVE
                remove_client(client_socket);  // Remove client from list
                break;
            case SHUTDOWN:
                printf("Client %s has SHUTDOWN.\n", message.chat_node.name);
                broadcast_message(&message, client_socket);  // Optionally notify others of the departure
                close(client_socket);  // Close the connection on LEAVE
                remove_client(client_socket);  // Remove client from list
                break;
            case SHUTDOWN_ALL:
                printf("Shutting down server or client as requested by %s.\n", message.chat_node.name);
                close(client_socket);  // Close the connection on SHUTDOWN
                remove_client(client_socket);  // Remove client from list
                exit(EXIT_SUCCESS);
                return NULL;  // Exit the thread function
                break;
            case NOTE:
                printf("Note from %s: %s\n", message.chat_node.name, message.note);
                broadcast_message(&message, client_socket);  // Broadcast note to all other clients
                break;
            default:
                fprintf(stderr, "Unknown message type received: %d\n", message.type);
                break;
        }
    }

    close(client_socket);  // Ensure the socket is closed if we exit the loop
    remove_client(client_socket);  // Remove client from list on any other exit
    return NULL;
}
