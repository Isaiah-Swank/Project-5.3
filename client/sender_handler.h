#define SENDER_HANDLER_H
#ifndef SENDER_HANDLER_H


#include "main.h" // For Client struct
typedef enum {
    REQUEST_JOIN,
    REQUEST_LEAVE,
    REQUEST_SHUTDOWN,
    REQUEST_SHUTDOWN_ALL,
    REQUEST_NOTE
} RequestType;
// Handles the input from the user
void handle_input(char *input, Client *client);

void send_request(Client *client, RequestType type);

#endif // SENDER_HANDLER_H

