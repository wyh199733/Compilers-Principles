%option yylineno
%{
	#include <stdio.h>
	#include <string.h>
	#include "syntax.tab.h"
	int flexerror=0;
	struct sign{
		int line;
		int column;
		int flag;
		int token;
		char type[32];
		char text[32];
		struct sign* root;
		struct sign* leaf;
		struct sign* brother;
};
	struct sign* create_sign(char type_[],int line_);
%}
delim		[ \t\n]
ws		{delim}+
letter		[A-Za-z]
digit		[0-9]
type		int|float
id		({letter}|_)({letter}|{digit}|_)*
int		[0]|[1-9][0-9]*
int8		[0][1-7][0-7]*
int16		[0][xX][1-9A-Fa-f][0-9A-Fa-f]*
float		(([0-9]*\.[0-9]+)|([0-9]+\.[0-9]*))
snfloat	{float}[Ee][+-]?{int}
relop		>|<|>=|<=|==|!=
%%
{ws}		{/* */}
struct		{
		yylval.sign=create_sign("STRUCT",yylineno);	
		return(STRUCT);}
return		{
		yylval.sign=create_sign("RETURN",yylineno);
		return(RETURN);}
if		{
		yylval.sign=create_sign("IF",yylineno);
		return(IF);}
else		{
		yylval.sign=create_sign("ELSE",yylineno);
		return(ELSE);}
while		{
		yylval.sign=create_sign("WHILE",yylineno);
		return(WHILE);}
{type}		{
		yylval.sign=create_sign("TYPE",yylineno);
		yylval.sign->flag=1;
		strcpy(yylval.sign->text, yytext);
		return TYPE;}
{int}		{
		yylval.sign=create_sign("INT",yylineno);
		yylval.sign->flag=1;
		strcpy(yylval.sign->text, yytext);
		return INT;}
{int8}		{
		yylval.sign=create_sign("INT",yylineno);
		yylval.sign->flag=1;
		strcpy(yylval.sign->text, yytext);
		return INT;}
{int16}	{
		yylval.sign=create_sign("INT",yylineno);
		yylval.sign->flag=1;
		strcpy(yylval.sign->text, yytext);
		return INT;}
{float}	{
		yylval.sign=create_sign("FLOAT",yylineno);
		yylval.sign->flag=1;
		strcpy(yylval.sign->text, yytext);
		return FLOAT;}
{snfloat}	{
		yylval.sign=create_sign("FLOAT",yylineno);
		yylval.sign->flag=1;
		strcpy(yylval.sign->text, yytext);
		return FLOAT;}
{id}		{
		yylval.sign=create_sign("ID",yylineno);
		yylval.sign->flag=1;
		strcpy(yylval.sign->text, yytext);
		return ID;}
{relop}	{
		yylval.sign=create_sign("RELOP",yylineno);
		yylval.sign->flag=1;
		strcpy(yylval.sign->text, yytext);
		return RELOP;}
";"		{
		yylval.sign=create_sign("SEMI",yylineno);
		return SEMI;}
","		{
		yylval.sign=create_sign("COMMA",yylineno);
		return COMMA;}
"="		{
		yylval.sign=create_sign("ASSIGNOP",yylineno);
		return ASSIGNOP;}
"+"		{
		yylval.sign=create_sign("PLUS",yylineno);
		return PLUS;}
"-"		{
		yylval.sign=create_sign("MINUS",yylineno);
		return MINUS;}
"*"		{
		yylval.sign=create_sign("PLUS",yylineno);
		return STAR;}
"/"		{
		yylval.sign=create_sign("DIV",yylineno);
		return DIV;}
"&&"		{
		yylval.sign=create_sign("AND",yylineno);
		return AND;}
"||"		{
		yylval.sign=create_sign("OR",yylineno);
		return OR;}
"."		{
		yylval.sign=create_sign("DOT",yylineno);
		return DOT;}
"!"		{
		yylval.sign=create_sign("NOT",yylineno);
		return NOT;}
"("		{
		yylval.sign=create_sign("LP",yylineno);
		return LP;}
")"		{
		yylval.sign=create_sign("RP",yylineno);
		return RP;}
"["		{
		yylval.sign=create_sign("LB",yylineno);
		return LB;}
"]"		{
		yylval.sign=create_sign("RB",yylineno);
		return RB;}
"{"		{
		yylval.sign=create_sign("LC",yylineno);
		return LC;}
"}"		{
		yylval.sign=create_sign("RC",yylineno);
		return RC;}
.		{printf("Error type A at line %d: Mysterious character \'%s\'\n", yylineno, yytext);flexerror=1;}
%%
struct sign* create_sign(char type_[],int line_)
{
	struct sign* sign_=(struct sign*)malloc(sizeof(struct sign));
	sign_->line=line_;
	sign_->flag=0;
	sign_->token=1;
	strcpy(sign_->type,type_);
	sign_->root=NULL;
	sign_->leaf=NULL;
	sign_->brother=NULL;
	return sign_;
}
