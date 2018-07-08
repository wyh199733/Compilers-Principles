#ifndef _OBJECTCODE_H
#define _OBJECTCODE_H

#include "intercode.h"
#include "semantic.h"

typedef struct Var_t{
	/*int value;*/
	int reg_no;
	Operand op;
	struct Var_t *next;
}VarDescipter;

typedef struct RegDescipter{
	/*int value;*/
	char name[6];
	int old;
	struct Var_t *var;
}RegDescipter;

typedef struct StkDescipter{
	int length;
	int from;
	int old[1024];
	VarDescipter *varstack[1024];
}StkDescipter;
void printObjectCode(InterCode codehead,FILE *fp);
void insertObjectCode(InterCode ir,FILE *fp);
void storeAllVar(FILE *fp);
void loadAllVar(FILE *fp);
int getReg(Operand op, FILE *fp);
#endif
