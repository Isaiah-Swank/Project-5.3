#ifndef MAIN_H
#define MAIN_H


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

#define STANDARD_SIZE 128
#define SERVER_ADDR "127.0.0.1" // ip address
#define PORT 23657              // server port

#define FALSE false
#define TRUE !false

// number of connectsions
#define NUM_CONNECTIONS 1

#endif
