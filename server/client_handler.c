// client_handler.c

#include "client_handler.h"
#include <errno.h>
void *talk_to_client(void *arg) {
    if (!arg) {
        fprintf(stderr, "Error: NULL pointer received in talk_to_client\n");
        return NULL;
    }

    int client_socket = *((int *) arg);
    free(arg);  // Free the dynamically allocated socket descriptor
    arg = NULL;  // Prevent use after free

    pthread_mutex_unlock(&mutex_client_socket);

    Message message;
    while (recieve_message(client_socket, &message) > 0) {  // Keep receiving messages
        message.chat_node.ip = ntohl(message.chat_node.ip);
        message.chat_node.port = ntohs(message.chat_node.port);

printf("%d, %s, %s\n", message.type, message.chat_node.name, message.note);
printf("%d, %d, %s\n", message.chat_node.ip, message.chat_node.port, message.chat_node.name);

        switch (message.type) {

                case JOIN:////////////////////////////////////////////////////////////////
                int socket_to_chat_node;
                struct addrinfo hints, *server_info;
                char port_string[6];
                char ip_string[INET_ADDRSTRLEN];
                pthread_mutex_lock( &mutex_chat_node_list );

                message.type = JOINING;  // Modify the message type if necessary
                printf("Client %s has joined.\n", message.chat_node.name);

                // Here you could add the chat node to a list or similar structure if needed
            ChatNodeListElement *current = chat_nodes->first;

       if (current != NULL)     //check for first node
       {
            //send joining message to clients
            do {            
printf("sending message to %s\n", current->chat_node.name);
                memset(&hints, 0, sizeof(hints));
                hints.ai_socktype = SOCK_STREAM;
                hints.ai_family = AF_INET;
                sprintf(port_string, "%u", current->chat_node.port);
 
                inet_ntop(AF_INET, &(current->chat_node.ip), ip_string, INET_ADDRSTRLEN);

                getaddrinfo(ip_string, port_string, &hints, &server_info);

                socket_to_chat_node = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

                if( connect(socket_to_chat_node, server_info->ai_addr, server_info->ai_addrlen) == -1)
                {
                    perror("Error connecting to client!");
                }

                //send message through socket 
                send_message( socket_to_chat_node, &message );

                close(socket_to_chat_node);
     
                if (current->next != NULL)
                {
                    current = current->next;
                }
            } while (current->next);
       }

            //add new node to chat nodes list
            chat_nodes_add_node( chat_nodes, &message.chat_node);
            pthread_mutex_unlock( &mutex_chat_node_list );

                break;
            case LEAVE:
                printf("Client %s has left the chat.\n", message.chat_node.name);
                close(client_socket);  // Close the connection on LEAVE
                return NULL;  // Exit the thread function
            case SHUTDOWN:
            case SHUTDOWN_ALL:
                printf("Shutting down server or client as requested by %s.\n", message.chat_node.name);
                close(client_socket);  // Close the connection on SHUTDOWN
                return NULL;  // Exit the thread function
            case NOTE:
                printf("Note from %s: %s\n", message.chat_node.name, message.note);
                // Implement what should happen on NOTE, like logging the note or displaying it
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
    return NULL;
}





/*
void *talk_to_client(void *arg) {

    int client_socket = *((int *) arg);
    Message message;

    pthread_mutex_unlock( &mutex_client_socket );

    //read message from client 
    ssize_t received_message = recieve_message( client_socket, &message);
    
    message.chat_node.ip = ntohl(message.chat_node.ip);
    message.chat_node.port = ntohs(message.chat_node.port);
    
    printf("%d, %s, %s\n", message.type, message.chat_node.name, message.note);
    printf("%d, %d, %s\n", message.chat_node.ip, message.chat_node.port, message.chat_node.name);
    


    //close connection
    close(client_socket);

    switch(message.type) 
    {
        case JOIN:
            int socket_to_chat_node;
            struct addrinfo hints, *server_info;
            char port_string[6];
            char ip_string[INET_ADDRSTRLEN];

            pthread_mutex_lock( &mutex_chat_node_list );

            message.type = JOINING;   //set type to JOINING

            ChatNodeListElement *current = chat_nodes->first;

            //send joining message to clients
            while (current->next)
            {
                memset(&hints, 0, sizeof(hints));
                hints.ai_socktype = SOCK_STREAM;
                hints.ai_family = AF_INET;
                sprintf(port_string, "%u", current->chat_node.port);
                
                inet_ntop(AF_INET, &(current->chat_node.ip), ip_string, INET_ADDRSTRLEN);

                getaddrinfo(ip_string, port_string, &hints, &server_info);

                socket_to_chat_node = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

                if( connect(socket_to_chat_node, server_info->ai_addr, server_info->ai_addrlen) )
                {
                    perror("Error connecting to server!");
                }

                //send message through socket 
                send_message( socket_to_chat_node, &message );

                close(socket_to_chat_node);
                current = current->next;
            }

            //add new node to chat nodes list
            chat_nodes_add_node( chat_nodes, &message.chat_node);
            
            pthread_mutex_unlock( &mutex_chat_node_list );
            break;
        
        case LEAVE:
        {
            int socket_to_chat_node;
            struct addrinfo hints, *server_info;
            char port_string[6];

            pthread_mutex_lock( &mutex_chat_node_list );

            //remove chat node from list
            chat_nodes_remove_node(chat_nodes, &message.chat_node);
            
            message.type = LEFT;   //set type to LEFT

            ChatNodeListElement *current = chat_nodes->first;

            //send joining message to clients
            while (current->next)
            {
                memset(&hints, 0, sizeof(hints));
                hints.ai_socktype = SOCK_STREAM;
                hints.ai_family = AF_INET;
                sprintf(port_string, "%u", current->chat_node.port);
                
                inet_ntop(AF_INET, &(current->chat_node.ip), ip_string, INET_ADDRSTRLEN);

                getaddrinfo(ip_string, port_string, &hints, &server_info);

                socket_to_chat_node = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

                if( connect(socket_to_chat_node, server_info->ai_addr, server_info->ai_addrlen) )
                {
                    perror("Error connecting to server!");
                }

                //send message through socket 
                send_message( socket_to_chat_node, &message );
                
                close(socket_to_chat_node);
                current = current->next;
            }
            
            pthread_mutex_unlock( &mutex_chat_node_list );
            break;
        }
        case NOTE:
        {
            int socket_to_chat_node;
            struct addrinfo hints, *server_info;
            char port_string[6];

            pthread_mutex_lock( &mutex_chat_node_list );

            ChatNodeListElement *current = chat_nodes->first;

            //send message to all clients
            while (current->next)
            {
                //check for sender node
                if ( chat_node_equal( &(current->chat_node), &(message.chat_node)))
                {
                    current = current->next;
                    continue; 
                }
                memset(&hints, 0, sizeof(hints));
                hints.ai_socktype = SOCK_STREAM;
                hints.ai_family = AF_INET;
                sprintf(port_string, "%u", current->chat_node.port);

                inet_ntop(AF_INET, &(current->chat_node.ip), ip_string, INET_ADDRSTRLEN);

                getaddrinfo(ip_string, port_string, &hints, &server_info);

                socket_to_chat_node = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

                if( connect(socket_to_chat_node, server_info->ai_addr, server_info->ai_addrlen) )
                {
                    perror("Error connecting to server!");
                }

                //send message through socket 
                send_message( socket_to_chat_node, &message );
                
                close(socket_to_chat_node);
                current = current->next;
            }
            pthread_mutex_unlock( &mutex_chat_node_list );
            break;
        }

        case SHUTDOWN_ALL:
        {
            Message *message_ptr = new_message( SHUTDOWN, NULL, NULL );
            int socket_to_chat_node;
            struct addrinfo hints, *server_info;
            char port_string[6];

            pthread_mutex_lock( &mutex_chat_node_list );

            ChatNodeListElement *current = chat_nodes->first;

            //send shutdown message to clients
            while (current->next)
            {
                memset(&hints, 0, sizeof(hints));
                hints.ai_socktype = SOCK_STREAM;
                hints.ai_family = AF_INET;
                sprintf(port_string, "%u", current->chat_node.port);

                inet_ntop(AF_INET, &(current->chat_node.ip), ip_string, INET_ADDRSTRLEN);

                getaddrinfo(ip_string, port_string, &hints, &server_info);

                socket_to_chat_node = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

                if( connect(socket_to_chat_node, server_info->ai_addr, server_info->ai_addrlen) )
                {
                    perror("Error connecting to server!");
                }

                //send message through socket 
                send_message( socket_to_chat_node, message_ptr );
                
                close(socket_to_chat_node);
                current = current->next;
            }
            
            pthread_mutex_unlock( &mutex_chat_node_list );
            exit(EXIT_SUCCESS);
            break;
        }

        // default, if none of these commands, send error message
        default:
            printf("%d\n", message.type);
            perror("Not a valid message type\n");
            break;
    }
    pthread_exit(NULL);
    printf("%ld\n", received_message);
}
*/
