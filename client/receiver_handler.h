#ifndef RECEIVER_HANDLER_H
#define RECEIVER_HANDLER_H

#include "properties.h" 
#include "message.h" 
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


void *receive_from_server(void *arg);
int setup_client_socket(const char *ip, int port);
#endif // RECEIVER_HANDLER_H
