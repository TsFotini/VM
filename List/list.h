#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//domh tou pinaka katakermatismou
typedef struct PHT_Element{
       char  numberPage[6];
	   int position; //8esi tou sthn ikoniki mnimi
	   char pid[4]; // apo pia ergsia proerxepte 
}PHT_Element;


typedef PHT_Element phtElement; 
typedef struct TypeOfNode* typeOfNode;

void Create(typeOfNode *list);

int isEmpty(typeOfNode list);

void Next(typeOfNode* node);

phtElement getData(typeOfNode node);

void insertNext(typeOfNode node, phtElement element);

void insertHead(typeOfNode* node, phtElement element);

void Insert(typeOfNode* list, phtElement element, typeOfNode node);

void deleteNode(typeOfNode* list, typeOfNode node);

void Route(typeOfNode list);

int Search(typeOfNode list, char* name, phtElement* element, char *typeOfProcess);

int findAndDelete(typeOfNode* list, char* name, phtElement* element, char *typeOfProcess);
