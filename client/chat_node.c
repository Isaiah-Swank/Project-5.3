// chat_node.c file

#include "chat_node.h"

// create a chat node
ChatNode* chat_node_new(unsigned int ip, unsigned int port, char* name)
{
    // create a new chat node

    // populate the new chat node

    // return the new chat node
    return NULL; // stub return
}

// create a chat node list
ChatNodeList* chat_nodes_new(void)
{
    // create an empty ChatNodeList

    // return the chatNodeList
    return NULL; // stub return
}

// adding a chat node
void chat_nodes_add_node(ChatNodeList* chat_node_list, ChatNode* chat_node)
{
    // add new chat node to the ChatNodeList
}

// removing a chat node
int chat_nodes_remove_node(ChatNodeList* chat_node_list, ChatNode* chat_node)
{
    // remove the current chat node from the ChatNodeList
    return 0; // stub return
}

// compare two chat nodes
bool chat_node_equal(ChatNode* first, ChatNode* second)
{
    // check if the chat nodes are equal
    return false; // stub return
}