#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include "semantic.h"
int i=0,semanticerror=0;
void Program(struct sign *now){
	ExtDefList(now->leaf);
}
void ExtDefList(struct sign *now){
	struct sign* leaf=now->leaf;
	if(leaf!=NULL){
		ExtDef(leaf);
		ExtDefList(leaf->brother);
	}
}
void ExtDef(struct sign *now){
	struct sign* leaf=now->leaf,*brother;
	Type type;
	type=Specifier(leaf);
	brother=leaf->brother;
	if(strcmp(brother->type,"ExtDecList")==0){
		ExtDecList(brother,type);
	}
	else if(strcmp(brother->type,"SEMI")==0)return ;
	else{
		FunDec(brother);
		CompSt(brother->brother,type);
	}
}
void ExtDecList(struct sign *now,Type type){
	struct sign* leaf=now->leaf,*brother;
	VarDec(leaf,type);
	brother=leaf->brother;
	if(brother!=NULL){
		ExtDecList(brother->brother,type);
	}
}

Type Specifier(struct sign *now){
	struct sign* leaf=now->leaf;
	Type type;
	if(strcmp(leaf->type,"TYPE")==0){
		struct Type_* type=malloc(sizeof(struct Type_));
		type->kind=0;
		if(strcmp(leaf->text,"int")==0){
			type->u.basic=0;
			return type;
		}
		else if(strcmp(leaf->text,"float")==0){
			type->u.basic=1;
			return type;
		}
	}
	else{
		type=StructSpecifier(leaf);
		return type;
	}
}
Type StructSpecifier(struct sign *now){
	struct sign* leaf=now->leaf;
	struct Type_* type=malloc(sizeof(struct Type_));
	type->kind=2;
	struct FieldList_* sym=malloc(sizeof(struct FieldList_));
	sym->type=type;
	while(leaf!=NULL){
		if(strcmp(leaf->type,"OptTag")==0){
			if(leaf->leaf!=NULL){
				strcpy(sym->name,leaf->leaf->text);
				if(insert_table(sym)){
					printf("Error type 16 at Line %d: Duplicated name '%s'.\n",leaf->line,sym->name);
					semanticerror++;
					return NULL;
				}
			}
		}
		else if(strcmp(leaf->type,"Tag")==0){
			leaf=leaf->leaf;
			struct FieldList_* result=find_symbol(leaf->text);
			if(result==NULL||result->type->kind!=2){
				printf("Error type 17 at Line %d: Undefined structure '%s'.\n",leaf->line,leaf->text);
				semanticerror++;
				return NULL;
			}
			return result->type;
		}
		else if(strcmp(leaf->type,"DefList")==0){
			type->u.structure=DefList(leaf);
			return type;
		}
		leaf=leaf->brother;
	}
}
struct FieldList_* VarDec(struct sign *now,Type type){
	struct sign* leaf=now->leaf,*brother;
	FieldList sym;
	if(strcmp(leaf->type,"ID")==0){
		struct FieldList_* sym=malloc(sizeof(struct FieldList_));
		sym->type=type;
		strcpy(sym->name,leaf->text);//printf("6748632\n");
		sym->hash=NULL;//printf("6748632\n");
		if(insert_table(sym)){
			printf("Error type 3 at Line %d: Redefined variable '%s'.\n",leaf->line,leaf->text);
			semanticerror++;
			return NULL;
		}
		else{
			/*printf("symbol at Line %d: '%s'.\n",leaf->line,leaf->text);*/
			return sym;
		}
	}
	else{
		sym=VarDec(leaf,type);
		if(sym==NULL){
			return NULL;
		}
		brother=leaf->brother;
		Type temp=sym->type;
		struct Type_* array=malloc(sizeof(struct Type_));
		array->kind=1;
		array->u.array.size=atoi(brother->text);
		array->u.array.elem=type;
		if(temp->kind!=1){
			sym->type=array;
			return sym;
		}
		while(temp->u.array.elem->kind==1)
		{
			temp=temp->u.array.elem;
		}
		temp->u.array.elem=array;
		return sym;
	}
}
struct Functype_* FunDec(struct sign *now){
	struct sign* leaf=now->leaf;
	struct Functype_* fun=malloc(sizeof(struct Functype_));
	strcpy(fun->name,leaf->text);
	fun->hash=NULL;
	fun->param=NULL;
	leaf=leaf->brother;
	leaf=leaf->brother;
	if(insert_func(fun)){
		printf("Error type 4 at Line %d: Redefined function '%s'.\n",leaf->line,fun->name);
		semanticerror++;
		return NULL;
	}
	else{
		if(strcmp(leaf->type,"VarList")==0){
			fun->param=VarList(leaf);
		}
		/*printf("func at Line %d: '%s'.\n",leaf->line,now->leaf->text);*/
		return fun;
	}
}
struct FieldList_* VarList(struct sign *now){
	struct sign* leaf=now->leaf,*brother;
	struct FieldList_* sym;
	sym=ParamDec(leaf);
	brother=leaf->brother;
	if(brother!=NULL){
		FieldList t=sym;
		brother=brother->brother;
		if(t==NULL)sym=VarList(brother);
		else{
			while(t->tail!=NULL)t=t->tail;
			t->tail=VarList(brother);
		}
	}
	return sym;
}
struct FieldList_* ParamDec(struct sign *now){
	struct sign* leaf=now->leaf;
	struct FieldList_* sym;
	Type type;
	type=Specifier(leaf);
	sym=VarDec(leaf->brother,type);
	return sym;
}
void CompSt(struct sign *now,Type type){
	struct sign* leaf=now->leaf,*brother;
	brother=leaf->brother;
	DefList(brother);
	StmtList(brother->brother,type);
	return ;
}
void StmtList(struct sign *now,Type type){
	struct sign* leaf=now->leaf;
	if(leaf==NULL){
		return;
	}
	Stmt(leaf,type);
	StmtList(leaf->brother,type);
}
void Stmt(struct sign *now,Type type){
	struct sign* leaf=now->leaf;
	while(leaf!=NULL){
		if(strcmp(leaf->type,"RETURN")==0){
			leaf=leaf->brother;
			Type result=Exp(leaf);
			if(type==NULL||result==NULL){
				return ;
			}
			if(type->kind!=result->kind||type->u.basic!=result->u.basic){
				printf("Error type 8 at Line %d: Type mismatched for return.\n",leaf->line);
				semanticerror++;
			}
		}
		else if(strcmp(leaf->type,"LP")==0){
			leaf=leaf->brother;
			Type result=Exp(leaf);
			if(result!=NULL&&result->kind==0&&result->u.basic==0){
				printf("Error type 20 at Line %d: Type of Exp between if/while/else should be INT.\n",leaf->line);
				semanticerror++;
			}
		}
		else if(strcmp(leaf->type,"Exp")==0){
			Exp(leaf);
		}
		else if(strcmp(leaf->type,"Stmt")==0){
			Stmt(leaf,type);
		}
		leaf=leaf->brother;
	}
}

