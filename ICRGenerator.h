#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ATTR_ASGN 0
#define ATTR_CALC 1
#define ATTR_ARGU 2
#define ATTR_FUNC 3
#define ATTR_CONT 4
#define ATTR_GOTO 5

typedef struct ICRGNode
{
  //index of node in the list
  //In another word, position of the line in the intermediate code
	int index;

	//Attribute: 0:assignment, 1:control flow, 2:calculation, 3: function call
	//					4: goto, 5: control, etc
	int attr;// = -1;

  //type of argument, 0:real, 1:int, 2:boolean etc.
  int arg1Type;
  int arg2Type;
  int outputType;

  //Type of operation, 0: assign, 1: addition, 2: subtraction, 3: division
  //                   4: multiplication, 5: unary negation
  int operation;


	//String information
	char *info;

	//Previous node and Next node
	struct ICRGNode* prev;// = NULL;
  struct ICRGNode* next;// = NULL;
} ICRGNode;


typedef struct ICRGList
{
  //head of the list
  ICRGNode *listhead;// = NULL;
	ICRGNode *current;
} ICRGList;


typedef struct backpatch_List
{
	ICRGNode* head;
}backpatch_List;


ICRGList* list;

backpatch_List* bplist;

ICRGList* getICRGList();

void icr_store(int attr,int arg1,int arg2,int output,int operation,char* extra);

void icr_store_assignment(int arg1,int arg2,int output,int operation,char* extra);

void icr_store_calculation(int arg1,int arg2,int output,int operation,char* extra);

void icr_store_argument(int arg1,int arg2,int output,int operation,char* extra);

void icr_store_function(int arg1,int arg2,int output,int operation,char* extra);

void icr_store_control(int arg1,int arg2,int output,int operation,char* extra);

void icr_store_goto(int arg1,int arg2,int output,int operation,char* extra);

void backpatch();




void icr_print_all(char* output_path);
