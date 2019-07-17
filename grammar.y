%{
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include "symbolTable.h"
#include <assert.h>
#include "ASCGenerator.h"
extern int yylex();
extern int yyparse();
void yyerror(char* p);
%}
/*--------------------------------------*
 *										*
 * Definitions							*
 *										*
 *--------------------------------------*/
// identifier
%token ID
// type names
%token T_INTEGER
%token T_REAL
%token T_BOOLEAN
%token T_CHARACTER 
%token T_STRING
// constants
%token C_INTEGER
%token C_REAL
%token C_CHARACTER
%token C_STRING
%token C_TRUE
%token C_FALSE
//syntax error
%token SYNTAX_ERR
// keywords
%token NULL_PTR
%token RESERVE
%token RELEASE
%token FOR
%token WHILE
%token IF
%token THEN
%token ELSE
%token SWITCH
%token CASE
%token OTHERWISE
%token TYPE
%token FUNCTION
%token CLOSURE
// punctuation - grouping
%token L_PARENTHESIS
%token R_PARENTHESIS
%token L_BRACKET
%token R_BRACKET
%token L_BRACE
%token R_BRACE
%token S_QUOTE
%token D_QUOTE
// punctuation - other
%token SEMI_COLON
%token COLON
%token COMMA
%token ARROW
%token BACKSLASH
// operators
%token ADD
%token SUB_OR_NEG
%token MUL
%token DIV
%token REM
%token DOT
%token LESS_THAN
%token EQUAL_TO
%token ASSIGN
%token INT2REAL
%token REAL2INT
%token IS_NULL
%token NOT
%token AND
%token OR

/*----------------- Define type of tokens -----------------*/
%type <s> ID
%type <s> T_INTEGER
%type <s> T_REAL
%type <s> T_BOOLEAN
%type <s> T_CHARACTER 
%type <s> T_STRING
%type <s> TYPE
%type <s> FUNCTION
%type <s> definition
%type<s> _type;
%type<s> id_list
%type<s> const
%type<s> expression
%type<i> pre_unary_op
%type<i> post_unary_op
%type<i> binary_op
%type<s> assignable

/*------ Define associativity of operators -----*/
%right ASSIGN
%left LESS_THAN EQUAL_TO
%left SUB_OR_NEG ADD AND OR
%left DIV MUL REM OTHER
%left PRE_UNARY POST_UNARY IS_NULL
%left PAREN

/*------ Union ---------------------------------*/
%union{	
	char* s;
	int i;
	struct {
		char* text;
		int col;
		int line;
		char* type;
		int opType;
	} data;
}

/* start symbol */
%start program

%%
/*--------------------------------------*
 *										*
 * Grammar Rules						*
 *										*
 *--------------------------------------*/

program:        def_list {scopeCounter += 1;} sblock 
			    ;

def_list:       definition def_list
				| 
                ;

definition:     TYPE ID COLON { 
					addToSymbolTable($2, 0, (char*)"type record", $1, $<data.line>2, $<data.col>2);
				} dblock						 
				| TYPE ID COLON const ARROW _type{
					if(isGlobalDeclared($2)){
						add_already_declared_Error($2, $<data.line>2, $<data.col>2);
					}
					addToSymbolTable($2, 0, $6, $1, $<data.line>2, $<data.col>2);				
				} 
                | TYPE ID COLON const ARROW _type COLON L_PARENTHESIS const R_PARENTHESIS {					
					if(isGlobalDeclared($2)){
						add_already_declared_Error($2, $<data.line>2, $<data.col>2);
					}
					addToSymbolTable($2, 0, $6, $1, $<data.line>2, $<data.col>2);					
				}
                | TYPE ID COLON pblock ARROW _type  {					
					if(isGlobalDeclared($2)){
						add_already_declared_Error($2, $<data.line>2, $<data.col>2);
					}
					addFunctionType($2, $6, $<data.line>2, $<data.col>2);			
				}
                | FUNCTION ID COLON ID {
					if(isGlobalDeclared($2)){
						add_already_declared_Error($2, $<data.line>2, $<data.col>2);
					}
					if(!isGlobalDeclared($4)){
						add_not_declared_ErrorType($4, $<data.line>4, $<data.col>4);
					}
					addToSymbolTable($2, 0, $4, $1, $<data.line>2, $<data.col>2); 
				} sblock  { 
										
				}
                ;

sblock:         L_BRACE {					
					scopeCounter += 1; pushScope(declarationBlock); 
				} statement_list  R_BRACE {					 
					popScope(); 
				}
                | L_BRACE dblock {

					scopeCounter += 1; pushScope(declarationBlock); 
				} statement_list R_BRACE {					
					popScope(); 
				} 
                ;

dblock:         L_BRACKET {scopeCounter += 1;} declar_list R_BRACKET 
                ;

