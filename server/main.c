#include "main.h"
#include DBG
#include "dbg.h"

/*
//globals
ChatNodeListElement *chatNodeHeadPtr = NULL;
ChatNode *headChatNode = NULL;
chatNodeHeadPtr->chat_node = headChatNode;
chatNodeHeadPtr->next = NULL;
*/
pthread_mutex_t mutex_client_socket;
pthread_mutex_t mutex_chat_node_list;

ChatNode* chat_nodes;
/************************************************************************
 * MAIN
 ************************************************************************/

int main(int argc, char** argv) {
    
    int server_socket;                 // descriptor of server socket
    struct sockaddr_in server_address; // for naming the server's listening socket
    int client_socket;
    int yes=1;
	int rcvRet;
	bool keepGoing = TRUE;
    char propertiesTxt[1024];     
	
	//initialize member list
	chat_nodes. = NULL;
	chat_nodes. = NULL;

    // sent when ,client disconnected
    signal(SIGPIPE, SIG_IGN);
    
    // create unnamed network socket for server to listen on
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }
    
    // name the socket (making sure the correct network byte ordering is observed)
    server_address.sin_family      = AF_INET;           // accept IP addresses
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); // accept clients on any interface
    server_address.sin_port        = htons(PORT);       // port to listen on
    
    // binding unnamed socket to a particular port
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) != 0) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }
    
    // listen for client connections (pending connections get put into a queue)
    if (listen(server_socket, NUM_CONNECTIONS) != 0) {
        perror("Error listening on socket");
        exit(EXIT_FAILURE);
    }
    
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(char));

    // server loop
    while (TRUE) 
	{
		//wait for client connection
		while (keepGoing) 
		{
			client_socket = -1;
			client_socket = accept(server_socket, NULL, NULL);
		    switch (client_socket)
			{			
				default:
					keepGoing = FALSE;
					break;
				case -1:
					break;
			}
		}
		keepGoing = true;
		while (keepGoing)
		{
			rcvRet = 0;
			
			//get string of properties file from client
			rcvRet = recv(client_socket, propertiesTxt, sizeof(propertiesTxt), 0);
			
                        //convert that string to chatNode
                        //makeChatNodeFromString()

			switch(rcvRet)
			{
				case -1:			
					printf("Error reading from network\n");
					printf("[handle_request] thread ID %p error writing to network", (void *)pthread_self());
					pthread_exit(NULL);
					
				case 0:
					break;
					
				default:  //receives message from client
					keepGoing=false;
					break;
			}
		}
				
		pthread_t thread;
		
		//handle client
		if (pthread_create(&thread, NULL, handle_client, (void *)&client_socket))
		{
			printf("Server with PID %d error creating thread", getpid());
			exit(EXIT_FAILURE);
		}

        //save thread memory before closing
		if (pthread_detach(thread))
		{
			printf("Server with PID %d error detaching thread", getpid());
			exit(EXIT_FAILURE);
		}
			
		// close connection
		if (close(client_socket) == -1) 
		{
			perror("Error closing socket");
			exit(EXIT_FAILURE);
		} 
	
		pthread_exit(NULL);
    }
}
