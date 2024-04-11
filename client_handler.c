// client_handler.c

#include "client_handler.h"

void talk_to_client(int command, ChatNode *client) {
    switch(command) 
    {
        case JOIN:
            // Code to handle JOIN command
                // handle_join function

            // Notify the joining client about successful joining
                // message function

            break;
        case JOINING:
            // This case is just for internal handling, not triggered directly by client
            // Notify other clients about the joining client
                // message function
            break;
        case LEAVE:
            // Code to handle LEAVE command
                // handle_leave function

            // Notify client they are leaving the chat
                // message function

            // set LEFT command to notify other clients

            break;
        case LEFT:
            // This case is just for internal handling, not triggered directly by client
            // Notify other clients about the leaving client
                // message function

            break;

        case NOTE:
            // code to handle sending a note
            break;

        // SHUTDOWN
        case SHUTDOWN:

            // code to handle SHUTDOWN

            // notify the other clients that this client has left the chat

            break;

        case SHUTDOWN_ALL:
            // notify all clients of the shutdown

            // code to handle SHUTDOWN_ALL

            break;

        // default, if none of these commands, send error message
        default:

            break;
    }
}

// function that handles the client joining the chat
void handle_join(int client_socket, ChatNode *node)
{
    // add client (ChatNode) to the list of the others (ChatNodes)

}

// function that handles the client leaving the chat (closing the client socket)
void handle_leave(int client_socket)
{
    // remove the client node from the list of participants
    
    // Clean up resources for the leaving client
     
}

// function that handles the client sending a NOTE
void handle_note(int client_socket)
{
    // send note to all the other clients
     
}

// function that handles the client shutting down (only personal)
void handle_shutdown(int client_socket)
{
    // remove the client node from the list of participants
    
    // Clean up resources for the leaving client
     
}

// function that handles the client shutting down the whole chat room
void handle_shutdown_all(int client_socket, ChatNodeList client_list)
{
    // remove the client node from the list of participants
    
    // Clean up resources for the leaving client
     
}