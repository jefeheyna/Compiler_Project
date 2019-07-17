#include <stdio.h>
#include <string.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "symbolTable.h"
#include "ASCGenerator.h"
#include <assert.h>

void initialize(){
    tableIndex = 0;
    paraListCounter = 0;
	globalIndex = 0;
	dblockIndex = 0;
	paraListReset = 0;
	scopeCounter = 0;
	head = NULL; 
	tail = NULL;
}

int digitCount(int num){
	int value = 1;
    while(num != 0) {
        num /= 10;
        ++value;
    }
	if(value == 1){value +=1;}
	return value;
}

void addParameterId(char* type, int scope, char* id, int lineNum, int colNum){
    paraList[paraListCounter].name = id;
    paraList[paraListCounter].type = type;
    paraList[paraListCounter].scope = scope;
    paraList[paraListCounter].annotation = (char*)"parameter";
	paraList[paraListCounter].lineNum = lineNum;
	paraList[paraListCounter].colNum = colNum;
	paraListCounter +=1;
	paraListReset += 1;
}

void addFunctionType(char* id, char* _type, int lineNum, int colNum){	
	char * temp;
	temp = concat((char*)"(", (char*)"");	
	for(int i=0; i < paraListReset; i++){
		if(i!=0){
			temp = concat(temp, (char*)", ");
		}
		temp = concat(temp, paraList[i].type);        
	}   	
	temp = concat(temp, (char*)") -> ");
	temp = concat(temp, _type);	
	addToSymbolTable(id, 0, temp, (char*)"type", lineNum, colNum);
	for(int i=paraListCounter - paraListReset; i < paraListCounter; i++){		
		addToSymbolTable(paraList[i].name, 1, paraList[i].type, paraList[i].annotation, paraList[i].lineNum, paraList[i].colNum);								
	}
	paraListReset = 0;
}

void addDeclaration(char* name, int scope,char* type,char* annotation, int lineNum, int colNum){	
	for(int i = 0; i < 50; i++){
		if (declarationBlock[i].name == NULL) {
			declarationBlock[i].name = name;
			declarationBlock[i].scope = scope;						
			declarationBlock[i].type = type; 
			declarationBlock[i].annotation = annotation;
			declarationBlock[i].lineNum = lineNum;
			declarationBlock[i].colNum = colNum;
			dblockIndex += 1;
			return;
		}		
	}	
}

void addId(char* type, int scope, char* name, int lineNum, int colNum){
	addDeclaration(name, scope, type, (char*)"local", lineNum, colNum);											
	addToSymbolTable(name, scope, type, (char*)"local", lineNum, colNum);
}

void addToSymbolTable(char* name, int scope,char* type,char* annotation, int lineNum, int colNum){
	//add to displayTable struct array
	displayTable[tableIndex].name = name;
	displayTable[tableIndex].scope = scope;						
	displayTable[tableIndex].type = type; 
	displayTable[tableIndex].annotation = annotation;
	displayTable[tableIndex].lineNum = lineNum;
	displayTable[tableIndex].colNum = colNum;
	tableIndex += 1;
	//add to globalList which is the global scope
	globalList[globalIndex].name = name;
	globalList[globalIndex].scope = scope;						
	globalList[globalIndex].type = type; 
	globalList[globalIndex].annotation = annotation;
	globalList[globalIndex].lineNum = lineNum;
	globalList[globalIndex].colNum = colNum;
	globalIndex += 1;

}

