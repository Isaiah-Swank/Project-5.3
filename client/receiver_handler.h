#ifndef RECEIVER_HANDLER_H
#define RECEIVER_HANDLER_H

#include "properties.h"  // Assuming properties are used within the receiver handler
#include "message.h"     // Assuming a common message structure is defined here

// Function prototype for the thread that handles receiving messages from the server
void *receive_from_server(void *arg);

#endif // RECEIVER_HANDLER_H
