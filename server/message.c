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




ssize_t send_message(int socket, Message* message_ptr) {
    ssize_t bytes_sent = 0;
    ssize_t total_bytes_sent = 0;

    message_ptr->chat_node.ip = htonl(message_ptr->chat_node.ip);

    // Send the message type
    bytes_sent = send(socket, &message_ptr->type, sizeof(unsigned char), 0);
    if (bytes_sent == -1) {
        perror("Error sending message type");
        return -1;  // Return immediately if there's an error
    }
    total_bytes_sent += bytes_sent;

    // Send the ChatNode structure
    bytes_sent = send(socket, &message_ptr->chat_node, sizeof(ChatNode), 0);
    if (bytes_sent == -1) {
        perror("Error sending ChatNode");
        return -1;  // Return immediately if there's an error
    }
    total_bytes_sent += bytes_sent;

    // Send the Note structure
    bytes_sent = send(socket, &message_ptr->note, sizeof(Note), 0);
    if (bytes_sent == -1) {
        perror("Error sending Note");
        return -1;  // Return immediately if there's an error
    }
    total_bytes_sent += bytes_sent;

    // Return the total number of bytes sent
    return total_bytes_sent;
}


ssize_t recieve_message(int socket, Message* message_ptr)
{
    ssize_t bytes_read;

    //read in type
    bytes_read = read(socket, &message_ptr->type, sizeof(unsigned char));
    if(bytes_read == -1)
    {
    	perror("Error reading message type (received_message)");
    	return -1;
    }

    //read in chat Node
    bytes_read = read(socket, &message_ptr->chat_node, sizeof(ChatNode));
    if(bytes_read == -1)
    {
    	perror("Error reading chat node (recieve_message)");
    	return -1;
    }

    //read in note
    bytes_read = read(socket, &message_ptr->note, sizeof(Note));
    if(bytes_read == -1)
    {
      perror("Error reading message");
      return -1;
    }

    // return the index
    return bytes_read;
}
