// client_handler.c

#include "client_handler.h"
#include <errno.h>

static int client_sockets[100];  // array to hold client sockets
static int num_clients = 0;
static pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

//add client for socket array
void add_client(int client_sock) {
    pthread_mutex_lock(&clients_mutex);
    if (num_clients < 100) {
        client_sockets[num_clients++] = client_sock;
    }
    pthread_mutex_unlock(&clients_mutex);
}

//remove client for socket array
void remove_client(int client_sock) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < num_clients; i++) {
        if (client_sockets[i] == client_sock) {   //if we are removing current
            client_sockets[i] = client_sockets[--num_clients];  // Move last to current
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}



void broadcast_message(const Message *msg, int sender_sock) {
    pthread_mutex_lock(&clients_mutex);

    //loop through client array
    for (int i = 0; i < num_clients; i++) {
        if (client_sockets[i] != sender_sock) {  // Do not send back to the sender
            //send message to current client
            if (send(client_sockets[i], msg, sizeof(*msg), 0) < 0) {
                perror("Failed to send message to client");
            }
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void *talk_to_client(void *arg) {
    if (!arg) {
        fprintf(stderr, "Error: NULL pointer received in talk_to_client\n");
        return NULL;
    }

    int client_socket = *((int *) arg);
    free(arg);  // Free the dynamically allocated socket descriptor
    arg = NULL;  // Prevent use after free

    pthread_mutex_unlock(&mutex_client_socket);
    add_client(client_socket);  // Add the client to the list when connection is established

    Message message;

    //continually receive message but only do stuff it is real message
    while (recieve_message(client_socket, &message) > 0) {

        //convert ip and port back to host
        message.chat_node.ip = ntohl(message.chat_node.ip);
        message.chat_node.port = ntohs(message.chat_node.port);

        switch (message.type) {
            case JOIN:
                message.type = JOINING;  // Modify the message type if necessary
                printf("Client %s has joined.\n", message.chat_node.name);
                // Broadcast joining message
                broadcast_message(&message, client_socket);
                break;
            case LEAVE:
                printf("Client %s has left the chat.\n", message.chat_node.name);
                broadcast_message(&message, client_socket);  // Optionally notify others of the departure
                close(client_socket);  // Close the connection on LEAVE
                remove_client(client_socket);  // Remove client from list
                return NULL;  // Exit the thread function
            case SHUTDOWN:
                break;
            case SHUTDOWN_ALL:
                printf("Shutting down server or client as requested by %s.\n", message.chat_node.name);
                close(client_socket);  // Close the connection on SHUTDOWN
                remove_client(client_socket);  // Remove client from list
                return NULL;  // Exit the thread function
            case NOTE:
                printf("Note from %s: %s\n", message.chat_node.name, message.note);
                broadcast_message(&message, client_socket);  // Broadcast note to all other clients
                break;
            default:
                fprintf(stderr, "Unknown message type received: %d\n", message.type);
                break;
        }
    }

    if (errno == ECONNRESET || errno == ETIMEDOUT) {
        perror("Connection reset by peer or timeout");
    } else {
        perror("Failed to receive message or connection closed");
    }
    
    close(client_socket);  // Ensure the socket is closed if we exit the loop
    remove_client(client_socket);  // Remove client from list on any other exit
    return NULL;
}
