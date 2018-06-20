%{ 
	#include<stdio.h>
	#include<stdlib.h>
	#include<string.h>
	#include<stdarg.h>
	#include "lex.yy.c"
	#include "semantic.h"
	#include "intercode.h"
	struct sign* root;
	extern flexerror;
	extern semanticerror;
	int bisonerror=0;
%}
/*declared types*/
%union		{struct sign* sign;}
/*declared tokens*/
%token 	<sign>INT FLOAT TYPE ID SEMI COMMA
%right 	<sign>ASSIGNOP NOT
%left  	<sign>PLUS MINUS STAR DIV RELOP AND OR 
%left  	<sign>DOT LP RP LB RB LC RC
%nonassoc	<sign>STRUCT RETURN IF ELSE WHILE
/*declared non-terminals*/
%type 		<sign>Program ExtDefList ExtDef ExtDecList
%type 		<sign>Specifier StructSpecifier OptTag Tag
%type 		<sign>VarDec FunDec VarList ParamDec
%type 		<sign>CompSt StmtList Stmt
%type 		<sign>DefList Def DecList Dec
%type 		<sign>Exp Args
%%
Program : ExtDefList{
		$$=create_sign("Program",$1->line);
		links($$,$1);
		if(!flexerror&&!bisonerror)print($$);
		/*Program($$);
		if(semanticerror==0)*/
		translate_Program($$);};
ExtDefList : ExtDef ExtDefList{
		$$=create_sign("ExtDefList",$1->line);
		links($$,$1);
		brother($1,$2);}
|{
		$$=create_sign("ExtDefList",yylineno);
		$$->token=0;};
ExtDef : Specifier ExtDecList SEMI{
		$$=create_sign("ExtDef",$1->line);
		links($$,$1);
		brother($1,$2);
		brother($2,$3);}
| Specifier SEMI{
		$$=create_sign("ExtDef",$1->line);
		links($$,$1);
		brother($1,$2);}
| Specifier FunDec CompSt{
		$$=create_sign("ExtDef",$1->line);
		links($$,$1);
		brother($1,$2);
		brother($2,$3);};
ExtDecList : VarDec{
		$$=create_sign("ExtDecList",$1->line);
		links($$,$1);}
| VarDec COMMA ExtDecList{
		$$=create_sign("ExtDecList",$1->line);
		links($$,$1);
		brother($1,$2);
		brother($2,$3);};

Specifier : TYPE{
		$$=create_sign("Specifier",$1->line);
		links($$,$1);}
| StructSpecifier{
		$$=create_sign("Specifier",$1->line);
		links($$,$1);};
StructSpecifier : STRUCT OptTag LC DefList RC{
		$$=create_sign("StructSpecifier",$1->line);
		links($$,$1);
		brother($1,$2);
		brother($2,$3);
		brother($3,$4);
		brother($4,$5);}
| STRUCT Tag{
		$$=create_sign("StructSpecifier",$1->line);
		links($$,$1);
		brother($1,$2);};
OptTag : ID{
		$$=create_sign("OptTag",$1->line);
		links($$,$1);}
|  {
		$$=create_sign("OptTag",yylineno);
		$$->token=0;};
Tag : ID	{
		$$=create_sign("Tag",$1->line);
		links($$,$1);};

VarDec : ID{
		$$=create_sign("VarDec",$1->line);
		links($$,$1);}
| VarDec LB INT RB{
		$$=create_sign("VarDec",$1->line);
		links($$,$1);
		brother($1,$2);
		brother($2,$3);
		brother($3,$4);};
FunDec : ID LP VarList RP{
		$$=create_sign("FunDec",$1->line);
		links($$,$1);
		brother($1,$2);
		brother($2,$3);
		brother($3,$4);}
| ID LP RP{
		$$=create_sign("FunDec",$1->line);
		links($$,$1);
		brother($1,$2);
		brother($2,$3);};			
VarList : ParamDec COMMA VarList{
		$$=create_sign("VarList",$1->line);
		links($$,$1);
		brother($1,$2);
		brother($2,$3);}
| ParamDec{
		$$=create_sign("VarList",$1->line);
		links($$,$1);};
ParamDec : Specifier VarDec{
		$$=create_sign("ParamDec",$1->line);
		links($$,$1);
		brother($1,$2);};
CompSt : LC DefList StmtList RC{
		$$=create_sign("CompSt",$1->line);
		links($$,$1);
		brother($1,$2);
		brother($2,$3);
		brother($3,$4);};			
StmtList : Stmt StmtList{
		$$=create_sign("StmtList",$1->line);
		links($$,$1);
		brother($1,$2);}
| {
		$$=create_sign("StmtList",yylineno);
		$$->token=0;};
Stmt : Exp SEMI{
		$$=create_sign("Stmt",$1->line);
		links($$,$1);
		brother($1,$2);}
