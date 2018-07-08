#ifndef _INTERCODE_H
#define _INTERCODE_H
#include "symbol.h"
typedef struct Operand_* Operand;
typedef struct InterCode_* InterCode;
struct Operand_ {
enum {TEMPVAR,VARIABLE,CONSTANT,ADDRESS,LABEL,FUNCTION} kind;
union {
int var_no;
int value1;
float value2;
char name[32];
}u;
Operand next;
};

struct InterCode_
{
enum {ASSIGN_,MIN_,ADD_,SUB_,MUL_,DIV_,RETURN_,LABEL_,IFGOTO_,IF_,GOTO_,READ_,WRITE_,CALL_,ARG_,FUNC_,PARAM_,DEC_,ARR_} kind;
union {
struct { Operand le, ri;}other;
struct { Operand right, left; }assign;
struct { Operand result, op1, op2; }binop;
struct { Operand label, op1, op2, relop; }ifgo;
}u;
InterCode last;
InterCode next;
};
struct Operand_* lookup(char *name);
void insertCode(InterCode c);
void printCode(char* name);
void printOperand(Operand op,FILE* fp);
void translate_Program(struct sign *now);
void translate_ExtDefList(struct sign *now);
void translate_ExtDef(struct sign *now);
void translate_ExtDecList(struct sign *now);

void translate_Specifier(struct sign *now);
void translate_StructSpecifier(struct sign *now);

struct Operand_* translate_VarDec(struct sign *now);
void translate_FunDec(struct sign *now);
struct Operand_* translate_VarList(struct sign *now);
struct Operand_* translate_ParamDec(struct sign *now);

void translate_CompSt(struct sign *now);
void translate_StmtList(struct sign *now);
void translate_Stmt(struct sign *now);

void translate_DefList(struct sign *now);
void translate_Def(struct sign *now);
void translate_DecList(struct sign *now);
void translate_Dec(struct sign *now);

struct Operand_* translate_Exp(struct sign *now);
struct Operand_* translate_Args(struct sign *now);
#endif
