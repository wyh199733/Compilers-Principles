#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include "intercode.h"
int varCount=1;
int labelCount=1;
InterCode codehead=NULL;
InterCode codenow=NULL;
Operand labeltrue,labelfalse;
void insertCode(InterCode code){
	code->last=NULL;
	code->next=NULL;
	if(codehead==NULL){
		codehead=code;
		codenow=code;
	}
	else{
		codenow->next=code;
		code->last=codenow;
		codenow=code;
	}
}
struct Operand_* lookup(char *name){
	InterCode temp=codehead;
	while(temp!=NULL){
		if(temp->kind==DEC_){
			Operand op1,op2;
			op1=temp->u.other.le;
			op2=temp->u.other.ri;
			if(strcmp(op1->u.name,name)==0)
			return op2;
		}
		temp=temp->next;
	}
	return NULL;
}
void printCode(char* fname){//printf("printstart\n");
	FILE *fp=fopen(fname,"w");
	char var[32];
	int array=1;
	if(fp==NULL)
	{
		printf("open file error\n");
		return;
	}
	InterCode code=codehead;
	Operand temp;
	/*while(code!=NULL){
		switch(code->kind){
			case ASSIGN_ :printf("321\n");
				printOperand(code->u.assign.left,fp);
				fputs("\t:=\t",fp);
				printOperand(code->u.assign.right,fp);break;
			case MIN_ :printf("789\n");
				printOperand(code->u.assign.left,fp);
				fputs("\t:=\t#0\t-\t",fp);
				printOperand(code->u.assign.right,fp);break;
			case ADD_ :
				printOperand(code->u.binop.result,fp);
				fputs("\t:=\t",fp);
				printOperand(code->u.binop.op1,fp);
				fputs("\t+\t",fp);
				printOperand(code->u.binop.op2,fp);break;
			case SUB_ :
				printOperand(code->u.binop.result,fp);
				fputs("\t:=\t",fp);
				printOperand(code->u.binop.op1,fp);
				fputs("\t-\t",fp);
				printOperand(code->u.binop.op2,fp);break;
			case MUL_ :
				printOperand(code->u.binop.result,fp);
				fputs("\t:=\t",fp);
				printOperand(code->u.binop.op1,fp);
				fputs("\t*\t",fp);
				printOperand(code->u.binop.op2,fp);break;
			case DIV_ :
				printOperand(code->u.binop.result,fp);
				fputs("\t:=\t",fp);
				printOperand(code->u.binop.op1,fp);
				fputs("\t/\t",fp);
				printOperand(code->u.binop.op2,fp);break;
			case RETURN_ :
				fputs("RETURN\t",fp);
				printOperand(code->u.other.le,fp);
				fputs("\n",fp);break;
			case LABEL_ :printf("LABEL\n");
				fputs("LABEL\t",fp);
				printOperand(code->u.other.le,fp);
				fputs("\t:",fp);break;
			case GOTO_ :printf("GOTO\n");
				fputs("GOTO\t",fp);
				printOperand(code->u.other.le,fp);break;
			case IFGOTO_ :printf("IFGOTO\n");
				printf("1\n");fputs("IF\t",fp);
				printf("2\n");printOperand(code->u.ifgo.op1,fp);fputs("\t",fp);
				printf("3\n");printOperand(code->u.ifgo.relop,fp);fputs("\t",fp);
				printf("4\n");printOperand(code->u.ifgo.op2,fp);fputs("\t",fp);
				printf("5\n");fputs("GOTO\t",fp);
				printf("6\n");printOperand(code->u.ifgo.label,fp);break;
			case IF_ :break;
			case READ_ :printf("159\n");
				fputs("READ\t",fp);
				printOperand(code->u.other.le,fp);
				break;
			case WRITE_ :
				printOperand(code->u.other.le,fp);
				fputs("\t:=\t",fp);
				printOperand(code->u.other.ri,fp);
				fputs("\n",fp);
				fputs("WRITE\t",fp);
				printOperand(code->u.other.le,fp);break;
			case CALL_ :printf("258\n");
				printOperand(code->u.other.le,fp);
				fputs("\t:=\tCALL\t",fp);
				printOperand(code->u.other.ri,fp);break;
			case ARG_ :
				temp=code->u.other.le;
				do{
					fputs("ARG\t",fp);
					printOperand(temp,fp);
					temp=temp->next;
					if(temp!=NULL){
						fputs("\n",fp);
					}
				}while(temp!=NULL);break;
			case FUNC_ :
				fputs("FUNCTION\t",fp);
				printOperand(code->u.other.le,fp);
				fputs("\t:",fp);break;
			case PARAM_ :
				temp=code->u.other.le;
				do{
					fputs("PARAM\t",fp);
					printOperand(temp,fp);
					temp=temp->next;
					if(temp!=NULL){
						fputs("\n",fp);
					}
				}while(temp!=NULL);break;
			case DEC_ :
				fputs("DEC\t",fp);
				fputs(code->u.other.le->u.name,fp);
				temp=code->u.other.ri;
				while(temp!=NULL){
				printf("%d\n",temp->u.value1);
				array=array*(temp->u.value1);
				temp=temp->next;
				}
				sprintf(var,"\t%d",array*4);
				fputs(var,fp);break;
		}
		fputs("\n",fp);
		code=code->next;
	}*/
	printObjectCode(codehead,fp);
}
void printOperand(Operand op,FILE* fp){
	char var[32];
	memset(var,0,sizeof(var));
	switch(op->kind){
		case TEMPVAR :
			fputs("t",fp);
			sprintf(var,"%d",op->u.var_no);
			fputs(var,fp);break;
		case VARIABLE :printf("var");
			fputs(op->u.name,fp);break;
		case CONSTANT :
			fputs("#",fp);
			sprintf(var,"%d",op->u.value1);
			fputs(var,fp);break;
		case ADDRESS :printf("123456789");
			fputs(op->u.name,fp);
			break;
		case LABEL :
			fputs("label",fp);
			sprintf(var,"%d",op->u.var_no);
			fputs(var,fp);break;
			break;
		case FUNCTION:
			fputs(op->u.name,fp);break;
	}
}
void translate_Program(struct sign *now){printf("%s:%d\n",now->type,now->line);
	translate_ExtDefList(now->leaf);
}
void translate_ExtDefList(struct sign *now){printf("%s:%d\n",now->type,now->line);
	struct sign* leaf=now->leaf;
	if(leaf!=NULL){
		translate_ExtDef(leaf);
		translate_ExtDefList(leaf->brother);
	}
}
void translate_ExtDef(struct sign *now){printf("%s:%d\n",now->type,now->line);
	struct sign* leaf=now->leaf,*brother;
	brother=leaf->brother;
	if(strcmp(brother->type,"ExtDecList")==0)return;
	else if(strcmp(brother->type,"SEMI")==0)return;
	else{
		translate_FunDec(brother);
		translate_CompSt(brother->brother);
	}
}
void translate_FunDec(struct sign *now){printf("%s:%d\n",now->type,now->line);
	struct sign* leaf=now->leaf,*brother=leaf->brother;
	brother=brother->brother;
	InterCode code2=malloc(sizeof(struct InterCode_));
	Operand op1=malloc(sizeof(struct Operand_));
	strcpy(op1->u.name,leaf->text);
	op1->kind=FUNCTION;
	code2->kind=FUNC_;
	code2->u.other.le=op1;
	insertCode(code2);
	if(strcmp(brother->type,"VarList")==0){
		Operand op;
		op=translate_VarList(brother);
		InterCode code1=malloc(sizeof(struct InterCode_));
		code1->kind=PARAM_;
		code1->u.other.le=op;
		insertCode(code1);
	}
}
struct Operand_* translate_VarList(struct sign *now){printf("%s:%d\n",now->type,now->line);
	struct sign* leaf=now->leaf,*brother;
	Operand op1,op2,temp;
	op1=translate_ParamDec(leaf);
	temp=op1;
	brother=leaf->brother;
	if(brother!=NULL){
		brother=brother->brother;
		op2=translate_VarList(brother);
		temp->next=op2;
		temp=op2;
	}
	return op1;
}
struct Operand_* translate_ParamDec(struct sign *now){printf("%s:%d\n",now->type,now->line);
	struct sign* leaf=now->leaf,*brother=leaf->brother;
	Operand op=malloc(sizeof(struct Operand_));
	leaf=brother->leaf;
	op->kind=VARIABLE;
	strcpy(op->u.name,leaf->text);
	op->next=NULL;
	return op;
}

