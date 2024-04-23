// client_handler.h

#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

/************************************************************************
 * libraries
 ************************************************************************/
// should always be there ...
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// socket/bind/listen/accept
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <pthread.h>

// read/write/close
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>

#include "message.h"
#include "chat_node.h"

// Function prototypes
void *talk_to_client(void *socket);

//allows use of mutex from other c files
extern pthread_mutex_t mutex_client_socket;
extern pthread_mutex_t mutex_chat_node_list;
extern ChatNodeList* chat_nodes;

#endif /* CLIENT_HANDLER_H */