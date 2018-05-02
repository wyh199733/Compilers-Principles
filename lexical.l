%option yylineno

%{
	#include <stdio.h>
	#include <string.h>
	#include "syntax.tab.h"
%}

/*regular definitions */
delim		[ \t\n]
ws		{delim}+
letter		[A-Za-z]
digit		[0-9]
type		int|float
id		({letter}|_)({letter}|{digit}|_)*
int		[0]|[1-9][0-9]*
int8		[0][1-7][0-7]*
int16		[0][xX][1-9A-Fa-f][0-9A-Fa-f]*
float		[-+]?(([0-9]*\.[0-9]+)|([0-9]+\.[0-9]*))
relop		>|<|>=|<=|==|!=
%%
{ws}		{/* */}
struct		{printf("%s:STRUCT line %d\n",yytext,yylineno);return(STRUCT);}
return		{printf("%s:RETURN line %d\n",yytext,yylineno);return(RETURN);}
if		{printf("%s:IF line %d\n",yytext,yylineno);return(IF);}
else		{printf("%s:ELSE line %d\n",yytext,yylineno);return(ELSE);}
while		{printf("%s:WHILE line %d\n",yytext,yylineno);return(WHILE);}
{type}		{printf("%s:TYPE line %d\n",yytext,yylineno);return TYPE;}
{int}		{printf("%s:INT line %d\n",yytext,yylineno);return INT;}
{int8}		{printf("%s:INT line %d\n",yytext,yylineno);return INT;}
{int16}	{printf("%s:INT line %d\n",yytext,yylineno);return INT;}
{float}	{printf("%s:FLOAT line %d\n",yytext,yylineno);return FLOAT;}
{id}		{printf("%s:ID line %d\n",yytext,yylineno);return ID;}
{relop}	{printf("%s:RELOP line %d\n",yytext,yylineno);return RELOP;}
";"		{printf("%s:SEMI line %d\n",yytext,yylineno);return SEMI;}
","		{printf("%s:COMMA line %d\n",yytext,yylineno);return COMMA;}
"="		{printf("%s:ASSIGNOP line %d\n",yytext,yylineno);return ASSIGNOP;}
"+"		{printf("%s:PLUS line %d\n",yytext,yylineno);return PLUS;}
"-"		{printf("%s:MINUS line %d\n",yytext,yylineno);return MINUS;}
"*"		{printf("%s:STAR line %d\n",yytext,yylineno);return STAR;}
"/"		{printf("%s:DIV line %d\n",yytext,yylineno);return DIV;}
"&&"		{printf("%s:AND line %d\n",yytext,yylineno);return AND;}
"||"		{printf("%s:OR line %d\n",yytext,yylineno);return OR;}
"."		{printf("%s:DOT line %d\n",yytext,yylineno);return DOT;}
"!"		{printf("%s:NOT line %d\n",yytext,yylineno);return NOT;}
"("		{printf("%s:LP line %d\n",yytext,yylineno);return LP;}
")"		{printf("%s:RP line %d\n",yytext,yylineno);return RP;}
"["		{printf("%s:LB line %d\n",yytext,yylineno);return LB;}
"]"		{printf("%s:RB line %d\n",yytext,yylineno);return RB;}
"{"		{printf("%s:LC line %d\n",yytext,yylineno);return LC;}
"}"		{printf("%s:RC line %d\n",yytext,yylineno);return RC;}
.		{printf("Error type A at line %d: Mysterious character \'%s\'\n", yylineno, yytext);}
%%
