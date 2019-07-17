#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "ASCGenerator.h"
#include "symbolTable.h"
#include <stdbool.h>


ASCGList* mainList;

void asc_init()
{
	mainList = malloc(sizeof(ASCGList));
	list = getMainList();
}

ASCGList* getMainList()
{
	return mainList;
}

void asc_store_error_message(char *error_message, size_t line_number, ASCGList* list)
{
	prepare_node_and_add(error_message,line_number,0, list);
}
void asc_store_scope_information(char *scope_information, size_t line_number, ASCGList* list)
{
	prepare_node_and_add(scope_information,line_number,1,list);
}
void asc_store_source_code(char *source_code, size_t line_number, ASCGList* list)
{
	prepare_node_and_add(source_code,line_number,2,list);
}

void prepare_node_and_add(char *message, size_t line_number, int attr, ASCGList* list)
{
	int ln = (int)line_number;
	ASCGNode *node;
	node = (ASCGNode*)malloc(sizeof(ASCGNode));
	node->index = ln;
	node->attr = attr;

	node->info = malloc(strlen(message)+1);
	strcpy(node->info,message);
	node->prev = NULL;
	node->next = NULL;
	add(list,node);
}

void asc_print(char *source_code_path,char* output_path,int load_attr, ASCGList* list)
{
	//This loads all source code into the list
	load_source_code_to_list(source_code_path, list);

	//print to file with output_path
	print_list_to_file(output_path,load_attr, list);
}
void print_list_to_file(char *print_file_path,int prefCondition, ASCGList* list)
{
	//Attempt to open a file to write
	FILE *optFile = fopen( print_file_path, "w+");

	if(optFile == NULL)
	{
		printf("Error opening file path: %s \n File is NULL.\n", print_file_path);
		return;
	}

	ASCGNode *Node = updateHead(list);
	int lineCount = 1;
	while(Node!=NULL)
	{
		int ln = Node->index;
		int attr = Node->attr;
		char* info = Node->info;

		//Attribute: 0 = error_message  1 = scope_information  2 = source_code
		//prefCondition: -1 = all line		attr-> only store target attribute info
		if(prefCondition == -1 || prefCondition==attr)
		{
			if(attr == 0)
			{				
				int inputRetVal = fputs(info,optFile);//optFile prints message				
				if(prefCondition == 0){
					//print error messages to console.
					printf(" %s\n", info);
				}								
				if(inputRetVal == 0)
				{	
					printf("Error printing error node: %d,%d,%s \n", ln,attr,info);
					return;
				}
				inputRetVal = fputs("\n",optFile);//optFile prints line end
				if(inputRetVal == 0)
				{
					printf("Error printing \\n after error_message node\n");
					return;
				}
			}
			else if(attr == 1)
			{	
				int inputRetVal = fputs(info,optFile);//optFile
				if(inputRetVal == 0)
				{
					printf("Error printing scope node: %d,%d,%s \n", ln,attr,info);
					return;
				}
				inputRetVal = fputs(" ",optFile);//optFile
				if(inputRetVal == 0)
				{
					printf("Error printing \" \" after error_message node\n");
					return;
				}
			}
			else
			{	
				char * temp = "";
				if(lineCount < 10){
					char resultLine[5]; 
					sprintf(resultLine, "%d", lineCount);
					temp ="0";
					temp = concat(temp, resultLine);
					temp = concat(temp, (char*)":0 ");					
				}
				else{
					char resultLine[5]; 
					sprintf(resultLine, "%d", lineCount);		
					temp = concat(resultLine, (char*)":0 ");
				}			

				int inputRetVal = fputs(concat(temp, info),optFile);//optFile
				if(inputRetVal < 0)
				{
					printf("Error printing source node: %d,%d,%s \n", ln,attr,info);
					return;
				}
				lineCount++;
			}
		}
		
		Node = Node->next;
	}

	//printf("Closing file.\n");
	int closerst = fclose(optFile);
	//printf("Close file Success.\n\n");
	if(closerst!=0)
	{
		printf("Close file failed.\n\n");
	}
}