FieldList DefList(struct sign *now){
	struct sign* leaf=now->leaf;
	struct FieldList_* sym;
	if(leaf==NULL){
		return NULL;
	}
	sym=Def(leaf);
	leaf=leaf->brother;
	FieldList t=sym;
	if(t!=NULL){
		while(t->tail!=NULL)
		{
			t=t->tail;
		}
		t->tail=DefList(leaf);
	}
	else sym=DefList(leaf);
	return sym;
}
FieldList Def(struct sign *now){
	struct sign* leaf=now->leaf;
	struct FieldList_* sym;
	Type type=Specifier(leaf);
	//printf("7777\n");
	//if(type!=NULL)printf("%d\n",type->u.basic);
	sym=DecList(leaf->brother,type);
	return sym;
}
FieldList DecList(struct sign *now,Type type){
	struct sign* leaf=now->leaf,*brother;
	struct FieldList_* sym;//if(type!=NULL)printf("%d\n",type->u.basic);
	sym=Dec(leaf,type);//printf("88888\n");
	brother=leaf->brother;
	if(brother!=NULL){
		brother=brother->brother;
		FieldList t=sym;
		if(t!=NULL)
		{
			while(t->tail!=NULL)t=t->tail;
			t->tail=DecList(brother,type);
		}
		else 
			t=DecList(brother,type);
	}
	return sym;
}
FieldList Dec(struct sign *now,Type type){
	struct sign* leaf=now->leaf,*brother;
	struct FieldList_* sym;//if(type!=NULL)printf("%d\n",type->u.basic);
	sym=VarDec(leaf,type);
	brother=leaf->brother;
	if(brother!=NULL){
		brother=brother->brother;
		Type result=Exp(brother);//printf("1111\n");
		if(type!=NULL&&result!=NULL&&(type->kind!=result->kind||type->u.basic!=result->u.basic)){
				printf("Error type 5 at Line %d: Type mismatched for assignment.\n",brother->line);
				semanticerror++;
		}
	}//printf("12542278\n");
	return sym;
}

