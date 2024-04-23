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

typedef enum {
    REQUEST_JOIN,
    REQUEST_LEAVE,
    REQUEST_SHUTDOWN,
    REQUEST_SHUTDOWN_ALL,
    REQUEST_NOTE
} RequestType;




void *send_to_server(void *arg);
int setup_client_socket(const char *ip, int port);
void send_join_request(int socket, const char* message);
void send_leave_request(int socket);
void send_shutdown_request(int socket);
void send_shutdown_all_request(int socket);
void send_note(int socket, const char* message);




#endif 