void load_source_code_to_list(char *source_code_path, ASCGList* list)
{
	char *data = read_all_char(source_code_path);
	//add all strings into list
	char delimiter = '\n';

	int lineCounter = 1;

	//line start index
	int lineS = 0;
	//iterating index
	int idx = 0;
	while(data[idx]!='\0')
	{
	//	printf("%d\n",'\0');
	//	printf("%d\n",data[idx]);
		if(data[idx] == delimiter)
		{
		//	printf("%s\n","in if" );
			if(lineS == idx)
			{
			//	printf("%s\n","in if if" );
				lineS = idx+1;
				//	printf("here-----------------------------");
				//printList(list);
				asc_store_source_code("\n",lineCounter, list);
					//printf("%s\n","after if1" );
			//		printf("%s\n","in if if 2" );
				//printf("\n");
			}
			else
			{
				char line[idx-lineS+1];
				memcpy( line, &data[lineS], idx-lineS+2 );
				line[idx-lineS+1] = '\0';
				lineS = idx+1;
				asc_store_source_code(&line[0],lineCounter, list);
			}
			lineCounter++;
		}
		idx++;
	}
}
char* read_all_char(char *source_code_path)
{
	FILE *source_file = fopen(source_code_path, "r"); // read mode

	if (source_file == NULL)
	{
	  printf("Failed to open source_code_path: %s.!\n", source_code_path);
	  return NULL;
	}

	char ch;
	//Find the length of the file
	fseek(source_file, 0, SEEK_END);
  long flength = ftell(source_file);

  int fseekcode = fseek(source_file, 0, SEEK_SET);
	if(fseekcode != 0)
	{
		printf("Fseek error, returning NULL.\n");
		return NULL;
	}
  char *fbuffer = 0;
	fbuffer = calloc(flength,sizeof(char));
  if (fbuffer)
  {
    fread (fbuffer, 1, flength, source_file);
  }
	//printf("Printing source code content:\n");
	//printf("%s",fbuffer);
	//printf("Printed source code content\n");

	fclose(source_file);
	return fbuffer;
}


void asc_print_errors(ASCGList* list)
{
	if(list!=NULL)
	{
		print_list_to_file("errors.txt",0, list);
	}
	else
	{
		printf("%s\n", "Printing errors from NULL list");
	}
}




////////////////////////////////
////      Below are the    /////
//////   List functions    /////
////////////////////////////////

