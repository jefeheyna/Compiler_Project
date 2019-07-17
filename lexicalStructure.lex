%{
	#include <stdio.h>
	#include <string.h>
	int col_num = 1;
	void getInfo();
%}

digit 		[0-9]+
integer 	("+"|"-")?{digit}+
real        ("+"|"-")?{digit}+"."{digit}+(("e"|"E")+{integer})?
letter		[A-Za-z]
identifier	[A-Za-z_][A-Za-z0-9_]*
comment 	\(\*([\t\r\n]|[^\*]|(\*[^\)]))*\*\)
character 	\'(\\)?({digit}|{letter}|\'|\")\'
string  	\"([^\"\n]|\\.)*\"

%option yylineno
%option noyywrap

%%
"\""			{ getInfo(); return D_QUOTE;}
"["				{ getInfo(); return L_BRACKET;}
"]"				{ getInfo(); return R_BRACKET;}
{comment}		{ getInfo(); }
"integer"		{ getInfo(); return T_INTEGER;}
{integer}		{ getInfo(); return C_INTEGER;}
"real"			{ getInfo(); return T_REAL;}
{real}			{ getInfo(); return C_REAL;}
"character"		{ getInfo(); return T_CHARACTER;}
{character}		{ getInfo(); return C_CHARACTER;}
"Boolean"		{ getInfo(); return T_BOOLEAN;}
"true"			{ getInfo(); return C_TRUE;}
"false"			{ getInfo(); return C_FALSE;}
"isNull"		{ getInfo(); return IS_NULL;}
"&"				{ getInfo(); return AND;}
"|"				{ getInfo(); return OR;}
"string"		{ getInfo(); return T_STRING;}
{string}		{ getInfo(); return C_STRING;}
"null"			{ getInfo(); return NULL_PTR;}
"reserve"		{ getInfo(); return RESERVE;}
"release"		{ getInfo(); return RELEASE;}
"for"			{ getInfo(); return FOR;}
"while"			{ getInfo(); return WHILE;}
"if"			{ getInfo(); return IF;}
"then"			{ getInfo(); return THEN;}
"else"			{ getInfo(); return ELSE;}
"switch"		{ getInfo(); return SWITCH;}
"case"			{ getInfo(); return CASE;}
"otherwise"		{ getInfo(); return OTHERWISE;}
"type"			{ getInfo(); return TYPE;}
"function"		{ getInfo(); return FUNCTION;}
"closure"		{ getInfo(); return CLOSURE;}
"i2r"			{ getInfo(); return INT2REAL;}
"r2i"			{ getInfo(); return REAL2INT;}
{identifier}	{ getInfo(); return ID;}
"("				{ getInfo(); return L_PARENTHESIS;}
")"				{ getInfo(); return R_PARENTHESIS;}
"{"				{ getInfo(); return L_BRACE;}
"}"				{ getInfo(); return R_BRACE;}
":="            { getInfo(); return ASSIGN;}
"="             { getInfo(); return EQUAL_TO;}    
"<"             { getInfo(); return LESS_THAN;}                               
"."             { getInfo(); return DOT;}   
","             { getInfo(); return COMMA;}  
"+"             { getInfo(); return ADD;}  
"-"             { getInfo(); return SUB_OR_NEG;}       
"*"             { getInfo(); return MUL;}
"%"				{ getInfo(); return REM;}       
"\\"            { getInfo(); return BACKSLASH;}   
"/"				{ getInfo(); return DIV;}
"'"				{ getInfo(); return S_QUOTE;}
";"				{ getInfo(); return SEMI_COLON;}
":"				{ getInfo(); return COLON;}
"->"			{ getInfo(); return ARROW;}
"!"				{ getInfo(); return NOT;}

[ \t] 			{col_num +=1; }
[\n]			{col_num = 1;}
[\r]			{col_num = 1;}

%%

void getInfo(){
	yylval.data.line = yylineno; 
	yylval.data.col = col_num; 
	yylval.data.text = strdup(yytext);
	col_num += yyleng;
}