declar_list:    declaration SEMI_COLON declar_list
                | declaration
                ;

declaration:    _type { declarationType = $1; } COLON id_list 
                ;

id_list:        ID COMMA id_list {					 
					if(isSblockDeclared($1)){
						add_already_declared_Error($1, $<data.line>1, $<data.col>1);
					}	
					addId(declarationType, scopeCounter, $1, $<data.line>1, $<data.col>1);
					 
				}
				| ID assignOp const COMMA {					 
					if(isSblockDeclared($1) == 1){
						
						add_already_declared_Error($1, $<data.line>1, $<data.col>1);
					}			
					addId(declarationType, scopeCounter, $1, $<data.line>1, $<data.col>1);
					
				} id_list
				| ID {					
					if(isSblockDeclared($1)){
						add_already_declared_Error($1, $<data.line>1, $<data.col>1);
					}			
					addId(declarationType, scopeCounter, $1, $<data.line>1, $<data.col>1);
					
				}				 	 		
				| ID assignOp const {					
					if(isSblockDeclared($1)){
						add_already_declared_Error($1, $<data.line>1, $<data.col>1);
					}			
					addId(declarationType, scopeCounter, $1, $<data.line>1, $<data.col>1);					 
				}   
                ;

statement_list: statement SEMI_COLON statement_list
                | statement SEMI_COLON
                ;

statement:      FOR L_PARENTHESIS statement SEMI_COLON expression {
					if(strcmp($5, (char*)"Boolean")!=0){
						addTypeError((char*)"Boolean", $5, $<data.line>1);
					}
					//printf("for: %s \n", $5);
				} SEMI_COLON statement R_PARENTHESIS sblock
                | WHILE L_PARENTHESIS expression {

					if(strcmp($3, (char*)"Boolean")!=0){
						addTypeError((char*)"Boolean", $3, $<data.line>1);
					}
				}R_PARENTHESIS sblock
                | IF L_PARENTHESIS expression {
					if(strcmp($3, (char*)"Boolean")!=0){
						addTypeError((char*)"Boolean", $3, $<data.line>1);
					} 
					//printf("here: %s \n", $3);
				} R_PARENTHESIS THEN sblock ELSE sblock
				| SWITCH L_PARENTHESIS expression {
					if(strcmp($3, (char*)"Boolean")!=0){
						addTypeError((char*)"Boolean", $3, $<data.line>1);
					}
				} R_PARENTHESIS L_BRACE CASE const {
					if(strcmp($<data.type>7, (char*)"integer")!=0){
						addTypeError((char*)"integer", $<data.type>7, $<data.line>1);
					}
				} COLON sblock R_PARENTHESIS OTHERWISE COLON sblock
                | sblock
                | assignable assignOp expression 
                | mem_op assignable 
                ;

assignable:     ID {										
					if(!idExist($1)){
						//If it doesn't exist
						add_not_declared_Error($1, $<data.line>1, $<data.col>1);
					}					
					if(idExist($1)){
						$<data.type>$ = getId($1)->type;
						$<data.text>$ = getId($1)->name;
						$<data.line>$ = getId($1)->lineNum;	
					}
									
				}
                | assignable ablock
                | assignable rec_op ID {					
					if(idExist($3)){
						$<data.type>$ = getId($3)->type;
						$<data.text>$ = getId($3)->name;
						$<data.line>$ = getId($3)->lineNum;	
					}					
				}
                ;


expression:		const {
					$<data.type>$ = $<data.type>1;
					$<data.text>$ = $<data.text>1;
					$<data.line>$ = $<data.line>1;
					//printf("test again: %s \n", $<data.text>1);
				}
				| pre_unary_op expression %prec PRE_UNARY {

					$$ = evaluate_exp_rule_2($<data.opType>1, $<data.type>2, $<data.text>2, $<data.line>1); 
				}
				| expression post_unary_op %prec POST_UNARY {

					$$ = evaluate_exp_rule_3($<data.type>1,$2, $<data.text>1, $<data.line>1);					
				}
				| assignable {
					$$ = $1; 
				}
				| L_PARENTHESIS expression R_PARENTHESIS %prec PAREN {
					$$ = $2; 
				}
                | expression binary_op expression %prec OTHER {

					//printf("test 2: %s \n", $<data.type>1);
					$$ = evaluate_exp_rule_6($<data.type>1,$<data.opType>2,$<data.type>3,$<data.text>1,$<data.text>3, $<data.line>2);					
				}
                ;

pblock:			L_PARENTHESIS { scopeCounter += 1;} para_list R_PARENTHESIS { scopeCounter -= 1;}
				;

para_list:      ne_para_list 
				| 
				;

ne_para_list:	para_declar COMMA ne_para_list 
				| para_declar 
				;