void translate_CompSt(struct sign *now){printf("%s:%d\n",now->type,now->line);
	struct sign* leaf=now->leaf,*brother;
	brother=leaf->brother;
	translate_DefList(brother);
	translate_StmtList(brother->brother);
}
void translate_StmtList(struct sign *now){printf("%s:%d\n",now->type,now->line);
	struct sign* leaf=now->leaf;
	if(leaf==NULL){
		return;
	}
	translate_Stmt(leaf);printf("123456789\n%d",leaf->line);
	translate_StmtList(leaf->brother);
}
void translate_Stmt(struct sign *now){printf("%s:%d\n",now->type,now->line);
	struct sign* leaf=now->leaf,*brother;
	if(strcmp(leaf->type,"RETURN")==0){printf("return\n");
		leaf=leaf->brother;
		Operand op=translate_Exp(leaf);
		InterCode code=malloc(sizeof(struct InterCode_));
		code->kind=RETURN_;
		code->u.other.le=op;
		insertCode(code);
	}
	else if(strcmp(leaf->type,"Exp")==0){
		translate_Exp(leaf);
	}
	else if(strcmp(leaf->type,"Stmt")==0){
		translate_Stmt(leaf);
	}
	else if(strcmp(leaf->type,"CompSt")==0){
		translate_CompSt(leaf);
	}
	else if(strcmp(leaf->type,"WHILE")==0){
		Operand la1=malloc(sizeof(struct Operand_));
		la1->kind=LABEL;
		la1->u.var_no=labelCount++;
		Operand la2=malloc(sizeof(struct Operand_));
		la2->kind=LABEL;
		la2->u.var_no=labelCount++;
		Operand la3=malloc(sizeof(struct Operand_));
		la3->kind=LABEL;
		la3->u.var_no=labelCount++;
		labeltrue=la2;labelfalse=la3;
		InterCode code1=malloc(sizeof(struct InterCode_));
		code1->kind=LABEL_;
		code1->u.other.le=la1;
		insertCode(code1);
		translate_Exp(leaf->brother->brother);
		InterCode code2=malloc(sizeof(struct InterCode_));
		code2->kind=LABEL_;
		code2->u.other.le=la2;
		insertCode(code2);
		translate_Stmt(leaf->brother->brother->brother->brother);
		InterCode code3=malloc(sizeof(struct InterCode_));
		code3->kind=GOTO_;
		code3->u.other.le=la1;
		insertCode(code3);
		InterCode code4=malloc(sizeof(struct InterCode_));
		code4->kind=LABEL_;
		code4->u.other.le=la3;
		insertCode(code4);
	}
	else if(strcmp(leaf->type,"IF")==0){
		brother=leaf->brother;
		brother=brother->brother;
		brother=brother->brother;
		brother=brother->brother;
		brother=brother->brother;
		if(brother==NULL){printf("IF\n");
			Operand la1=malloc(sizeof(struct Operand_));
			la1->kind=LABEL;
			la1->u.var_no=labelCount++;
			Operand la2=malloc(sizeof(struct Operand_));
			la2->kind=LABEL;
			la2->u.var_no=labelCount++;
			labeltrue=la1;labelfalse=la2;
			translate_Exp(leaf->brother->brother);
			InterCode code1=malloc(sizeof(struct InterCode_));
			code1->kind=LABEL_;
			code1->u.other.le=la1;
			insertCode(code1);
			translate_Stmt(leaf->brother->brother->brother->brother);
			//if(leaf->root->brother!=NULL){
				//if(leaf->root->brother->leaf!=NULL){
				InterCode code5=malloc(sizeof(struct InterCode_));
				code5->kind=GOTO_;
				code5->u.other.le=la2;
				insertCode(code5);
				InterCode code2=malloc(sizeof(struct InterCode_));
				code2->kind=LABEL_;
				code2->u.other.le=la2;
				insertCode(code2);
				//}
			//}
		}
		else{printf("IFELSE\n");
			Operand la1=malloc(sizeof(struct Operand_));
			la1->kind=LABEL;
			la1->u.var_no=labelCount++;
			Operand la2=malloc(sizeof(struct Operand_));
			la2->kind=LABEL;
			la2->u.var_no=labelCount++;
			Operand la3=malloc(sizeof(struct Operand_));
			la3->kind=LABEL;
			la3->u.var_no=labelCount++;
			labeltrue=la1;labelfalse=la2;
			translate_Exp(leaf->brother->brother);
			InterCode code1=malloc(sizeof(struct InterCode_));
			code1->kind=LABEL_;
			code1->u.other.le=la1;
			insertCode(code1);
			translate_Stmt(leaf->brother->brother->brother->brother);
			//if(leaf->root->brother!=NULL){
				//if(leaf->root->brother->leaf!=NULL){
				InterCode code3=malloc(sizeof(struct InterCode_));
				code3->kind=GOTO_;
				code3->u.other.le=la3;
				insertCode(code3);
				//}
			//}
			InterCode code2=malloc(sizeof(struct InterCode_));
			code2->kind=LABEL_;
			code2->u.other.le=la2;
			insertCode(code2);
			translate_Stmt(brother->brother);
			//if(leaf->root->brother!=NULL){
				//if(leaf->root->brother->leaf!=NULL){
				InterCode code5=malloc(sizeof(struct InterCode_));
				code5->kind=GOTO_;
				code5->u.other.le=la3;
				insertCode(code5);
				InterCode code4=malloc(sizeof(struct InterCode_));
				code4->kind=LABEL_;
				code4->u.other.le=la3;
				insertCode(code4);
				//}
			//}
		}
	}
}

