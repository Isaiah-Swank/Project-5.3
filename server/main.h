#ifndef MAIN_H
#define MAIN_H

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

#include "chat_node.h"
#include "properties.h"
#include "client_handler.h"
/************************************************************************
 * preprocessor directives
 ************************************************************************/
#define STANDARD_SIZE 128
#define SERVER_ADDR "127.0.0.1" // loopback ip address
#define PORT 23657              // port the server will listen on

// I cannot let go of the old-fashioned way :) - for readability ...
#define FALSE false
#define TRUE !false

// number of pending connections in the connection queue
#define NUM_CONNECTIONS 1

#endif
