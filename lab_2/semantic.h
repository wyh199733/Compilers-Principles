#ifndef SEMANTIC_H
#define SEMANTIC_H
#include "symbol.h"
void Program(struct sign *now);
void ExtDefList(struct sign *now);
void ExtDef(struct sign *now);
void ExtDecList(struct sign *now,Type type);

Type Specifier(struct sign *now);
Type StructSpecifier(struct sign *now);

struct FieldList_* VarDec(struct sign *now,Type type);
struct Functype_* FunDec(struct sign *now);
struct FieldList_* VarList(struct sign *now);
struct FieldList_* ParamDec(struct sign *now);

void CompSt(struct sign *now,Type type);
void StmtList(struct sign *now,Type type);
void Stmt(struct sign *now,Type type);

FieldList DefList(struct sign *now);
FieldList Def(struct sign *now);
FieldList DecList(struct sign *now,Type type);
FieldList Dec(struct sign *now,Type type);

Type Exp(struct sign *now);
int Args(struct sign *now,FieldList sym);
#endif
