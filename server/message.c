// message.c file

#include "message.h"

Message* new_message(int type, ChatNode* chat_node_ptr, char* note)
{
    Message* new_message = malloc(sizeof(Message));

    // set the new message to the indivdual JOIN message
    new_message->type = type;
    new_message->chat_node = *chat_node_ptr;
    printf("%d\n", new_message->chat_node.ip);
    strncpy(new_message->note, note, sizeof(new_message->note) -1);

    // return the new message
    return new_message;
}
ssize_t send_message(int socket, Message* message_ptr)
{
    ssize_t bytes_sent;
    message_ptr->chat_node.ip = htonl(message_ptr->chat_node.ip);
    
    // send the message to the IP in the socket
    bytes_sent = send(socket, &message_ptr->type, sizeof(unsigned char), 0);

    // check if there was an error sending the message
    if(bytes_sent == -1)
    {
        perror("Error sending message");
    }
    
    bytes_sent = send(socket, &message_ptr->chat_node, sizeof(ChatNode), 0);

    // check if there was an error sending the message
    if(bytes_sent == -1)
    {
        perror("Error sending message");
    }
    
    
    bytes_sent = send(socket, &message_ptr->note, sizeof(Note), 0);

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
    // int index = 0;
    ssize_t bytes_read;
    // size_t total_bytes_read = 0;

    
    // size_t buffer_size = sizeof(message_ptr->note) - 1;
    
    bytes_read = read(socket, &message_ptr->type, sizeof(unsigned char));
    if(bytes_read == -1)
    {
    	perror("Error reading message type (received_message)");
    	return -1;
    }
    
    bytes_read = read(socket, &message_ptr->chat_node, sizeof(ChatNode));
    if(bytes_read == -1)
    {
    	perror("Error reading chat node (recieve_message)");
    	return -1;
    }
    
    // read the note
    bytes_read = read(socket, &message_ptr->note, sizeof(Note));

    // check if error reading current byte
    if(bytes_read == -1)
    {
      // print error message
      perror("Error reading message");
      return -1;
    }

    // return the index
    return bytes_read;
}