void printSymbolTable(){
	int nameWidth = 6;
	int typeWidth = 6;
	for(int i = 0; i < 50; i++){
		if(displayTable[i].name != NULL){
			if(nameWidth < strlen(displayTable[i].name)){
				nameWidth = strlen(displayTable[i].name);
			}			
		}		
	}
	for(int i = 0; i < 50; i++){
		if(displayTable[i].name != NULL){
			if(typeWidth < strlen(displayTable[i].type)){
				typeWidth = strlen(displayTable[i].type);
			}			
		}		
	}
	printf("\n");
	printf(" NAME");
	for(int i=0; i < nameWidth - 4; i++){
		printf(" ");
	}
	printf(" :");
	printf(" SCOPE");
	for(int i=0; i < 1; i++){
		printf(" ");
	}
	printf(":");
	printf(" TYPE");
	for(int i=0; i < typeWidth - 4; i++){
		printf(" ");
	}
	printf(" :");
	printf(" Extra annotation");
	for(int i=0; i < nameWidth - 17; i++){
		printf(" ");
	}
	printf("\n");
	for(int i = 0; i < tableIndex; i++){
		if(displayTable[i].name != NULL){
			int length = nameWidth - strlen(displayTable[i].name);
			printf(" %s", displayTable[i].name);
			for(int i=0; i < length; i++){
				printf(" ");
			}
			printf(" :");
		}
		if(1){			
			int length = 6 - digitCount(displayTable[i].scope);
						
			printf(" %d", displayTable[i].scope);
			for(int i=0; i < length; i++){
				printf(" ");
			}
			printf(" :");
		}
		if(displayTable[i].type != NULL){
			int length = typeWidth - strlen(displayTable[i].type);			
			printf(" %s", displayTable[i].type);
			for(int i=0; i < length; i++){
				printf(" ");
			}
			printf(" :");
		}
		if(displayTable[i].annotation != NULL){				
			printf(" %s", displayTable[i].annotation);					
		}
		printf("\n");		
	}
	printf("\n");
}

void pushScope(struct Entry * thisdata) {
	//printSDblockScopes();
	if(dblockIndex == 0){		
		//printf(" this data is empty \n");
		return;
	}
	struct Scope* newNode = (struct Scope*)malloc(sizeof(struct Scope));
	newNode->storeCount = 0; 
	for(int i=0; i < dblockIndex; i++){
		if(thisdata[i].name != NULL){
			newNode->data[i] = thisdata[i];
			newNode->storeCount += 1;
		}		
	}		
	if (tail == NULL) {
		assert (tail == NULL);
		head = tail = newNode;
	} else {
		newNode->prev = tail;
		tail->next = newNode;
		tail = newNode;
	}
	memset(declarationBlock, 0, sizeof declarationBlock);
	return;
}

void popScope() {	
	int deleteCounter = 0;    
	struct Scope* temp = tail;
	if (head == NULL) {
		assert(tail == NULL);		
		//empty list		
		return;
	}
	if (head == tail) {
		head = tail = NULL;
	} else {
		tail = tail->prev;
		tail->next = NULL;
	}	
	deleteCounter = temp->storeCount;
	//free(temp);	
	dblockIndex -= deleteCounter;	
	return;
}

//Prints all the elements in linked list in forward traversal order
void printSDblockScopes() {
	struct Scope* temp = head;
	if (head == NULL) {
		assert(tail == NULL);
		printf("Empty list \n");		
		return;
	}
	printf("__________________________________ \n");
	while(temp != NULL) {
		for(int i=0; i < temp->storeCount; i++){		
			printf(" %s %s %s \n" , temp->data[i].name, temp->data[i].annotation, temp->data[i].type);
		}
		printf("  \n");
		temp = temp->next;
	}
	printf("__________________________________ \n");
	return;
}

