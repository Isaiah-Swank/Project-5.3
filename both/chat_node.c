// chat_node.c file

#include "chat_node.h"

// create a chat node
ChatNode* chat_node_new(unsigned int ip, unsigned int port, char* name)
{
    // create a new chat node
    ChatNode* newNode;

    // populate the new chat node
    newNode->ip = ip;
    newNode->port = port;
    strcpy(newNode->name, name);

    // return the new chat node
    return newNode;           // stub return
}

// create a chat node list
ChatNodeList* chat_nodes_new(void)
{
    // create an empty ChatNodeList
    ChatNodeList* nodesList;

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
    ChatNodeListElement* currentNode = chat_node_list->first;
    
    // add new chat node to the ChatNodeList
    // check if there is NOT already a node in the first spot
    if(chat_node_list->first == NULL)
    {
        chat_node_list->first->chat_node = *chat_node;
    }
    // else, loop through linked list
    else
    {
        while(currentNode->next != NULL)
        {
            currentNode = currentNode->next;
        }
        // add the new node to the end of the list
        currentNode->next->chat_node = *chat_node;

        // set the last node to the new node
        chat_node_list->last->chat_node = *chat_node;

    }
    // free the temp variable
    free(currentNode);
    
}

// removing a chat node
int chat_nodes_remove_node(ChatNodeList* chat_node_list, ChatNode* chat_node)
{

    // variables
    ChatNodeListElement* currentNode = chat_node_list->first;
    ChatNodeListElement* tempNode;


    // check if the node to remove is at the front
    if(chat_node_equal(&currentNode->chat_node, chat_node))
    {
        // set the currentNode to the second in the list
        currentNode = currentNode->next;

        // free/remove the ndoe
        free(chat_node_list->first);

        // set the new start of the list to the currentNode
        chat_node_list->first = currentNode;

        // free the temp node
        free(currentNode);

        // return 1 for success
        return 1;

    }

    // if not, iterate through the linked list and look for it
    else
    {
        while(!chat_node_equal(&currentNode->next->chat_node, chat_node) || currentNode == NULL)
        {
            currentNode = currentNode->next;
        }
        
        // check if the node is not in the list
        if(currentNode == NULL)
        {
            // return 0 for failure
            return 0;
        }
        // else (found the node to be removed), check if its the last one
        else if(currentNode->next->next == NULL)
        {
            // remove the node
            free(currentNode->next);

            // set the last in the list to 
            chat_node_list->last = currentNode;

            // free the working node
            free(currentNode);

            // return 1 for success
            return 1;
        }
        else
        {
            // set the temp node to the one after the node to be removed
            tempNode = currentNode->next->next;

            // free/remove the wanted node
            free(currentNode->next);

            // set the next in line of the currentNode to the tempNode
            currentNode->next = tempNode;

            // free the temp node
            free(tempNode);

            // return 1 for success
            return 1;
        }
    }
    
}

// compare two chat nodes
bool chat_node_equal(ChatNode* first, ChatNode* second)
{
    // check if the chat nodes are equal
    return (first->ip == second->ip);
}


int main()
{
	return 0;

}
