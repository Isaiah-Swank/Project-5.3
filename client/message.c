// message.c file

#include "message.h"

Message* new_message(int type, ChatNode* chat_node_ptr, char* note)
{
    // switch statement checking for Individual or Group Messages
    switch(type) {

        // Individual Client JOIN Message
        case JOIN:

            break;
            
        // Group message "*** is Joining the Chat"
        case JOINING:

            break;
        // Individual Client LEAVE Message
        case LEAVE:

            break;
        // Group message "*** left the Chat"
        case LEFT:
 
            break;

        // Normal text 'note' being sent
        case NOTE:

            break;

        // shutdown message to individual client
        case SHUTDOWN:

            break;

        // shutdown message to ALL clients
        case SHUTDOWN_ALL:

            break;

        // default, not a valid command
        default:
            
            // send error message
            break;
    }
}
size_t send_message(int socket, Message* message_ptr)
{
    // enter the socket given in the parameter

    // send the message to the IP in the socket

    // disconnect from the socket
    return NULL; // stub return
}
size_t recieve_message(int socket, Message* message_ptr)
{
    // enter the socket given in the parameter

    // recieve the message from the IP in the socket

    // disconnect from the socket
    return NULL; // stub return
}