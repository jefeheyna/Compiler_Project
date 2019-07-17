#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "symbolTable.h"
typedef struct ASCGNode
{
	//Line number
	int index;// = -1;

	//Attribute: 0 = error_message  1 = scope_information  2 = source_code
	int attr;// = -1;

	//String information, can be scope or error
	char *info;// = "Default info in ASCGNode.c";

	//Previous node and Next node
	struct ASCGNode* prev;// = NULL;
  struct ASCGNode* next;// = NULL;
} ASCGNode;


typedef struct ASCGList
{
  //head of the list
  ASCGNode *listhead;// = NULL;
} ASCGList;

void add(ASCGList *list, ASCGNode *node);

ASCGNode* pop(ASCGList *list);

int isEmpty(ASCGList *list);

void printList(ASCGList *list);

ASCGNode* updateHead(ASCGList *list);

int size_of(ASCGList *list);
