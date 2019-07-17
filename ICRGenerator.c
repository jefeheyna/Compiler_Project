
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ICRGenerator.h"



void icr_init()
{
	list = malloc(sizeof(ICRGList));
}
ICRGNode* init_node()
{
	ICRGNode* node = malloc(sizeof(ICRGNode));
	ICRGNode* prev = list->current;
	if(prev == NULL)
	{
		list->listhead = node;
		list->current = node;
		node->index = 0;
	}
	else
	{
		prev->next = node;
		node->prev = prev;
		node->index = prev->index + 1;
	}
	return node;
}
ICRGList* getICRGList()
{
	return list;
}

/*
void icr_store(int attr,int arg1,int arg2,int output,int operation,char* extra)
{
	switch(attr)
	{
		//Please change the arguments and variables for your need.
		case 0:
			icr_store_assignment(arg1,arg2,output,operation,extra);
			break;
		case 1:
			icr_store_control(arg1,arg2,output,operation,extra);
			break;
		case 2:
			icr_store_calculation(arg1,arg2,output,operation,extra);
			break;
		case 3:
			icr_store_function(arg1,arg2,output,operation,extra);
			break;
		default:
			printf("Error in icr_store attr! attr= %d\n",attr);
	}
}
*/
/*
Assignment will take in arg2 value and store it in arg1's memory.
There are two type of implementations and you are welcomed to do your own.
1:
Grammar.y requests the memory of arg1, requests arg2's value.
Grammar.y will do type checking.
Sends memory address to ICRGenerator.c for simple storing purpose.
ICRGenerator.c performs code generation
2:
Grammar.y will send the name of arg1 and arg2 to ICRGenerator.c
ICRGenerator.c then communicates with Symbol table for memory address of arg1 and value of arg2
ICRGenerator performs type checking
ICRGenerator produces output
*/
void icr_store_assignment(int arg1,int arg2,int output,int operation,char* extra)
{
	ICRGNode* node = init_node();
	node->attr = ATTR_ASGN;
	node->arg1Type = arg1;
	node->arg2Type = arg2;
	node->outputType = output;
	node->operation = operation;
	node->info = extra;
}

/*
Calculation will take values of arg1, perform calculation with value in arg2 based on value in operation
Store the result in memory of output.
Refer to Assignment function if needed
*/
void icr_store_calculation(int arg1,int arg2,int output,int operation,char* extra)
{
	ICRGNode* node = init_node();
	node->attr = ATTR_CALC;
	node->arg1Type = arg1;
	node->arg2Type = arg2;
	node->outputType = output;
	node->operation = operation;
	node->info = extra;
}
/*
Function is for function calls
this should handle parameters and their values.
Refer to assignment if needed
*/
void icr_store_argument(int arg1,int arg2,int output,int operation,char* extra)
{
	ICRGNode* node = init_node();
	node->attr = ATTR_ARGU;
	node->arg1Type = arg1;
	node->arg2Type = arg2;
	node->outputType = output;
	node->operation = operation;
	node->info = extra;
}

/*
Function is for function calls
this should handle parameters and their values.
Refer to assignment if needed
*/
void icr_store_function(int arg1,int arg2,int output,int operation,char* extra)
{
	ICRGNode* node = init_node();
	node->attr = ATTR_FUNC;
	node->arg1Type = arg1;
	node->arg2Type = arg2;
	node->outputType = output;
	node->operation = operation;
	node->info = extra;
}

/*
Control is for control flow
this should handle control flow's intermediate code
arg1 and arg2 are likely to be contain interger values of line index number for goto commands.
arg1 and arg2 can also be used for labeling.
Specific implementation is independent but should be consistant throughout all control flows statements.
Refer to assignment and calculation if needed
*/
void icr_store_control(int arg1,int arg2,int output,int operation,char* extra)
{
	ICRGNode* node = init_node();
	node->attr = ATTR_CONT;
	node->arg1Type = arg1;
	node->arg2Type = arg2;
	node->outputType = output;
	node->operation = operation;
	node->info = extra;
}

void icr_store_goto(int arg1,int arg2,int output,int operation,char* extra)
{
	ICRGNode* node = init_node();
	node->attr = ATTR_GOTO;
	node->arg1Type = arg1;
	node->arg2Type = arg2;
	node->outputType = output;
	node->operation = operation;
	node->info = extra;
}


/*
this should fill all empty goto within list according to index values of each node in bplist
*/
void backpatch()
{
	ICRGNode* node = list->head;
	while(head->next!=NULL)
	{
		if(node->attr == ATTR_GOTO)
		{
			ICRGNode* current = pop_bplist();
			node->arg1Type = current->arg1Type;
		}
	}
}
ICRGNode* pop_bplist()
{
	ICRGNode* head = bplist->head;
	if(head!=NULL)
	{
		bplist->head = head->next;
	}
	return head;
}


//Print the ic to a file named output_path
void icr_print_all(char* output_path)
{
	if(list == NULL)
	{
		printf("Printing empty list");
		return;
	}

	//Attempt to open a file to write
	FILE *optFile = NULL;
	if(output_path!=NULL)
	{
		optFile = fopen( output_path, "w+");
	}
	ICRGNode *Node = list->listhead;
	//iterate through the list
	while (Node != NULL)
	{
    int index = Node -> index;
		char* info = Node -> info;
		int attr = Node -> attr;
		int arg1Type = Node->arg1Type;
		int arg2Type = Node->arg2Type;
		int outputType = Node->outputType;
		int operation = Node->operation;

		char* line = malloc(sizeof(char)*100);
		sprintf(line,"%d, %d, %d, %d, %d, %d, %s  \n", index, attr, arg1Type, arg2Type, outputType,operation, info);
		if(optFile == NULL)
		{
				printf("%s",line);
		}
		else
		{
			int writeRetVal = fputs(line,optFile);
			if(writeRetVal == 0)
			{
				printf("Error printing in ICR write to file\n");
				return;
			}
		}
		Node = Node->next;
	}
	if(optFile!=NULL)
	{
		int closeRst = fclose(optFile);
		if(closeRst!=0)
		{
			printf("Close file failed.\n\n");
		}
	}

}
