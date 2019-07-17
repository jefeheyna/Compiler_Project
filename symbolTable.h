/* Doubly Linked List implementation */

#ifndef symbolTable_h
#define symbolTable_h

struct Scope  {
	struct Scope* next;
	struct Scope* prev;
    int storeCount;
    struct Entry {
        char* name;
        char* type;
        int scope;
        char* annotation;
        int lineNum;
        int colNum;
    } data[50];    
};

struct Scope* head; 
struct Scope* tail;

/**
 * To long to explain what these do: 
 * displayTable, globalList, paraList, declarationBlock
 * addToSymbolTable, addDeclaration, addParameterId, addFunctionType, addId
 * declarationType, digitCount, pushScope, Print, popScope, initialize
 * paraListCounter, paraListReset, scopeCounter, tableIndex, globalIndex, dblockIndex
 * 
 * DO NOT use these.
 * 
 */
struct Entry displayTable[100];
struct Entry globalList[50];
struct Entry paraList[50];
struct Entry declarationBlock[50];

void addToSymbolTable(char* name, int scope,char* type,char* annotation, int lineNum, int colNum);
void addDeclaration(char* name, int scope,char* type,char* annotation, int lineNum, int colNum);
void addParameterId(char* type, int scope, char* id, int lineNum, int colNum);
void addFunctionType(char* id, char* _type, int lineNum, int colNum);
void addId(char* type, int scope, char* name, int lineNum, int colNum);
char* declarationType;
int digitCount(int num);


char* evaluate_exp_rule_2(int pre_unary_op, char *exp, char *expName, int line);
char* evaluate_exp_rule_3(char *exp, int post_unary_op, char *expName, int line);

char* evaluate_exp_rule_6(char *exp1, int binary_op, char *exp2, char *exp1Name, char *exp2Name, int line);

void addTypeError(char *expected, char *actual, int line);


//This will add a new scope at tail of Doubly linked list
void pushScope(struct Entry * data);
//This will print all the sdblock scopes recursively
void printSDblockScopes();
void popScope();
void initialize();
int paraListCounter;
int paraListReset;
int scopeCounter;
int tableIndex;
int globalIndex;
int dblockIndex;

void add_not_declared_Error(char * id, int line, int colNum);
void add_already_declared_Error(char * id, int line, int colNum);
void add_not_declared_ErrorType(char * id, int line, int colNum);

bool isSblockDeclared(char * id);
bool isGlobalDeclared(char * id);
bool isParaIdDeclared(char * id);

/**
 * printSymbolTable() prints all the ids that have been added 
 * to the symbol table at some point. 
 * NOTE: printSymbolTable() does not remove ids from the table when when popScope() is called.
 * Because printSymbolTable() only prints what was added 
 * @return void
 * 
 */
void printSymbolTable();


/*----------------------------------------*
 *										  *
 * Helper functions you can use are below *
 *										  *
 *----------------------------------------*/


/**
 * idExist() check that the specified id exist in the table. 
 * @param id -  id or name
 * @return bool - either true or false*
 * 
 */
bool idExist(char * id);

/**
 * concat() is a function used to concatenate char* (strings)
 * @param s1 -  fist string
 * @param s2 -  second string *
 * @return char*
 * 
 */
char* concat(const char *s1, const char *s2);

/**
 * getId() function gets all the data for the specified id.
 * use getId( )-> to access the data
 * @param id or name 
 * @return struct Entry - pointer
 * 
 */
struct Entry * getId(char * id);

/**
 * isSameType() check that id1 and id2 are of the same type. 
 * @param id1 id2 -  id or name
 * @return bool - either true or false*
 * 
 */
bool isSameType(char * id1, char * id2);


/***
 * NOTE: You should call idExist() before doing any operation or calling other functions 
 * 		 in order to avoid getting null values, or segmentation faults.
 *      
 *       The primary role of the symbol talbe is to store ids and their types
 *       So the only functions you should be using to interact with the symbol table are
 *       idExist(), getId(), and isSameType().
 * 
*/

#endif /* symbolTable_h */