void translate_DefList(struct sign *now){printf("%s:%d\n",now->type,now->line);
	struct sign* leaf=now->leaf;
	if(leaf==NULL){
		return;
	}
	translate_Def(leaf);
	leaf=leaf->brother;
	translate_DefList(leaf);
}
void translate_Def(struct sign *now){printf("%s:%d\n",now->type,now->line);
	struct sign* leaf=now->leaf;
	translate_DecList(leaf->brother);
}
void translate_DecList(struct sign *now){printf("%s:%d\n",now->type,now->line);
	struct sign* leaf=now->leaf,*brother;
	translate_Dec(leaf);
	brother=leaf->brother;
	if(brother!=NULL){
		brother=brother->brother;
		translate_DecList(brother);
	}
}
void translate_Dec(struct sign *now){printf("%s:%d\n",now->type,now->line);
	struct sign* leaf=now->leaf,*brother;
	if(leaf->leaf->brother!=NULL){
		translate_VarDec(leaf);
	}
	brother=leaf->brother;
	printf("123\n");
	if(brother!=NULL){
		printf("%s\t:=\t",leaf->leaf->text);
		brother=brother->brother;
		Operand op2;
		Operand op1=malloc(sizeof(struct Operand_));
		op1->kind=VARIABLE;
		strcpy(op1->u.name,leaf->leaf->text);
		op2=translate_Exp(brother);
		InterCode code=malloc(sizeof(struct InterCode_));
		code->kind=ASSIGN_;
		code->u.assign.left=op1;
		code->u.assign.right=op2;
		insertCode(code);
	}
}
struct Operand_* translate_VarDec(struct sign *now){printf("%s:%d\n",now->type,now->line);
	struct sign* leaf=now->leaf,*brother=leaf->brother;
	brother=brother->brother;
	if(strcmp(leaf->leaf->type,"ID")==0){printf("486\n");
		InterCode code=malloc(sizeof(struct InterCode_));
		Operand op2=malloc(sizeof(struct Operand_));
		op2->kind=ADDRESS;
		op2->u.value1=atoi(brother->text);printf("%d\n",op2->u.value1);
		op2->next=NULL;
		Operand op1=malloc(sizeof(struct Operand_));
		op1->kind=ADDRESS;/*printf("%s\n",brother->type);printf("%s\n",brother->text);*/
		strcpy(op1->u.name,leaf->leaf->text);
		op1->next=op2;
		code->kind=DEC_;
		code->u.other.le=op1;
		code->u.other.ri=op2;
		insertCode(code);
		return op2;
	}
	else{printf("426\n");
		Operand op2,temp;
		Operand op1=malloc(sizeof(struct Operand_));
		op1->kind=CONSTANT;
		op1->u.value1=atoi(brother->text);/*printf("%s\n",brother->text);*/
		op1->next=NULL;
		op2=translate_VarDec(leaf);printf("%d\n",op2->u.value1);
		temp=op2;
		if(temp->next!=NULL){
			temp=temp->next;
		}
		/*printf("%d\n",temp->kind);*/
		temp=temp->next=op1;
		return op2;
	}
}
struct Operand_* translate_Exp(struct sign *now){printf("%s:%d\n",now->type,now->line);
	struct sign* leaf=now->leaf,*brother=leaf->brother;
	if(strcmp(leaf->type,"LP")==0){
		translate_Exp(brother);
	}
	else if(strcmp(leaf->type,"INT")==0){
		Operand con=malloc(sizeof(struct Operand_));
		con->kind=CONSTANT;
		con->u.value1=atoi(leaf->text);
		con->next=NULL;
		return con;
	}
	else if(strcmp(leaf->type,"FLOAT")==0){
		Operand con=malloc(sizeof(struct Operand_));
		con->kind=CONSTANT;
		con->u.value2=atof(leaf->text);
		con->next=NULL;
		return con;
	}
	else if(strcmp(leaf->type,"ID")==0&&brother!=NULL){
		brother=brother->brother;
		brother=brother->brother;
		if(brother==NULL){
			if(strcmp(leaf->text,"read")==0){printf("753\n");
				InterCode code=malloc(sizeof(struct InterCode_));
				Operand op1=malloc(sizeof(struct Operand_));
				op1->kind=TEMPVAR;
				op1->u.var_no=varCount++;
				Operand op2=malloc(sizeof(struct Operand_));
				strcpy(op2->u.name,leaf->text);
				op2->kind=VARIABLE;
				code->kind=READ_;
				code->u.other.le=op1;
				code->u.other.ri=op2;
				insertCode(code);
				return op1;
			}
			else{
				InterCode code=malloc(sizeof(struct InterCode_));
				Operand op1=malloc(sizeof(struct Operand_));
				op1->kind=TEMPVAR;
				op1->u.var_no=varCount++;
				Operand op2=malloc(sizeof(struct Operand_));
				strcpy(op2->u.name,leaf->text);
				op2->kind=FUNCTION;
				code->kind=FUNC_;
				code->u.other.le=op1;
				code->u.other.ri=op2;
				insertCode(code);
				return op1;
			}
		}
		else{
			if(strcmp(leaf->text,"write")==0){printf("357\n");
				InterCode code=malloc(sizeof(struct InterCode_));
				Operand op1=malloc(sizeof(struct Operand_));
				op1->kind=TEMPVAR;
				op1->u.var_no=varCount++;
				Operand op2=translate_Args(leaf->brother->brother);
				code->kind=WRITE_;
				code->u.other.le=op1;
				code->u.other.ri=op2;
				insertCode(code);
				return NULL;
			}
			else{
				InterCode code1=malloc(sizeof(struct InterCode_));
				Operand op=translate_Args(leaf->brother->brother);
				code1->kind=ARG_;
				code1->u.other.le=op;
				insertCode(code1);
				InterCode code2=malloc(sizeof(struct InterCode_));
				Operand op1=malloc(sizeof(struct Operand_));
				op1->kind=TEMPVAR;
				op1->u.var_no=varCount++;
				Operand op2=malloc(sizeof(struct Operand_));
				strcpy(op2->u.name,leaf->text);
				op2->kind=FUNCTION;
				code2->kind=CALL_;
				code2->u.other.le=op1;
				code2->u.other.ri=op2;
				insertCode(code2);
				return op1;
			}
		}
	}
	else if(strcmp(leaf->type,"ID")==0){printf("167\n");
		printf("%s\n",leaf->text);
		Operand var=malloc(sizeof(struct Operand_));
		var->kind=VARIABLE;
		strcpy(var->u.name,leaf->text);
		var->next=NULL;
		return var;
	}
	else if(strcmp(leaf->type,"MINUS")==0){printf("456\n");
		InterCode code=malloc(sizeof(struct InterCode_));
		Operand op2;
		Operand op1=malloc(sizeof(struct Operand_));
		op2=translate_Exp(brother);
		op1->kind=TEMPVAR;
		op1->u.var_no=varCount++;
		code->u.assign.left=op1;
		code->u.assign.right=op2;
		code->kind=MIN_;
		insertCode(code);
		return op1;
	}
	else if(strcmp(leaf->type,"NOT")==0){
			Operand temp;
			temp=labeltrue;
			labeltrue=labelfalse;
			labelfalse=temp;
			temp=translate_Exp(leaf->brother);
			return temp;
	}
	else if(strcmp(leaf->type,"Exp")==0){printf("111\n");
		struct sign *rel=leaf->brother;
		if(strcmp(rel->type,"ASSIGNOP")==0){printf("222\n");
			rel=rel->brother;
			struct sign *left=leaf,*right=rel;
			Operand op1,op2;
			op1=translate_Exp(left);
			//printf("\t:=\t");
			op2=translate_Exp(right);
			InterCode code=malloc(sizeof(struct InterCode_));
			code->kind=ASSIGN_;
			code->u.assign.left=op1;
			code->u.assign.right=op2;
			insertCode(code);
			return op1;
		}
		else if(strcmp(rel->type,"PLUS")==0||strcmp(rel->type,"MINUS")==0||strcmp(rel->type,"STAR")==0||strcmp(rel->type,"DIV")==0){
			rel=rel->brother;
			struct sign *left=leaf,*right=rel;
			brother=left->brother;
			Operand op1,op2;
			op1=translate_Exp(left);
			//printf("\t+\t");
			op2=translate_Exp(right);
			Operand result=malloc(sizeof(struct Operand_));
			result->kind=TEMPVAR;
			result->u.var_no=varCount++;
			InterCode code=malloc(sizeof(struct InterCode_));
			code->u.binop.op1=op1;
			code->u.binop.op2=op2;
			code->u.binop.result=result;
			if(strcmp(brother->type,"PLUS")==0)
					code->kind=ADD_;
			else if(strcmp(brother->type,"MINUS")==0)
					code->kind=SUB_;
			else if(strcmp(brother->type,"STAR")==0)
					code->kind=MUL_;
			else if(strcmp(brother->type,"DIV")==0)
					code->kind=DIV_;
			insertCode(code);
			return result;
		}
		else if(strcmp(rel->type,"RELOP")==0){printf("relop\n");
			Operand op1,op2;
			op1=translate_Exp(leaf);
			op2=translate_Exp(leaf->brother->brother);
			Operand relop=malloc(sizeof(struct Operand_));
			relop->kind=VARIABLE;
			strcpy(relop->u.name,rel->text);
			InterCode code1=malloc(sizeof(struct InterCode_));
			code1->kind=IFGOTO_;
			code1->u.ifgo.op1=op1;
			code1->u.ifgo.op2=op2;
			code1->u.ifgo.label=labeltrue;
			code1->u.ifgo.relop=relop;
			insertCode(code1);
			InterCode code2=malloc(sizeof(struct InterCode_));
			code2->kind=GOTO_;
			code2->u.ifgo.label=labelfalse;
			insertCode(code2);
			return NULL;
		}
		else if(strcmp(rel->type,"OR")==0){
			Operand or=malloc(sizeof(struct Operand_));
			or->kind=LABEL;
			or->u.var_no=labelCount++;
			Operand temp;
			temp=labelfalse;
			labelfalse=or;
			translate_Exp(leaf);
			InterCode code1=malloc(sizeof(struct InterCode_));
			code1->kind=LABEL_;
			code1->u.other.le=or;
			insertCode(code1);
			labelfalse=temp;
			translate_Exp(leaf->brother->brother);
			return NULL;
		}
		else if(strcmp(rel->type,"AND")==0){
			Operand and=malloc(sizeof(struct Operand_));
			and->kind=LABEL;
			and->u.var_no=labelCount++;
			Operand temp;
			temp=labeltrue;
			labeltrue=and;
			translate_Exp(leaf);
			InterCode code1=malloc(sizeof(struct InterCode_));
			code1->kind=LABEL_;
			code1->u.other.le=and;
			insertCode(code1);
			labeltrue=temp;
			translate_Exp(leaf->brother->brother);
			return NULL;
		}
		else if(strcmp(rel->type,"LB")==0){printf("LB\n");
			brother=brother->brother;
			if(strcmp(leaf->leaf->type,"ID")==0){printf("asd\n");
				Operand op,temp;
				op=lookup(leaf->leaf->text);
				temp=op->next;
				Operand result=malloc(sizeof(struct Operand_));
				result->kind=TEMPVAR;
				result->u.var_no=varCount++;
				Operand op1=malloc(sizeof(struct Operand_));
				op1->kind=CONSTANT;
				op1->u.value1=atoi(brother->leaf->text);
				printf("%s\n",brother->leaf->text);
				Operand op2=malloc(sizeof(struct Operand_));
				op2->kind=CONSTANT;
				op2->u.value1=4;
				while(temp!=NULL){
					op2->u.value1*=temp->u.value1;
					temp=temp->next;
				}
				InterCode code1=malloc(sizeof(struct InterCode_));
				code1->kind=MUL_;
				code1->u.binop.op1=op1;
				code1->u.binop.op2=op2;
				code1->u.binop.result=result;
				insertCode(code1);
				Operand add=malloc(sizeof(struct Operand_));
				add->kind=ADDRESS;
				strcpy(add->u.name,"&");
				strcat(add->u.name,leaf->leaf->text);
				Operand end=malloc(sizeof(struct Operand_));
				end->kind=TEMPVAR;
				end->u.var_no=varCount++;
				InterCode code2=malloc(sizeof(struct InterCode_));
				code2->kind=ADD_;
				code2->u.binop.op1=add;
				code2->u.binop.op2=result;
				code2->u.binop.result=end;
				insertCode(code2);
				printf("tyu%s\n",leaf->root->brother->type);
				if(strcmp(leaf->root->brother->type,"LB")==0){
					printf("bnm\n");
					return op->next;
				}
				else{printf("iop\n");
					Operand put=malloc(sizeof(struct Operand_));
					char tempvar[32];
					put->kind=ADDRESS;
					strcpy(put->u.name,"*");
					sprintf(tempvar,"t%d",end->u.var_no);
					strcat(put->u.name,tempvar);
					return put;
				}
			}
			else{printf("zxc\n");
				Operand op,temp;
				char var[32];
				op=translate_Exp(leaf);printf("qwe\n");
				temp=op->next;
				Operand result=malloc(sizeof(struct Operand_));
				result->kind=TEMPVAR;
				result->u.var_no=varCount++;
				Operand op1=malloc(sizeof(struct Operand_));
				op1->kind=CONSTANT;
				op1->u.value1=atoi(brother->leaf->text);
				Operand op2=malloc(sizeof(struct Operand_));
				op2->kind=CONSTANT;
				op2->u.value1=4;
				while(temp!=NULL){
					op2->u.value1*=temp->u.value1;
					temp=temp->next;
				}
				InterCode code1=malloc(sizeof(struct InterCode_));
				code1->kind=MUL_;
				code1->u.binop.op1=op1;
				code1->u.binop.op2=op2;
				code1->u.binop.result=result;
				insertCode(code1);
				Operand add=malloc(sizeof(struct Operand_));
				add->kind=ADDRESS;
				sprintf(var,"t%d",result->u.var_no-1);
				strcpy(add->u.name,var);
				Operand end=malloc(sizeof(struct Operand_));
				end->kind=TEMPVAR;
				end->u.var_no=varCount++;
				InterCode code2=malloc(sizeof(struct InterCode_));
				code2->kind=ADD_;
				code2->u.binop.op1=add;
				code2->u.binop.op2=result;
				code2->u.binop.result=end;
				insertCode(code2);
				if(strcmp(leaf->root->brother->type,"LB")==0){
					return op->next;
				}
				else{
					Operand put=malloc(sizeof(struct Operand_));
					char tempvar[32];
					put->kind=ADDRESS;
					strcpy(put->u.name,"*");
					sprintf(tempvar,"t%d",end->u.var_no);
					strcat(put->u.name,tempvar);
					return put;
				}
			}
		}
		else if(strcmp(rel->type,"DOT")==0){
		printf("cannot translate\n");
		}
	}
}
struct Operand_* translate_Args(struct sign *now){printf("%s:%d\n",now->type,now->line);
	struct sign* leaf=now->leaf,*brother=leaf->brother;
	if(brother==NULL){printf("%s\n",leaf->leaf->text);
		return translate_Exp(leaf);
	}
	else{
		Operand op1,op2,temp;
		op1=translate_Args(brother->brother);
		op2=translate_Exp(leaf);
		/*printf("%s\n",op1->u.name);
		printf("%s\n",op2->u.name);
		printf("%d\n",op1->kind);
		printf("%d\n",op2->kind);*/
		temp=op1;
		while(temp->next!=NULL){
			temp=temp->next;
		}
		temp->next=op2;
		if(op1->next==NULL)printf("349\n");
		return op1;
	}
}
