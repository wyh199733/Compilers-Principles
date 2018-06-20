#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include "symbol.h"
struct FieldList_* varTable[max];
struct Functype_* funcTable[max];
unsigned int hash_pjw(char* name){
	unsigned int val = 0, i;
	for (; *name; ++name)
	{
		val = (val << 2) + *name;
		if (i = val & ~0x3fff) val = (val ^ (i >> 12)) & 0x3fff;	
	}
	return val;
}
void table(){
	int i;
	for(i=0;i<max;i++){
		varTable[i]=NULL;
		funcTable[i]=NULL;
	}
}
int insert_table(FieldList now){
	if(now->name==NULL)return 0;
	unsigned int num=hash_pjw(now->name);
	if(varTable[num]==NULL){
		varTable[num]=now;
	}
	else{
		FieldList hang=varTable[num];
		if(strcmp(hang->name,now->name)==0)return 1;
		else{
			while(hang->hash!=NULL){
			hang=hang->hash;
			if(strcmp(hang->name,now->name)==0)return 1;
			}
			hang->hash=now;
		}
	}
	return 0;
}
int insert_func(Functype now){
	if(now->name==NULL)return 0;
	unsigned int num=hash_pjw(now->name);
	if(funcTable[num]==NULL){
		funcTable[num]=now;
		insert_Param(now);
	}
	else{
		Functype hang=funcTable[num];
		if(strcmp(hang->name,now->name)==0)return 1;
		else{
			while(hang->hash!=NULL){
			hang=hang->hash;
			if(strcmp(hang->name,now->name)==0)return 1;
			}
			hang->hash=now;
		}
		insert_Param(now);
	}
	return 0;
}
struct FieldList_* find_symbol(char *name){
	unsigned int num=hash_pjw(name);
	if(varTable[num]==NULL)return NULL;
	FieldList now=varTable[num];
	while(now!=NULL){
		if(strcmp(now->name,name)==0)return now;
		now=now->hash;
	}
	return NULL;
}
struct Functype_* find_func(char *name){
	unsigned int num=hash_pjw(name);
	if(funcTable[num]==NULL)return NULL;
	Functype now=funcTable[num];
	while(now!=NULL){
		if(strcmp(now->name,name)==0)return now;
		now=now->hash;
	}
	return NULL;
}
void insert_Param(Functype now){
	FieldList param=now->param;
	int i=0;
	while(param!=NULL)
	{
		i=insert_table(param);
		if(i==1)
		{
			printf("Error type 3 at line : Redefined variable'%s'\n",param->name);
		}
		param=param->tail;
	}
}