Type Exp(struct sign *now){//printf("99999999999\n");
	struct sign* leaf=now->leaf,*brother=leaf->brother;
	Type type;
	if(strcmp(leaf->type,"LP")==0){
		return Exp(brother);
	}
	else if(strcmp(leaf->type,"INT")==0){//printf("99999999999\n");
		struct Type_* type=malloc(sizeof(struct Type_));
		type->kind=0;
		type->u.basic=0;
		//printf("%s:%d\n",leaf->text,type->u.basic);
		return type;
	}
	else if(strcmp(leaf->type,"FLOAT")==0){
		struct Type_* type=malloc(sizeof(struct Type_));
		type->kind=0;
		type->u.basic=1;
		return type;
	}
	else if(strcmp(leaf->type,"ID")==0&&brother!=NULL){
		FieldList result=find_symbol(leaf->text);
		Functype function=find_func(leaf->text);
		if(function==NULL){
			if(result==NULL){
				printf("Error type 2 at Line %d: Undefined function '%s'.\n",leaf->line,leaf->text);
				semanticerror++;
			return NULL;
			}
			else{
				printf("Error type 11 at Line %d:'%s' is not a function.\n",leaf->line,leaf->text);
				semanticerror++;
			return NULL;
			}
		}
		FieldList param=function->param;
		leaf=leaf->brother;
		leaf=leaf->brother;
		if(strcmp(leaf->type,"RP")==0){
			if(param!=NULL){
				printf("Error type 9 at line %d:Function '%s' is not applicable for arguments.\n",leaf->line,function->name);
				semanticerror++;
			}
		}
		else{
			if(!Args(leaf,param)){
				printf("Error type 9 at line %d:Function '%s' is not applicable for arguments.\n",leaf->line,function->name);
				semanticerror++;
			}
		}
		return function->type;
	}
	else if(strcmp(leaf->type,"ID")==0){
		FieldList result=find_symbol(leaf->text);
		if(result!=NULL){
			return result->type;
		}
		else{
			printf("Error type 1 at Line %d: Undefined variable '%s'.\n",leaf->line,leaf->text);
			semanticerror++;
			return NULL;
		}
	}
	else if(strcmp(leaf->type,"MINUS")==0){
		Type type=Exp(brother);
		if(type==NULL)return NULL;
		if(type->kind!=0){
			printf("Error type 7 at Line %d: Type mismatched for operands.\n",brother->line);
			semanticerror++;
		}
	}
	else if(strcmp(leaf->type,"NOT")==0)	
	{
		leaf=leaf->brother;
		Type type=Exp(leaf);
		if(type==NULL)return NULL;
		if(type->kind==0&&type->u.basic==0)return type;
		printf("Error type 7 at line %d: Type mismatched for operands\n",leaf->line);
		semanticerror++;
		return NULL;
	}
	else if(strcmp(leaf->type,"Exp")==0){	
		struct sign *rel=leaf->brother;
		if(strcmp(rel->type,"ASSIGNOP")==0)			
		{
			struct sign *left=leaf->leaf;
			Type leftType=NULL;
			if(strcmp(left->type,"ID")==0&&left->brother==NULL)
				leftType=Exp(leaf);
			else if(strcmp(left->type,"Exp")==0&&left->brother!=NULL&&strcmp(left->brother->type,"LB")==0)
				leftType=Exp(leaf);
			else if(strcmp(left->type,"Exp")==0&&left->brother!=NULL&&strcmp(left->brother->type,"DOT")==0){
				leftType=Exp(leaf);
			}
			else
				printf("Error type 6 at line %d: The left-hand side of an assignment must be a variable\n",leaf->line);
				semanticerror++;
			rel=rel->brother;
			Type rightType=Exp(rel);
			if(leftType==NULL||rightType==NULL)return NULL;
			if(leftType->kind==rightType->kind&&leftType->u.basic==rightType->u.basic){
				return leftType;
			}
			else{
				printf("Error type 5 at line %d: Type mismatched for assignment\n",leaf->line);
				semanticerror++;
				return NULL;
			}
		}
		else if(strcmp(rel->type,"PLUS")==0||strcmp(rel->type,"MINUS")==0||strcmp(rel->type,"STAR")==0||strcmp(rel->type,"DIV")==0||strcmp(rel->type,"RELOP")==0){
			//printf("575156456\n");
			Type t1=Exp(leaf);
			rel=rel->brother;
			Type t2=Exp(rel);
			//printf("874551254556\n");
			if(t1==NULL||t2==NULL)return NULL;
			else if(t1->kind==0&&t2->kind==0&&t1->u.basic==t2->u.basic)
				return t1;
			else{
				printf("Error type 7 at line %d: Type mismatched for operands\n",leaf->line);
				semanticerror++;
				return NULL;
			}
		}
		else if(strcmp(rel->type,"LB")==0)
		{
			Type t1=Exp(leaf);
			
			if(t1==NULL)return NULL;
			if(t1->kind!=1){
				printf("Error type 10 at line %d: '%s'is not an array\n",leaf->line,leaf->text);
				semanticerror++;
				return NULL;
			}
			rel=rel->brother;
			Type t2=Exp(rel);
			if(t2==NULL)return NULL;
			if(t2->kind!=0||t2->u.basic!=0)
			{
				printf("Error type 12 at line %d: '%s' is not an integer\n",rel->line,rel->leaf->text);
				semanticerror++;
				return NULL;
			}
			return t1->u.array.elem;
		}
		else if(strcmp(rel->type,"DOT")==0)	
		{
			Type t1=Exp(leaf);
			if(t1==NULL)return NULL;
			if(t1->kind!=2)
			{
				printf("Error type 13 at line %d: Illegal use of '.'\n",leaf->line);
				semanticerror++;
				return NULL;
			}
			FieldList fl=t1->u.structure;
			rel=rel->brother;
			while(fl!=NULL)
			{
				if(strcmp(fl->name,rel->text)==0)
					return fl->type;
				fl=fl->tail;
			}
			printf("Error type 14 at line %d: Non-existent field '%s‘\n",rel->line,rel->text);
			semanticerror++;
			return NULL;
		}

	}
}
int Args(struct sign *now,FieldList sym){
	if(now==NULL&&sym==NULL)return 1;
	else if(now==NULL||sym==NULL)return 0;
	struct sign *leaf=now->leaf;
	Type type=Exp(leaf);
	if(type==NULL)return 1;		
	if(leaf->brother==NULL&&sym->tail==NULL)return 1;
	else if(leaf->brother==NULL||sym->tail==NULL)return 0;
	return Args(leaf->brother->brother,sym->tail);
}
