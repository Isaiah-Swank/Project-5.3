// chat_node.c file

#include "chat_node.h"

ChatNode* chat_node_new(unsigned int ip, unsigned int port, char* name)
{
    // create a new chat node
    ChatNode* newNode = (ChatNode*) malloc(sizeof(ChatNode));

    // populate the new chat node
    newNode->ip = ip;
    newNode->port = port;
    strcpy(newNode->name, name);

    // return the new chat node
    return newNode;
}

// create a chat node list
ChatNodeList* chat_nodes_new(void)
{
    // create an empty ChatNodeList
    ChatNodeList* nodesList = (ChatNodeList*) malloc(sizeof(ChatNodeList));

    // set first and last values
    nodesList->first = NULL;
    nodesList->last = NULL;

    // return the chatNodeList
    return nodesList; // stub return
}

// adding a chat node
void chat_nodes_add_node(ChatNodeList* chat_node_list, ChatNode* chat_node)
{
    // variables
    ChatNodeListElement* newElement = (ChatNodeListElement*) malloc(sizeof(ChatNodeListElement));
    
    // fill the new chatNode
    newElement->chat_node = *chat_node;
    newElement->next = NULL;

    // check if there is NOT already a node in the first AND last spot
    if(chat_node_list->first == NULL)
    {
        chat_node_list->first = newElement;
        chat_node_list->last = newElement;
    }

    // else, add the new node to the end of the list
    else
    {
        chat_node_list->last->next = newElement;

        // update the new last in the list
        chat_node_list->last = newElement;
    }
    
}

// removing a chat node
int chat_nodes_remove_node(ChatNodeList* chat_node_list, ChatNode* chat_node)
{

    // variables
    ChatNodeListElement* currentNode = chat_node_list->first;
    ChatNodeListElement* prevNode = NULL;


    // check if the node to remove is at the front
    if(chat_node_equal(&currentNode->chat_node, chat_node))
    {
        // set the currentNode to the second in the list
        chat_node_list->first = currentNode->next;

        // check if the list is NULL
        if(chat_node_list->first == NULL)
        {
            // set the last to NULL
            chat_node_list->last = NULL;
        }

        // free the current node
        free(currentNode);

        // return 1 for success
        return 1;

    }

    // if not, iterate through the linked list and look for it
    else
    {
        while(currentNode != NULL && !chat_node_equal(&currentNode->next->chat_node, chat_node))
        {
            prevNode = currentNode;
            currentNode = currentNode->next;
        }
        
        // check if the node is not in the list
        if(currentNode == NULL)
        {
            // return 0 for failure
            return 0;
        }
        
        // update the list
        prevNode->next = currentNode->next;
        if( currentNode == chat_node_list->last)
        {
            chat_node_list->last = prevNode;
        }

        // free/remove the chat node
        free(currentNode);

        // return 1 for success
        return 1;
    }
    
}

// compare two chat nodes
bool chat_node_equal(ChatNode* first, ChatNode* second)
{
    // check if the chat nodes are equal
    return (first->ip == second->ip);
}

void chat_nodes_free(ChatNodeList* chat_node_list)
{
    // start at the beginning of the list
    ChatNodeListElement* current = chat_node_list->first;

    // loop through the list
    while(current != NULL)
    {
        // move to the next element in the list
        ChatNodeListElement* next = current->next;

        // free the current element
        free(current);

        // set the current to next
        current = next;
    }

    // free the list
    free(chat_node_list);
}
