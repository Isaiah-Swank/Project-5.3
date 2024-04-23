// main.c file for the client side

#define DBG
#include "dbg.h"

#include "client-main.h"

int main(int argc, char* argv[])
{
    pthread_t sender_thread;
    pthread_t receiver_thread;
    Properties *properties;
   

    // read the properties
    char* properties_file = "swank.properties"; // default file

    if(argc == 2)
    {
        // if there is already a chosen properties file
        properties_file = argv[1];
    }

    properties = property_read_properties(properties_file);

     
    // start the sender
    if(pthread_create(&sender_thread, NULL, send_to_server, (void*)properties))
    {
        debug("[main] error creating sender thread");
        exit(EXIT_FAILURE);
    }
  
    // start the receiver
    if(pthread_create(&receiver_thread, NULL, receive_from_server, (void*)properties))
    {
        debug("[main] error creating receiver thread");
        exit(EXIT_FAILURE);
    }

     printf("the problem is not reciever_handler");
 
    // join the receiver thread, which will be disconnected on a SHUTDOWN. aka how to exit
    if(pthread_join(receiver_thread, NULL))
    {
        debug("[main] error joining receiver thread");
        exit(EXIT_FAILURE);
    }
  
    return EXIT_SUCCESS;
}
