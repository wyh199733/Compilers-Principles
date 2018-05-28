#ifndef SYMBOL_H
#define SYMBOL_H
#include "sign.h"
#define max 32768
typedef struct Type_* Type;
typedef struct FieldList_* FieldList;
typedef struct Functype_* Functype;
struct Type_
{
enum { BASIC, ARRAY, STRUCTURE } kind;
union
{
int basic;
struct { Type elem; int size; } array;
FieldList structure;
} u;
};

struct FieldList_
{
	char name[32];
	Type type;
	FieldList tail;
	FieldList hash;
};

struct Functype_
{
	char name[32];
	Type type;
	FieldList param; 
	Functype hash;
};

unsigned int hash_pjw(char* name);
void table();
int insert_table(FieldList now);
int insert_func(Functype now);
void insert_Param(Functype now);
struct FieldList_* find_symbol(char *name);
struct Functype_* find_func(char *name);
#endif