char* concat(const char *s1, const char *s2){
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

bool isSblockDeclared(char * id){
	for(int i = 0; i < 50; i++){
		if(declarationBlock[i].name != NULL){
			if (strcmp(declarationBlock[i].name, id)==0) {
				return true;
			}
		}				
	}
	return false;
}

bool isParaIdDeclared(char * id){
	
	for(int i= paraListCounter - paraListReset; i < paraListCounter; i++){		
		if(paraList[i].name == NULL){
			//printf("paraList is null \n");
		}else{			
			if(strcmp(paraList[i].name, id)==0){			
				return true;
			}
		}	
	}
	return false;
}
bool isGlobalDeclared(char * id){
	for(int i=0; i < 50; i++){
		if(globalList[i].name == NULL){
			//do nothing
		}else{
			if(strcmp(globalList[i].name, id)==0){
				return true;
			}
		}	
	}
	return false;
}

bool idExist(char * id){
	bool value = false;	
	struct Scope* temp = tail;
	
	//check in all the local scopes
	while(temp != NULL) {	
		for(int i=0; i < temp->storeCount; i++){
			if(strcmp(temp->data[i].name, id)==0){
				value = true;				
			}
		}
		temp = temp->prev;
	}

	//check in global scopes
	for(int i=0; i < 50; i++){
		if(globalList[i].name == NULL){
			//do nothing
		}else{
			if(strcmp(globalList[i].name, id)==0){
				value = true;
			}
		}	
	}

	//check parameters (addFunctionType types) 
	for(int i=0; i < 50; i++){
		
		if(paraList[i].name == NULL){
			//printf("paraList is null \n");
		}else{			
			if(strcmp(paraList[i].name, id)==0){			
				value = true;
			}
		}	
	}	
	return value;
}

struct Entry * getId(char * id){

	if(idExist(id)){		
		struct Scope* temp = tail;		
		//check in all the local scopes
		while(temp != NULL) {			
			for(int i=0; i < temp->storeCount; i++){
				if(strcmp(temp->data[i].name, id)==0){				
					
					return &temp->data[i];			
				}
			}
			temp = temp->prev;
		}	
		//check in global scopes
		for(int i=0; i < 50; i++){
			if(globalList[i].name == NULL){
				//do nothing
			}else{
				if(strcmp(globalList[i].name, id)==0){					
					return &globalList[i];	
				}
			}	
		}
		//check parameters (addFunctionType types) 
		for(int i=0; i < 50; i++){
			
			if(paraList[i].name == NULL){
				//printf("paraList is null \n");
			}else{			
				if(strcmp(paraList[i].name, id)==0){			
					return &paraList[i];
				}
			}	
		}
	}
	return NULL;
}

bool isSameType(char * id1, char * id2){
	bool value =  false;
	if(idExist(id1) && idExist(id2)){
		if(strcmp(getId(id1)->type, getId(id2)->type)==0){
			return true;
		}
	}
	return false;
}


void add_not_declared_Error(char * id, int line, int colNum){
	char resultLine[5]; 
    sprintf(resultLine, "%d", line);
	char resultColumn[5];  
    sprintf(resultColumn, "%d", colNum);
	char* temp = "LINE ";
	temp = concat(temp, resultLine);
	temp = concat(temp, (char*)":");
	temp = concat(temp, resultColumn);
	temp = concat(temp, (char*)" ** ERROR: the name '");
	temp = concat(temp, id);
	temp = concat(temp, (char*)"', used here as a variable name, has not been declared at this point in the program.");
	asc_store_error_message(temp,line+1,list);
}

void add_not_declared_ErrorType(char * id, int line, int colNum){
	char resultLine[5]; 
    sprintf(resultLine, "%d", line);
	char resultColumn[5];  
    sprintf(resultColumn, "%d", colNum);
	char* temp = "LINE ";
	temp = concat(temp, resultLine);
	temp = concat(temp, (char*)":");
	temp = concat(temp, resultColumn);
	temp = concat(temp, (char*)" ** ERROR: the name '");
	temp = concat(temp, id);
	temp = concat(temp, (char*)"', used here as a type, has not been declared at this point in the program.");
	asc_store_error_message(temp,line+1,list);
}

void add_already_declared_Error(char * id, int line, int colNum){
	char resultLine[5]; 
    sprintf(resultLine, "%d", line);
	char resultColumn[5];  
    sprintf(resultColumn, "%d", colNum);
	char* temp = "LINE ";
	temp = concat(temp, resultLine);
	temp = concat(temp, (char*)":");
	temp = concat(temp, resultColumn);
	temp = concat(temp, (char*)" ** ERROR: the name '");
	temp = concat(temp, id);
	temp = concat(temp, (char*)"', used here has already been declared in this scope of the program.");
	asc_store_error_message(temp,line+1,list);
}




char* evaluate_exp_rule_2(int pre_unary_op, char *exp, char *expName, int line){
	
	if(pre_unary_op == 4){
		if(strcmp(exp, (char*)"Boolean")==0 ){
			return "Boolean";
		}
		else{
			char resultLine[5]; 
    		sprintf(resultLine, "%d", line);
			char* temp = "LINE ";
			temp = concat(temp,resultLine);
			temp = concat(temp, (char*)": ** ERROR: ");
			temp = concat(temp, expName);
			temp = concat(temp, (char*)" must be of type Boolean in order to perform this action");
			asc_store_error_message(temp,line+1,list);
			
		}
	}

	if(pre_unary_op == 5){
		if(strcmp(exp, (char*)"integer")==0 ){
			return "integer";
		}
		else{
			char resultLine[5]; 
    		sprintf(resultLine, "%d", line);
			char* temp = "LINE ";
			temp = concat(temp,resultLine);
			temp = concat(temp, (char*)": ** ERROR: ");
			temp = concat(temp, expName);
			temp = concat(temp, (char*)" must be of type integer in order to perform this action");
			asc_store_error_message(temp,line+1,list);
			
		}
	}

	if(pre_unary_op == 6){
		if(strcmp(exp, (char*)"real")==0 ){
			return "real";
		}
		else{
			char resultLine[5]; 
    		sprintf(resultLine, "%d", line);
			char* temp = "LINE ";
			temp = concat(temp,resultLine);
			temp = concat(temp, (char*)": ** ERROR: ");
			temp = concat(temp, expName);
			temp = concat(temp, (char*)" must be of type real in order to perform this action");
			asc_store_error_message(temp,line+1,list);
			
		}
	}
	return "different";
}

char* evaluate_exp_rule_3(char *exp, int post_unary_op, char *expName, int line){

	return "different";
}

char* evaluate_exp_rule_6(char *exp1, int binary_op, char *exp2, char *exp1Name, char *exp2Name, int line){

	if(binary_op == 1){
		if((strcmp(exp1, (char*)"integer")==0 || strcmp(exp1, (char*)"real")==0) 
		&& (strcmp(exp2, (char*)"integer")==0 || strcmp(exp2, (char*)"real")==0)){
			return "real";
		}
		else{
			char resultLine[5]; 
    		sprintf(resultLine, "%d", line);
			char* temp = "LINE ";
			temp = concat(temp,resultLine);
			temp = concat(temp, (char*)": ** ERROR: ");
			temp = concat(temp, exp1Name);
			temp = concat(temp, (char*)" and ");
			temp = concat(temp, exp2Name);
			temp = concat(temp, (char*)" must be of type integer or real in order to perform this action");
			asc_store_error_message(temp,line+1,list);
			
		}
	}

	if(binary_op == 11){
		if(strcmp(exp1, (char*)"integer")==0 && strcmp(exp2, (char*)"integer")==0){
			return "integer";
		}
		else{
			char resultLine[5]; 
    		sprintf(resultLine, "%d", line);
			char* temp = "LINE ";
			temp = concat(temp,resultLine);
			temp = concat(temp, (char*)": ** ERROR: ");
			temp = concat(temp, exp1Name);
			temp = concat(temp, (char*)" and ");
			temp = concat(temp, exp2Name);
			temp = concat(temp, (char*)" must be of type integer in order to perform this action");
			asc_store_error_message(temp,line+1,list);
			
		}
	}

	if(binary_op == 22){
		if(strcmp(exp1, (char*)"Boolean")==0 && strcmp(exp2, (char*)"Boolean")==0){
			return "Boolean";
		}
		else{
			char resultLine[5]; 
    		sprintf(resultLine, "%d", line);
			char* temp = "LINE ";
			temp = concat(temp,resultLine);
			temp = concat(temp, (char*)": ** ERROR: ");
			temp = concat(temp, exp1Name);
			temp = concat(temp, (char*)" and ");
			temp = concat(temp, exp2Name);
			temp = concat(temp, (char*)" must be of type Boolean in order to perform this action");
			asc_store_error_message(temp,line+1,list);			
		}
	}

	if(binary_op == 2){
		if(strcmp(exp1, exp2)==0 ){			
			return "Boolean";
		}
		else if((strcmp(exp1, (char*)"integer")==0 || strcmp(exp1, (char*)"real")==0) 
		&& (strcmp(exp2, (char*)"integer")==0 || strcmp(exp2, (char*)"real")==0)){
			return "Boolean";
		}
		else{
			char resultLine[5]; 
    		sprintf(resultLine, "%d", line);
			char* temp = "LINE ";
			temp = concat(temp,resultLine);
			temp = concat(temp, (char*)": ** ERROR: ");
			temp = concat(temp, exp1Name);
			temp = concat(temp, (char*)" and ");
			temp = concat(temp, exp2Name);
			temp = concat(temp, (char*)" must be of type Boolean in order to perform this action");
			asc_store_error_message(temp,line+1,list);			
		}
	}

	return "different";
}

void addTypeError(char *expected, char *actual, int line){
	char resultLine[5]; 
	sprintf(resultLine, "%d", line);
	char* temp = "LINE ";
	temp = concat(temp,resultLine);
	temp = concat(temp, (char*)": ** ERROR: This statement is expecting a ");
	temp = concat(temp, expected);
	temp = concat(temp,(char*)" but the actual type is ");
	temp = concat(temp, actual);
	asc_store_error_message(temp,line+1,list);
}