//Add a Node to the List
//Sorting should be done automatically
//scope_information is always placed in front of error_message
//This doesn't consider the situation of multiple error_message within one line
void add(ASCGList* list, ASCGNode* Node)
{
	//empty Node, return
  if(Node == NULL)
  {
		return;
	}

	//if list is NULL return
	if(list == NULL)
	{
		return;
  }
	//gathering the information within Node
	//the index here stands for which line of source_code it belongs to
	//attr is the type of information
	//info is the string itself
  int index = Node->index;
  int attr = Node->attr;
  char *info = Node->info;

	ASCGNode *listNode = updateHead(list);

  //empty List, place Node on list and return
  if(listNode == NULL)
  {
    list->listhead = Node;
    return;
  }
  else
  {
    //iterate through the List, find the next available spot in the List and insert
    //error_message is always in front of scope_information
		//scope_information is always in front of source_code
  	while (listNode != NULL)
  	{
  		int listidx = listNode->index;
  		int listatr = listNode->attr;
      //If found information for the same line of source_code
      if(listidx==index)
      {
				//Compare attr under same index, smaller attr goes first
        if(listatr == attr)
        {
					if(listNode->next == NULL)
					{
						listNode->next = Node;
						Node->prev = listNode;
						return;
					}
					else
					{
						listNode->next->prev = Node;
						Node->next = listNode->next;
						Node->prev = listNode;
						listNode->next = Node;
						return;
					}
        }
				else if(listatr < attr)
				{
					if(listNode->next == NULL)
					{
						listNode->next = Node;
						Node->prev = listNode;
						return;
					}
				}
        else if(listatr > attr)
        {
					if(listNode->prev == NULL)
					{
						listNode->prev = Node;
						Node->next = listNode;
						return;
					}
					else
					{
						listNode->prev->next = Node;
						Node->prev = listNode->prev;
						Node->next = listNode;
						listNode->prev = Node;
						return;
					}

        }
        else
        {
          //In case there is an unsigned attr error
          printf("Error: Unexpected fail adding to ASCGList, temp Node attr is not valid.\n");
          printf("index: %d.\n",listidx);
          printf("attr: %d.\n",listatr);
          printf("info: %s.\n",listNode->info);
          return;
        }
      }
      //If not found, the node should check if it should be added here
			//Otherwise, do nothing and check with next node.
      else if(index>listidx)
      {
				//if next is null, add here
				if(listNode->next == NULL)
				{
					Node->prev = listNode;
	        listNode->next = Node;
					return;
				}
      }
			//if the line number is smaller than the line number that the listnode belong to
			//Add in front of the node.
			else if(index<listidx)
			{
				//Check if replacement of pointer is necessary
				if(listNode->prev == NULL)
				{
					listNode -> prev = Node;
					Node -> next = listNode;
					return;
				}
				else
				{
					listNode->prev->next = Node;
	        Node->next = listNode;
	        Node->prev = listNode->prev;
	        listNode->prev = Node;
					return;
				}
			}
			//go to next node for iteration
  		listNode = listNode->next;
  	}


  }

  //Just in case if there is an Unexpected error for the method
	//This should execute if no value is returned before this line.
  printf("Error: Unexpected fail adding to ASCGList.\n");
  printf("index: %d.\n",index);
  printf("attr: %d.\n",attr);
  printf("info: %s.\n",info);
}

//pop the first Node in the List and remove it from the List
ASCGNode* pop(ASCGList* list)
{
  if(isEmpty(list)==0)
  {
    ASCGNode *Node = updateHead(list);
    if(Node->next!=NULL)
		{
			Node->next->prev=NULL;
			list->listhead = Node->next;
		}
		Node->next = NULL;
		Node->prev = NULL;
    return Node;
  }
  else
  {
    printf("Error: Expected failing pop, list == NULL\n");
    return NULL;
  }
}

//return 0 if the List is empty
//return 1 if the List is not empty
int isEmpty(ASCGList *list)
{
	if(list == NULL)
	{
		return 1;
	}
  if(list->listhead==NULL)
  {
    return 0;
  }
  else
  {
    return 1;
  }
}


//Print all the information in the List
void printList(ASCGList *list)
{
	if(list == NULL)
	{
		printf("Printing empty list");
		return;
	}

	ASCGNode *Node = updateHead(list);
	//iterate through the list
	while (Node != NULL)
	{
    int index = Node -> index;
		char* info = Node -> info;
		int attr = Node -> attr;
		printf("%d, %d, %s  \n", index, attr, info);
		Node = Node->next;
	}
}


//Getting the head of the list
ASCGNode* updateHead(ASCGList *list)
{
	if(list==NULL)
	{
		//printf("%s\n","NULL list getting head");
		return NULL;
	}
	ASCGNode* head = list->listhead;
	if(head==NULL)
	{
		//printf("%s\n","NULL listhead getting head");
		return NULL;
	}
	while(head->prev!=NULL)
	{
		head = head->prev;
		list->listhead = head;
	}
	return head;
}
int size_of(ASCGList *list)
{
	if(list==NULL)
	{
		//printf("%s\n","NULL list getting head");
		return -1;
	}
	ASCGNode* head = updateHead(list);
	if(head==NULL)
	{
		//printf("%s\n","NULL listhead getting head");
		return -1;
	}
	int size = 0;
	while(head!=NULL)
	{
		head = head->next;
		size++;
	}
	return size;
}
