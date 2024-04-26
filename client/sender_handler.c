#include "sender_handler.h"
#include "message.h"
#include <stdio.h>
#include <string.h>

#include "sender_handler.h"
#include "message.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>  // For close function


// Function to set up the client socket
int setup_client_socket(const char *ip, int port) {
    int sock;
    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sock);
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(sock);
        return -1;
    }

    return sock;
}

void *send_to_server(void *arg) {
    Properties *props = (Properties *)arg;
    char input[256];
    char* ip = property_get_property(props, "SERVER_IP");
    char* client_ip = property_get_property(props, "CLIENT_IP");
    char* client_port = property_get_property(props, "CLIENT_PORT");
    char* client_name = property_get_property(props, "MY_NAME");
    char* port_str = property_get_property(props, "SERVER_PORT");
    int port = atoi(port_str); // Convert port string to an integer
    ChatNode* new_chat_node = chat_node_new(atoi(client_ip), atoi(client_port), client_name);
    ssize_t sent_message;
    Message* message;

    if (!ip || !port_str) {
        fprintf(stderr, "IP address or port not specified in properties.\n");
        return NULL;
    }

    int client_socket = setup_client_socket(ip, port);
    if (client_socket == -1) 
    {
        fprintf(stderr, "Failed to setup client socket.\n");
        return NULL;
    }
    
    printf("Ready to connect!\n");
    
    while (1) {
        fgets(input, sizeof(input), stdin);
        
        //while(1)
        //{

        	if (strncmp(input, "JOIN", 4) == 0) {
            		message = new_message(JOIN, new_chat_node, input);
            		printf("%d, %s, %s\n", message->type, message->chat_node.name, message->note);
            		sent_message = send_message(client_socket, message);
            		printf("%s: Has JOINED the chat.\n", client_name);
            		break;
            		
        	} else if (strncmp(input, "LEAVE", 5) == 0) {
            		message = new_message(LEAVE, new_chat_node, input);
            		sent_message = send_message(client_socket, message);
            		printf("%s: Has LEFT the chat.\n", client_name);
            		// break;
        	} else if (strncmp(input, "SHUTDOWN", 8) == 0) {
            		message = new_message(SHUTDOWN, new_chat_node, input);
            		sent_message = send_message(client_socket, message);
            		printf("%s: Has SHUTDOWN.\n", client_name);
            		break;
        	} else if (strncmp(input, "SHUTDOWN ALL", 13) == 0) {
            		message = new_message(SHUTDOWN_ALL, new_chat_node, input);
            		sent_message = send_message(client_socket, message);
            		printf("%s: Has SHUTDOWN ALL.\n", client_name);
            		break;
        	} else 
        	{
            		message = new_message(NOTE, new_chat_node, input);
            		sent_message = send_message(client_socket, message);
            		printf("%s: %s\n", client_name, input);
            		break;
        	}
      }  
    
    if(sent_message < 0 )
    {
    	// no message sent
    	printf("no message sent\n");
    }
    close(client_socket);
    return NULL;
}







void send_join_request(int socket, const char* message) {
    if (message == NULL) {
        fprintf(stderr, "send_join_request: message parameter is NULL\n");
        return;
    }

    // Prepare the message for sending; you might want to add additional formatting
    char buffer[1024];
    int len = snprintf(buffer, sizeof(buffer), "%s\n", message);  // Append newline if protocol requires it

    if (len <= 0 || len >= sizeof(buffer)) {
        fprintf(stderr, "send_join_request: message formatting error or buffer overflow\n");
        return;
    }

    // Send the message to the server
    ssize_t bytes_sent = send(socket, buffer, strlen(buffer), 0);
    if (bytes_sent < 0) {
        perror("send_join_request: failed to send message");
    } else {
        printf("send_join_request: sent '%s' to server\n", buffer);
    }
}







void send_leave_request(int socket) {
    const char* cmd = "LEAVE\n";  // LEAVE command, assuming newline is needed

    // Send the LEAVE command to the server
    ssize_t bytes_sent = send(socket, cmd, strlen(cmd), 0);
    if (bytes_sent < 0) {
        perror("send_leave_request: failed to send LEAVE command");
    } else {
        printf("send_leave_request: 'LEAVE' command sent to server\n");
    }
}




void send_shutdown_request(int socket) {
    const char* cmd = "SHUTDOWN\n";  // Example shutdown command

    ssize_t bytes_sent = send(socket, cmd, strlen(cmd), 0);
    if (bytes_sent < 0) {
        perror("send_shutdown_request: Failed to send SHUTDOWN command");
    } else {
        printf("send_shutdown_request: 'SHUTDOWN' command sent\n");
    }
}






void send_shutdown_all_request(int socket) {
    const char* cmd = "SHUTDOWN ALL\n";  // SHUTDOWN ALL command

    ssize_t bytes_sent = send(socket, cmd, strlen(cmd), 0);
    if (bytes_sent < 0) {
        perror("send_shutdown_all_request: Failed to send SHUTDOWN ALL command");
    } else {
        printf("send_shutdown_all_request: 'SHUTDOWN ALL' command sent\n");
    }
}






void send_note(int socket, const char* message) {
    if (message == NULL) {
        fprintf(stderr, "send_note: Provided message is NULL\n");
        return;
    }

    ssize_t bytes_sent = send(socket, message, strlen(message), 0);
    if (bytes_sent < 0) {
        perror("send_note: Failed to send note");
    } else {
    // name: note
        printf("send_note: Note sent successfully\n");
    }
}
