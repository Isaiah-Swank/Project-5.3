#ifndef SENDER_HANDLER_H
#define SENDER_HANDLER_H


#include "properties.h" 
#include "message.h" 
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <unistd.h>

// Type of request
typedef enum {
    REQUEST_JOIN,
    REQUEST_LEAVE,
    REQUEST_SHUTDOWN,
    REQUEST_SHUTDOWN_ALL,
    REQUEST_NOTE
} RequestType;



// function prototypes
void *send_to_server(void *arg);
int setup_client_socket(const char *ip, int port);





#endif 


