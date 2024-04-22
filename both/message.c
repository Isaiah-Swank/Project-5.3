// message.c file

#include "message.h"

Message* new_message(int type, ChatNode* chat_node_ptr, char* note)
{
    Message* new_message = malloc(sizeof(Message));

    // set the new message to the indivdual JOIN message
    new_message->type = type;
    new_message->chat_node = *chat_node_ptr;
    strncpy(new_message->note, note, sizeof(new_message->note) -1);

    // return the new message
    return new_message;
}
ssize_t send_message(int socket, Message* message_ptr)
{
    ssize_t bytes_sent;
    
    // send the message to the IP in the socket
    bytes_sent = send(socket, message_ptr, sizeof(Message), 0);

    // check if there was an error sending the message
    if(bytes_sent == -1)
    {
        perror("Error sending message");
    }

    // return the number of bytes sent
    return bytes_sent;
}
ssize_t recieve_message(int socket, Message* message_ptr)
{
    int index = 0;
    ssize_t bytes_read;
    size_t total_bytes_read = 0;

    size_t buffer_size = sizeof(message_ptr->note) - 1;

    // iterate through the message
    while( index < buffer_size)
    {
        // read the current character
        bytes_read = read(socket, message_ptr->note + index, sizeof(message_ptr->note) - index - 1);

        // check if error reading current byte
        if(bytes_read <= 0)
        {
            // print error message
            perror("Error reading message");
            return bytes_read;
        }

        // increment total bytes read
        total_bytes_read += bytes_read;
        // increment the index
        index += bytes_read;

        // check if at the end of the message
        if(message_ptr->note[index - 1] == '\0')
        {
            // break off since the message is recieved
            break; 
        }
    }

    message_ptr->note[index] = '\0';

    // print the new message
    printf("%s\n", message_ptr->note);

    // return the index
    return total_bytes_read;
}
