#include "list.h"

typedef struct TypeOfNode{
	phtElement data;
	typeOfNode next;
}TypeOfNode;

void Create(typeOfNode *list){
	*list= NULL;
}

int isEmpty(typeOfNode list){
	return (list == NULL);
}

void Next(typeOfNode* node){
	*node = (*node)->next;
}

phtElement getData(typeOfNode node){
	return (node->data);
}

void insertNext(typeOfNode node, phtElement element){
	typeOfNode temp;

	temp = (typeOfNode)malloc(sizeof(typeOfNode));
	if (temp == NULL)
	{
		printf("Something wrong with malloc in insertNext \n"); 
		exit(-3);
	}

	temp->data = element;
	temp->next = node->next;
	node->next = temp;
}

void insertHead(typeOfNode* node, phtElement element){
	typeOfNode temp;

	temp = (typeOfNode)malloc(sizeof(typeOfNode));
	if (temp == NULL)
	{
		printf("Something wrong with malloc in insertHead \n");
		exit(-3);
	}

	temp->data = element;
	temp->next= *node;
	*node = temp;
}

void Insert(typeOfNode* list, phtElement element, typeOfNode node){
	if (isEmpty(node)){ 
		insertHead(list, element); 
	}
	else { 
		insertNext(node, element); 
	}
}

void delete(typeOfNode *node){
	typeOfNode temp;

	temp = *node;
	*node = temp->next;
	free(temp);
}

void deleteNode(typeOfNode* list, typeOfNode node){
	typeOfNode temp;

	if (isEmpty(*list))
		printf("List is Empty\n");
	else
	{
		if (isEmpty(node)) 
		{
			delete(list);
		}
		else
		{
			temp = node->next;
			delete(&node);
			node->next = temp;
		}
	}
}

void Route(typeOfNode list){
	typeOfNode current;

	current = list;
	while (!isEmpty(current))
	{ 
		Next(&current); 
	}
}


int Search(typeOfNode list, char* name, phtElement* element, char *typeOfProcess){
	typeOfNode current;

	current = list;
	while (!isEmpty(current))
	{ 
		if (strcmp((current->data.numberPage), name) == 0 && strcmp(typeOfProcess, current->data.pid) == 0)
		{
            *element = current->data;
			return 1;
		}

		Next(&current); 
	}
	return 0;
}

int findAndDelete(typeOfNode* list, char* name, phtElement* element, char *typeOfProcess){
	typeOfNode preCurrent;
    typeOfNode current;

    preCurrent = NULL;
	current = (*list);
	while(!isEmpty(current))
	{ 
		if (strcmp((current->data.numberPage), name) == 0 && strcmp(typeOfProcess, current->data.pid) == 0)
		{
			*element = current->data;
            deleteNode(&(*list), preCurrent);
			return 1;
		}
        preCurrent = current;
		Next(&current); 
	}
	return 0;
}