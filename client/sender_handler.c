#include "sender_handler.h"
#include "message.h"
#include <stdio.h>

void handle_input(char *input, struct Client *client) 
{
	// send properties file to server

        // Check if the input is a JOIN command
        //If input starts with "JOIN" Then
          //Extract IP and port from input if necessary
          //Call send_join_request(client, IP, port)
        
        //Check if the input is a LEAVE command
        //Else If input is "LEAVE" Then
          //Call send_leave_request(client)
        
        // Check if the input is a SHUTDOWN command
        //Else If input is "SHUTDOWN" Then
           //Call send_shutdown_request(client)
           //Set global running flag to false to stop the client application loop
        
        //Check if the input is a SHUTDOWN ALL command
        //Else If input is "SHUTDOWN ALL" Then
           //Call send_shutdown_all_request(client)
           //Set global running flag to false to stop the client application loop

        //If the input is none of the above, treat it as a chat message
        //Else
            //Call send_chat_message(client, input)
 return NULL;
}

void send_request(Client *client, RequestType type) {
    //switch (type) {
        //case REQUEST_JOIN:
            // Implement join request logic here
         //   break;
        //case REQUEST_LEAVE:
            // Implement leave request logic here
        //    break;
       // case REQUEST_SHUTDOWN:
            // Implement shutdown request logic here
       //     break;
     //   case REQUEST_SHUTDOWN_ALL:
            // Implement shutdown all request logic here
        //    break;
      //  case REQUEST_NOTE:
            // Implement sending a note message here
            // 'message' parameter is used for the note content
    //        break;
  //      default:
            // Handle unknown request type if necessary
  //          break;
    }
return NULL;
}
