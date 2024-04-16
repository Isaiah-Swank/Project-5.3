// main.c file for the client side

#define DBG
#include "dbg.h"

#include "client-main.h"

int main(int argc, char* argv[])
{
    pthread_t sender_thread;
    pthread_t reciever_therad;
    Properties properties;

    // read the properties
    char* properties_file = "swank_properties"; // default file

    if(argc == 2)
    {
        // if there is already a chosen properties file
        properties_file = argv[1];
    }

    properties = property_read_properties(properties_file);

    // start the sender
    if(pthread_create(sender_thread, NULL, send_to_server, (void*)properties))
    {
        debug("[main] error creating sender thread");
        exit(EXIT_FAILURE);
    }

    // start the reciever
    if(pthread_create(reciever_thread, NULL, recieve_from_sender, (void*)properties))
    {
        debug("[main] error creating reciever thread");
        exit(EXIT_FAILURE);
    }

    // join the reciever thread, which will be disconnected on a SHUTDOWN. aka how to exit
    if(pthread_join(reciever_thread, NULL))
    {
        debug("[main] error joining reciever thread");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}