| CompSt{
		$$=create_sign("Stmt",$1->line);
		links($$,$1);}
| RETURN Exp SEMI{
		$$=create_sign("Stmt",$1->line);
		links($$,$1);
		brother($1,$2);
		brother($2,$3);}
| IF LP Exp RP Stmt{
		$$=create_sign("Stmt",$1->line);
		links($$,$1);
		brother($1,$2);
		brother($2,$3);
		brother($3,$4);
		brother($4,$5);}
| IF LP Exp RP Stmt ELSE Stmt{
		$$=create_sign("Stmt",$1->line);
		links($$,$1);
		brother($1,$2);
		brother($2,$3);
		brother($3,$4);
		brother($4,$5);
		brother($5,$6);
		brother($6,$7);}
| WHILE LP Exp RP Stmt{
		$$=create_sign("Stmt",$1->line);
		links($$,$1);
		brother($1,$2);
		brother($2,$3);
		brother($3,$4);
		brother($4,$5);};			
DefList : Def DefList{
		$$=create_sign("DefList",$1->line);
		links($$,$1);
		brother($1,$2);}
| {
		$$=create_sign("DefList",yylineno);
		$$->token=0;};
Def : Specifier DecList SEMI{
		$$=create_sign("Def",$1->line);
		links($$,$1);
		brother($1,$2);
		brother($2,$3);};			
DecList : Dec{
		$$=create_sign("DecList",$1->line);
		links($$,$1);}
| Dec COMMA DecList{
		$$=create_sign("DecList",$1->line);
		links($$,$1);
		brother($1,$2);
		brother($2,$3);};
Dec : VarDec{
		$$=create_sign("Dec",$1->line);
		links($$,$1);}
| VarDec ASSIGNOP Exp{
		$$=create_sign("Dec",$1->line);
		links($$,$1);
		brother($1,$2);
		brother($2,$3);};

Exp : Exp ASSIGNOP Exp{
		$$=create_sign("Exp",$1->line);
		links($$,$1);
		brother($1,$2);
		brother($2,$3);}
| Exp AND Exp{
		$$=create_sign("Exp",$1->line);
		links($$,$1);
		brother($1,$2);
		brother($2,$3);}
| Exp OR Exp{
		$$=create_sign("Exp",$1->line);
		links($$,$1);
		brother($1,$2);
		brother($2,$3);}
| Exp RELOP Exp{
		$$=create_sign("Exp",$1->line);
		links($$,$1);
		brother($1,$2);
		brother($2,$3);}
| Exp PLUS Exp{
		$$=create_sign("Exp",$1->line);
		links($$,$1);
		brother($1,$2);
		brother($2,$3);}
| Exp MINUS Exp{
		$$=create_sign("Exp",$1->line);
		links($$,$1);
		brother($1,$2);
		brother($2,$3);}
| Exp STAR Exp{
		$$=create_sign("Exp",$1->line);
		links($$,$1);
		brother($1,$2);
		brother($2,$3);}
| Exp DIV Exp{
		$$=create_sign("Exp",$1->line);
		links($$,$1);
		brother($1,$2);
		brother($2,$3);}
| LP Exp RP{
		$$=create_sign("Exp",$1->line);
		links($$,$1);
		brother($1,$2);
		brother($2,$3);}
| MINUS Exp{
		$$=create_sign("Exp",$1->line);
		links($$,$1);
		brother($1,$2);}
| NOT Exp{
		$$=create_sign("Exp",$1->line);
		links($$,$1);
		brother($1,$2);}
| ID LP Args RP{
		$$=create_sign("Exp",$1->line);
		links($$,$1);
		brother($1,$2);
		brother($2,$3);
		brother($3,$4);}
| ID LP RP{
		$$=create_sign("Exp",$1->line);
		links($$,$1);
		brother($1,$2);
		brother($2,$3);}
| Exp LB Exp RB{
		$$=create_sign("Exp",$1->line);
		links($$,$1);
		brother($1,$2);
		brother($2,$3);
		brother($3,$4);}
| Exp DOT ID{
		$$=create_sign("Exp",$1->line);
		links($$,$1);
		brother($1,$2);
		brother($2,$3);}
| ID{
		$$=create_sign("Exp",$1->line);
		links($$,$1);}
| INT{
		$$=create_sign("Exp",$1->line);
		links($$,$1);}
| FLOAT{
		$$=create_sign("Exp",$1->line);
		links($$,$1);};
Args : Exp COMMA Args{
		$$=create_sign("Args",$1->line);
		links($$,$1);
		brother($1,$2);
		brother($2,$3);}
| Exp{
		$$=create_sign("Args",$1->line);
		links($$,$1);};
%%
yyerror(char* msg){
	fprintf(stderr,"Error type B at line %d:%s\n",yylineno,msg);
	bisonerror=1;
}
