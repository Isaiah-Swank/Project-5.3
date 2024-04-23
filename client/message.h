// message.h file

#ifndef MESSAGE_H
#define MESSAGE_H

#include "chat_node.h"
#include "stddef.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>


// COMMAND CONSTANTS
#define JOIN         1
#define LEAVE        2
#define NOTE         3
#define SHUTDOWN     4
#define SHUTDOWN_ALL 5
// Commands for displaying to other participants
#define JOINING      6
#define LEFT         7

// Fixed length of a NOTE command
typedef char Note[64];


// Message sent over the network
typedef struct message_struct
{
    unsigned char type;
    ChatNode chat_node;
    Note note;
} Message;

// Function Prototypes
Message* new_message(int type, ChatNode* chat_node_ptr, char* note);
ssize_t  send_message(int socket, Message* message_ptr);
ssize_t  receive_message(int socket, Message* message_ptr);

#endif
