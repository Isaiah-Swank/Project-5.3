#include "receiver_handler.h"
#include "message.h"

void receive_messages() 
{
    // Server loop: continuously listen for messages
    // Switch statement to handle different message types

        //While client is connected:
            // Wait for a message from the server
            
            // Check if message received successfully
           // If message is NULL:
             //   Print error
            
            // read message to understand what type it is and content
            
            // Handle the message based on its type
            // Switch messageType:
            //    Case NOTE:
                    // Display the note message content to the user
                    
                    
              //  Case JOIN:
                    // A new user has joined
                  
                    
              //  Case LEAVE:
                    // A user has left the chat
                   
                    
              //  Case SHUTDOWN:
                    // The server is shutting down or the client is instructed to shutdown
                    //initiate client shutdown sequence
                    
             //   Default:
                    // Handle unknown message types or keep alive messages
 return NULL;        
}
