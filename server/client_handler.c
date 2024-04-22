// client_handler.c

#include "client_handler.h"

void *talk_to_client(void *arg) {

    int client_socket = *((int *) arg);
    Message message;

    pthread_mutex_unlock( &mutex_client_socket );

    //read message from client 
    recieve_message( client_socket, &message);
    
    message.chat_node.ip = ntohl(message.chat_node.ip);
    message.chat_node.port = ntohs(message.chat_node.port);

    //close connection
    close(client_socket);

    switch(message.type) 
    {
        case JOIN:
            int socket_to_chat_node;
            struct addrinfo hints, *server_info;
            char port_string[6];

            pthread_mutex_lock( &mutex_chat_node_list );

            message.type = 6;   //set type to JOINING

            ChatNodeListElement *current = chat_nodes->first;

            //send joining message to clients
            while (current->next)
            {
                memset(&hints, 0, sizeof(hints));
                hints.ai_socktype = SOCK_STREAM;
                hints.ai_family = AF_INET;
                sprintf(port_string, "%u", current->chat_node.port);

                getaddrinfo(ip_ntop(current->chat_node.ip), port_string, &hints, &server_info);

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
            int error;
            char port_string[6];

            pthread_mutex_lock( &mutex_chat_node_list );

            //remove chat node from list
            chat_nodes_remove_node(chat_nodes, &message.chat_node);
            
            message.type = 7;   //set type to LEFT

            ChatNodeListElement *current = chat_nodes->first;

            //send joining message to clients
            while (current->next)
            {
                memset(&hints, 0, sizeof(hints));
                hints.ai_socktype = SOCK_STREAM;
                hints.ai_family = AF_INET;
                sprintf(port_string, "%u", current->chat_node.port);

                getaddrinfo(ip_ntop(current->chat_node.ip), port_string, &hints, &server_info);

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

                getaddrinfo(ip_ntop(current->chat_node.ip), port_string, &hints, &server_info);

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

                getaddrinfo(ip_ntop(current->chat_node.ip), port_string, &hints, &server_info);

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
            exit(EXIT_SUCCESS);
            break;
        }

        // default, if none of these commands, send error message
        default:
            perror("Not a valid message type\n");
            break;
    }
    pthread_exit(NULL);
}
