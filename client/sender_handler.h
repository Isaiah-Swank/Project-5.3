#ifndef SENDER_HANDLER_H
#define SENDER_HANDLER_H


#include "properties.h" 
#include "message.h"    
#include "main.h" 
typedef enum {
    REQUEST_JOIN,
    REQUEST_LEAVE,
    REQUEST_SHUTDOWN,
    REQUEST_SHUTDOWN_ALL,
    REQUEST_NOTE
} RequestType;




void *send_to_server(void *arg);

#endif 


