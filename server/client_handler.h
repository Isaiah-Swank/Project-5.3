// client_handler.h

#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include "chat_node.h"
#include "message.h"

// Function prototypes
void talk_to_client(int command, ChatNode *client);
void handle_join(int client_socket, ChatNode *node);
void handle_leave(int client_socket);
void handle_note(int client_socket);
void handle_shutdown(int client_socket);
void handle_shutdown_all(int client_socket, ChatNodeList client_list);

#endif /* CLIENT_HANDLER_H */