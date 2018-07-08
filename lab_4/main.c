#include <stdio.h>
#include "syntax.tab.c"
extern FILE* yyin;
int main(int argc, char** argv)
{
	if (argc <= 1) return 1;
	FILE* f = fopen(argv[1], "r");
	if (!f){
	perror(argv[1]);
	return 1;
	}
	yyrestart(f);
	yyparse();
	if(argc<=2)	return 1;
	printCode(argv[2]);
	return 0;
}
