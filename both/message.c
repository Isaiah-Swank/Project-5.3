// message.c file

#include "message.h"

Message* new_message(int type, ChatNode* chat_node_ptr, char* note)
{
    Message* new_message;

    // switch statement checking for Individual or Group Messages
    switch(type) {

        // Individual Client JOIN Message
        case JOIN:

            // set the new message to the indivdual JOIN message
            new_message->type = type;
            new_message->chat_node = *chat_node_ptr;
            strcpy(new_message->note, note);

            break;
            
        // Group message "*** is Joining the Chat"
        case JOINING:

            // set the new message to the group JOINING message
            new_message->type = type;
            new_message->chat_node = *chat_node_ptr;
            strcpy(new_message->note, note);

            break;
        // Individual Client LEAVE Message
        case LEAVE:

            // set the new message to the indivdual LEAVE message
            new_message->type = type;
            new_message->chat_node = *chat_node_ptr;
            strcpy(new_message->note, note);

            break;
        // Group message "*** left the Chat"
        case LEFT:
 
            // set the new message to the group LEFT message
            new_message->type = type;
            new_message->chat_node = *chat_node_ptr;
            strcpy(new_message->note, note);

            break;

        // Normal text 'note' being sent
        case NOTE:

            new_message->type = type;
            new_message->chat_node = *chat_node_ptr;
            strcpy(new_message->note, note);
            break;

        // shutdown message to individual client
        case SHUTDOWN:

            new_message->type = type;
            new_message->chat_node = *chat_node_ptr;
            strcpy(new_message->note, note);
            break;

        // shutdown message to ALL clients
        case SHUTDOWN_ALL:

            new_message->type = type;
            new_message->chat_node = *chat_node_ptr;
            strcpy(new_message->note, note);
            break;

        // default, not a valid command
        default:
            
            // send error message
            printf("ERROR IN CREATING NEW MESSAGE\n");
            exit(EXIT_FAILURE);
            break;
    }

    // return the new message
    return new_message;
}
size_t send_message(int socket, Message* message_ptr)
{
    // send the message to the IP in the socket
    send(socket, message_ptr, strlen(message_ptr), 0);

    // disconnect from the socket
    return NULL; // stub return
}
size_t recieve_message(int socket, Message* message_ptr)
{
    int index = 0;

    // iterate through the message
    while( message_ptr->note[index] != "\0" )
    {
        // recieve the message from the IP in the socket
        read(socket, message_ptr->note, strlen(message_ptr->note));

        // increment the index
        index += 1;
    }
        
    // print the recieved message
    printf("%s\n", message_ptr->note);

    // return NULL
    return NULL; // stub return
}