para_declar:	_type COLON ID {					
					if(isParaIdDeclared($3)){
						add_already_declared_Error($3, $<data.line>3, $<data.col>3);						
					}					
					addParameterId($1, scopeCounter, $3, $<data.line>3, $<data.col>3);						
				}  
				;

_type:          T_INTEGER {$$ = $1;}
                | T_REAL {$$ = $1;}
                | T_BOOLEAN {$$ = $1;}
                | T_CHARACTER {$$ = $1;}
                | T_STRING {$$ = $1;}
				| ID {					
					if(!idExist($1)){
						//If it doesn't exist
						add_not_declared_ErrorType($1, $<data.line>1, $<data.col>1);						
					}
					$$ = $1;					
				}
                ;

ablock:			L_PARENTHESIS arg_list R_PARENTHESIS
				;

arg_list:       ne_arg_list
				|
				;

ne_arg_list:	expression COMMA ne_arg_list
				| expression
				;

pre_unary_op:	NOT {	
					$<data.opType>$ = 4;
					$<data.line>$ = $<data.line>1;
				}
				| INT2REAL {	
					$<data.opType>$ = 5;
					$<data.line>$ = $<data.line>1;
				}
				| REAL2INT {	
					$<data.opType>$ = 6;
					$<data.line>$ = $<data.line>1;
				}
				;

post_unary_op:	IS_NULL { $$ = 10;}
				;

mem_op:			RESERVE
				| RELEASE
				;

assignOp:		ASSIGN
				;

rec_op:			DOT
				;

binary_op:		ADD {	
					$<data.opType>$ = 1;
					$<data.line>$ = $<data.line>1;
				}
				| SUB_OR_NEG {	
					$<data.opType>$ = 1;
					$<data.line>$ = $<data.line>1;
				}
				| MUL {	
					$<data.opType>$ = 1;
					$<data.line>$ = $<data.line>1;
				}
				| DIV {	
					$<data.opType>$ = 1;
					$<data.line>$ = $<data.line>1;
				}
				| REM {	
					$<data.opType>$ = 11;
					$<data.line>$ = $<data.line>1;
				}
				| AND {	
					$<data.opType>$ = 22;
					$<data.line>$ = $<data.line>1;
				}
				| OR {	
					$<data.opType>$ = 22;
					$<data.line>$ = $<data.line>1;
				}
				| LESS_THAN {	
					$<data.opType>$ = 2;
					$<data.line>$ = $<data.line>1;
				}
				| EQUAL_TO {	
					$<data.opType>$ = 2;
					$<data.line>$ = $<data.line>1;
				}
				;

const:			C_REAL {
					$<data.text>$ = $<data.text>1;
					$$ = (char*)"real";
					$<data.line>$ = $<data.line>1;
				}
				| C_INTEGER {
					$<data.text>$ = $<data.text>1;
					$<data.type>$ = (char*)"integer";
					$<data.line>$ = $<data.line>1;
				}
				| C_CHARACTER {
					$<data.text>$ = $<data.text>1;
					$$ = (char*)"character";
					$<data.line>$ = $<data.line>1;
				}
				| C_STRING {
					$<data.text>$ = $<data.text>1;
					$$ = (char*)"string";
					$<data.line>$ = $<data.line>1;
				}
				| C_TRUE {
					$<data.text>$ = $<data.text>1;
					$$ = (char*)"Boolean";
					$<data.line>$ = $<data.line>1;
				}
				| C_FALSE {
					$<data.text>$ = $<data.text>1;
					$$ = (char*)"Boolean";
					$<data.line>$ = $<data.line>1;
				}
				| NULL_PTR {
					$<data.text>$ = $<data.text>1;
					$$ = (char*)"null";
					$<data.line>$ = $<data.line>1;
				}
				;
%%

/*--------------------------------------*
 *										*
 * Functions (C code)					*
 *										*
 *--------------------------------------*/

#include "lex.yy.c"	

void yyerror(char* p){
	printf("---------------------------------------------- \n");
	printf("Line %d: Syntax error: %s \n",yylineno, yytext);
	printf("---------------------------------------------- \n");    
}

int main(int argc, char *argv[]){
	printf("\n");
  	asc_init();  
	initialize();
	yyparse();
	load_source_code_to_list((char*)"input.txt", list);
	
	//comp prog
	asc_print_errors(list);

	for(int i=0; i < argc; i++){
		if(strcmp(argv[i], "-asc")==0){
			//comp -asc prog
			print_list_to_file("prog.asc",-1, list);
		}
		else if(strcmp(argv[i], "-st")==0){
			//comp -st prog
			printSymbolTable();			
		}
		else if(strcmp(argv[i], "-ir")==0){
			//comp -ir prog
		}
	}	
	printf("\n");
}
