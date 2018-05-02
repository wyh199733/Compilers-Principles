%{
	#include<stdio.h>
	#include<stdlib.h>
	#include<string.h>
	#include<stdarg.h>
	#include "lex.yy.c"
%}
/*declared types*/
%union{struct type *type;}
/*declared tokens*/
%token <type>INT FLOAT TYPE ID SEMI COMMA
%right <type>ASSIGNOP NOT
%left  <type>PLUS MINUS STAR DIV RELOP AND OR 
%left  <type>DOT LP RP LB RB LC RC
%nonassoc <type>STRUCT RETURN IF ELSE WHILE
/*declared non-terminals*/
%type <type>Program ExtDefList ExtDef ExtDecList
%type <type>Specifier StructSpecifier OptTag Tag
%type <type>VarDec FunDec VarList ParamDec
%type <type>CompSt StmtList Stmt
%type <type>DefList Def DecList Dec
%type <type>Exp Args
%%
Program : ExtDefList;
ExtDefList : ExtDef ExtDefList
|  /*empty*/;
ExtDef : Specifier ExtDecList SEMI
| Specifier SEMI
| Specifier FunDec CompSt;	
ExtDecList : VarDec
| VarDec COMMA ExtDecList;

Specifier : TYPE
| StructSpecifier;
StructSpecifier : STRUCT OptTag LC DefList RC
| STRUCT Tag;
OptTag : ID
|  /*empty*/;
Tag : ID;

VarDec : ID
| VarDec LB INT RB;
FunDec : ID LP VarList RP
| ID LP RP;			
VarList : ParamDec COMMA VarList
| ParamDec;
ParamDec : Specifier VarDec			 ;
CompSt : LC DefList StmtList RC;			
StmtList : Stmt StmtList
|  /*empty*/;
Stmt : Exp SEMI
| CompSt
| RETURN Exp SEMI
| IF LP Exp RP Stmt
| IF LP Exp RP Stmt ELSE Stmt
| WHILE LP Exp RP Stmt;			
DefList : Def DefList
| /*empty*/;
Def : Specifier DecList SEMI;			
DecList : Dec
| Dec COMMA DecList;
Dec : VarDec
| VarDec ASSIGNOP Exp;

Exp : Exp ASSIGNOP Exp
| Exp AND Exp
| Exp OR Exp
| Exp RELOP Exp
| Exp PLUS Exp
| Exp MINUS Exp
| Exp STAR Exp
| Exp DIV Exp
| LP Exp RP
| MINUS Exp
| NOT Exp
| ID LP Args RP
| ID LP RP
| Exp LB Exp RB
| Exp DOT ID
| ID
| INT
| FLOAT;
Args : Exp COMMA Args
| Exp;
%%
yyerror(char* msg){
	fprintf(stderr,"Error type B at line %d:%s\n",yylineno,msg);
}
