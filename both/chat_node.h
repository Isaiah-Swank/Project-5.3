// chat_node.h file

#ifndef chat_node_h
#define chat_node_h

// header files to include
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// ChatNode Struct
typedef struct chat_node_struct
{
    unsigned int ip;            // client IP address
    unsigned short int port;    // Port the client is LISTENING on
    char name[16];              // client's logical name
} ChatNode;


// ChatNode list element struct ( for building a list)
typedef struct chat_node_list_element_struct
{
    ChatNode chat_node;
    struct chat_node_list_element_struct* next;
} ChatNodeListElement;


// ChatNode List
typedef struct chat_node_list_struct
{
    ChatNodeListElement* first;
    ChatNodeListElement* last;
} ChatNodeList;


// function prototypes

// create a chat node
ChatNode* chat_node_new(unsigned int ip, unsigned int port, char* name);

// create a chat node list
ChatNodeList* chat_nodes_new(void);

// adding a chat node
void chat_nodes_add_node(ChatNodeList* chat_node_list, ChatNode* chat_node);

// removing a chat node
int chat_nodes_remove_node(ChatNodeList* chat_node_list, ChatNode* chat_node);

// compare two chat nodes
bool chat_node_equal(ChatNode* first, ChatNode* second);

// manage the chat_node_list memory
void chat_node_free(ChatNodeList* chat_node_list);

#endif
