#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include"objectcode.h"
#define REG_NUM 32
#define CAN_USE 20
RegDescipter Regs[REG_NUM];
InterCode InterCodenow =NULL;
VarDescipter *List = NULL;
StkDescipter Stack;
int arg=0;
int infunction=0,anow=-1;
void printObjectCode(InterCode InterCodehead,FILE *fp){//printf("object\n");
	int i;
	for (i=0;i<CAN_USE;i++){printf("start\n");
		Regs[i].var=NULL;
		Regs[i].old=0;//printf("end\n");
	}
	printf("startregs\n");
	strcpy(Regs[0].name, "$zero");
	strcpy(Regs[1].name, "$at");
	strcpy(Regs[2].name, "$v0");
	strcpy(Regs[3].name, "$v1");
	strcpy(Regs[4].name, "$a0");
	strcpy(Regs[5].name, "$a1");
	strcpy(Regs[6].name, "$a2");
	strcpy(Regs[7].name, "$a3");
	strcpy(Regs[8].name, "$t0");
	strcpy(Regs[9].name, "$t1");
	strcpy(Regs[10].name, "$t2");
	strcpy(Regs[11].name, "$t3");
	strcpy(Regs[12].name, "$t4");
	strcpy(Regs[13].name, "$t5");
	strcpy(Regs[14].name, "$t6");
	strcpy(Regs[15].name, "$t7");
	strcpy(Regs[16].name, "$s0");
	strcpy(Regs[17].name, "$s1");
	strcpy(Regs[18].name, "$s2");
	strcpy(Regs[19].name, "$s3");
	strcpy(Regs[20].name, "$s4");
	strcpy(Regs[21].name, "$s5");
	strcpy(Regs[22].name, "$s6");
	strcpy(Regs[23].name, "$s7");
	strcpy(Regs[24].name, "$t8");	
	strcpy(Regs[25].name, "$t9");
	strcpy(Regs[26].name, "$k0");
	strcpy(Regs[27].name, "$k1");
	strcpy(Regs[28].name, "$gp");	
	strcpy(Regs[29].name, "$sp");
	strcpy(Regs[30].name, "$fp");
	strcpy(Regs[31].name, "$ra");
	printf("regs\n");
	fputs(".data\n", fp);
	fputs("_prompt: .asciiz \"Enter an integer:\"\n", fp);
	fputs("_ret: .asciiz \"\\n\"\n", fp);
	fputs(".globl main\n", fp);
	fputs(".text\n\n", fp);
	fputs("read:\n", fp);
	fputs("  li $v0, 4\n", fp);
	fputs("  la $a0, _prompt\n", fp);
	fputs("  syscall\n", fp);
	fputs("  li $v0, 5\n", fp);
	fputs("  syscall\n", fp);
	fputs("  jr $ra\n\n", fp);
	fputs("write:\n", fp);
	fputs("  li $v0, 1\n", fp);
	fputs("  syscall\n", fp);
	fputs("  li $v0, 4\n", fp);
	fputs("  la $a0, _ret\n", fp);
	fputs("  syscall\n", fp);
	fputs("  move $v0, $0\n", fp);
	fputs("  jr $ra\n\n", fp);
	InterCodenow=InterCodehead;
	int j=1;
	while(InterCodenow!=NULL){printf("startprint:%d\n",j++);
		insertObjectCode(InterCodenow,fp);
		InterCodenow=InterCodenow->next;
	}
}
void insertObjectCode(InterCode ir,FILE *fp){//printf("%d\n",ir->kind);
	if (ir->kind == ASSIGN_){//printf("%d\n",ir->kind);
		Operand left=ir->u.assign.left;
		Operand right=ir->u.assign.right;
		if(right->kind==CONSTANT){//printf("constant\n");
			int reg_no1 = getReg(left,fp);printf("1\n");
			char str[32];
			memset(str, 0, sizeof(str));
			sprintf(str, "  li %s, %d\n", Regs[reg_no1].name, right->u.value1);printf("2\n");
			fputs(str, fp);printf("3\n");
		}
		else if(right->kind==TEMPVAR||right->kind==VARIABLE){
			int reg_no2 = getReg(right, fp);
			int reg_no1 = getReg(left, fp);
			char str[32];
			memset(str, 0, sizeof(str));
			sprintf(str, "  move %s, %s\n", Regs[reg_no1].name, Regs[reg_no2].name);
			fputs(str, fp);
		}
	}
	else if(ir->kind == MIN_){//printf("%d\n",ir->kind);
		Operand op1 = ir->u.assign.left;
		Operand op2 = ir->u.assign.right;
		int reg_no1 = getReg(op1, fp);
		char str[32];
		memset(str, 0, sizeof(str));
		if(op2->kind==CONSTANT){
			sprintf(str, "  li  %s, -%d\n",Regs[reg_no1].name,op2->u.value1);
			fputs(str, fp);
		}
		else{
			int reg_no2 = getReg(op2, fp);
			sprintf(str, "  move  %s, -%s\n",Regs[reg_no1].name,Regs[reg_no2].name);
			fputs(str, fp);
		}
	}
	else if(ir->kind == ADD_||ir->kind == SUB_||ir->kind == MUL_||ir->kind == DIV_){printf("%d\n",ir->kind);
		Operand result = ir->u.binop.result;
		Operand op1 = ir->u.binop.op1;
		Operand op2 = ir->u.binop.op2;
		//int reg_no2 = getReg(op1, fp);
		//int reg_no3 = getReg(op2, fp);
		int reg_no1 = getReg(result, fp);
		char str[32];
		memset(str, 0, sizeof(str));
		if(ir->kind == ADD_){printf("+++++++%d++++++++",ir->kind);
			if(op2->kind!=CONSTANT&&op1->kind!=CONSTANT){
				int reg_no2 = getReg(op1, fp);
				int reg_no3 = getReg(op2, fp);
				sprintf(str, "  add %s, %s, %s\n", Regs[reg_no1].name, Regs[reg_no2].name, Regs[reg_no3].name);
				fputs(str, fp);
			}
			else{
				if(op2->kind==CONSTANT&&op1->kind==CONSTANT)
				sprintf(str, "  addi %s, %d, %d\n", Regs[reg_no1].name, op1->u.value1, op2->u.value1);
				else if(op1->kind==CONSTANT)
				sprintf(str, "  addi %s, %d, %s\n", Regs[reg_no1].name, op1->u.value1, Regs[getReg(op2, fp)].name);
				else
				sprintf(str, "  addi %s, %s, %d\n", Regs[reg_no1].name, Regs[getReg(op1, fp)].name, op2->u.value1);
				fputs(str, fp);
			}
		}
		else if(ir->kind == SUB_){
			if(op2->kind!=CONSTANT&&op1->kind!=CONSTANT){
				int reg_no2 = getReg(op1, fp);
				int reg_no3 = getReg(op2, fp);
				sprintf(str, "  sub %s, %s, %s\n", Regs[reg_no1].name, Regs[reg_no2].name, Regs[reg_no3].name);
				fputs(str, fp);
			}
			else{
				if(op2->kind==CONSTANT&&op1->kind==CONSTANT)
				sprintf(str, "  addi %s, %d, -%d\n", Regs[reg_no1].name, op1->u.value1, op2->u.value1);
				else if(op1->kind==CONSTANT)
				sprintf(str, "  addi %s, %d, -%s\n", Regs[reg_no1].name, op1->u.value1, Regs[getReg(op2, fp)].name);
				else
				sprintf(str, "  addi %s, %s, -%d\n", Regs[reg_no1].name, Regs[getReg(op1, fp)].name, op2->u.value1);
				fputs(str, fp);
			}
		}
		else if(ir->kind == MUL_){
			if(op2->kind!=CONSTANT&&op1->kind!=CONSTANT){
				int reg_no2 = getReg(op1, fp);
				int reg_no3 = getReg(op2, fp);
				sprintf(str, "  mul %s, %s, %s\n", Regs[reg_no1].name, Regs[reg_no2].name, Regs[reg_no3].name);
				fputs(str, fp);
			}
			else{
				if(op2->kind==CONSTANT&&op1->kind==CONSTANT)
				sprintf(str, "  mul %s, %d, %d\n", Regs[reg_no1].name, op1->u.value1, op2->u.value1);
				else if(op1->kind==CONSTANT)
				sprintf(str, "  mul %s, %d, %s\n", Regs[reg_no1].name, op1->u.value1, Regs[getReg(op2, fp)].name);
				else
				sprintf(str, "  mul %s, %s, %d\n", Regs[reg_no1].name, Regs[getReg(op1, fp)].name, op2->u.value1);
				fputs(str, fp);
			}
		}
		else if(ir->kind == DIV_){
			if(op2->kind!=CONSTANT&&op1->kind!=CONSTANT){
				int reg_no2 = getReg(op1, fp);
				int reg_no3 = getReg(op2, fp);
				sprintf(str, "  div %s, %s\n",  Regs[reg_no2].name, Regs[reg_no3].name);
				fputs(str, fp);
			}
			else{
				if(op2->kind==CONSTANT&&op1->kind==CONSTANT)
				sprintf(str, "  div %d, %d\n",  op1->u.value1, op2->u.value1);
				else if(op1->kind==CONSTANT)
				sprintf(str, "  div %d, %s\n",  op1->u.value1, Regs[getReg(op2, fp)].name);
				else
				sprintf(str, "  div %s, %d\n", Regs[getReg(op1, fp)].name, op2->u.value1);
				fputs(str, fp);
			}
			memset(str, 0, sizeof(str));
			sprintf(str, "  mflo %s\n",Regs[reg_no1].name);
			fputs(str, fp);
		}
	}
	else if(ir->kind == RETURN_){
		Operand op=ir->u.other.le;
		char str[32];
		memset(str, 0, sizeof(str));
		if(op->kind==CONSTANT){
			sprintf(str, "  move $v0, $%d\n", op->u.value1);
			fputs(str, fp);
		}
		else{
			int reg_no = getReg(op, fp);
			sprintf(str, "  move $v0, %s\n", Regs[reg_no].name);
			fputs(str, fp);
		}
		fputs("  jr $ra\n", fp);
		infunction=0;
	}
	else if(ir->kind == LABEL_){//printf("%d\n",ir->kind);
		Operand label=ir->u.ifgo.label;
		char str[32];
		memset(str, 0, sizeof(str));
		sprintf(str, "label%d:\n", label->u.var_no);
		fputs(str, fp);
	}
	else if(ir->kind == IFGOTO_){//printf("%d\n",ir->kind);
		printf("arrive\n");
		int reg_no1 = getReg(ir->u.ifgo.op1, fp);
		int reg_no2;
		printf("arrive\n");
		Operand relop=ir->u.ifgo.relop;
		Operand label=ir->u.ifgo.label;
		char rels[4];
		memset(rels, 0, sizeof(rels));
		if(strcmp(relop->u.name,"==")==0){
			strcpy(rels, "beq");
		}
		else if(strcmp(relop->u.name,"!=")==0){
			strcpy(rels, "bne");
		}
		else if(strcmp(relop->u.name,">")==0){
			strcpy(rels, "bgt");
		}
		else if(strcmp(relop->u.name,"<")==0){
			strcpy(rels, "blt");
		}
		else if(strcmp(relop->u.name,">=")==0){
			strcpy(rels, "bge");
		}
		else if(strcmp(relop->u.name,"<=")==0){
			strcpy(rels, "ble");
		}
		char str[32];printf("check\n");
		memset(str, 0, sizeof(str));
		if(ir->u.ifgo.op2->kind!=CONSTANT){
			reg_no2 = getReg(ir->u.ifgo.op2, fp);
			sprintf(str, "  %s %s, %s, label%d\n", rels, Regs[reg_no1].name, Regs[reg_no2].name, label->u.var_no);
		fputs(str, fp);
		}
		else{
			reg_no2=ir->u.ifgo.op2->u.value1;
			sprintf(str, "  %s %s, %d, label%d\n", rels, Regs[reg_no1].name, reg_no2, label->u.var_no);
		fputs(str, fp);
		}
	}
	else if(ir->kind == IF_){
		
	}
	else if(ir->kind == GOTO_){//printf("%d\n",ir->kind);
		Operand op = ir->u.other.le;
		char str[32];
		memset(str, 0, sizeof(str));
		sprintf(str, "  j label%d\n", op->u.var_no);
		fputs(str, fp);
	}
	else if(ir->kind == READ_){printf("%d\n",ir->kind);
		fputs("  addi $sp, $sp, -4\n", fp);
		fputs("  sw $ra, 0($sp)\n", fp);
		fputs("  jal read\n", fp);
		int reg_no = getReg(ir->u.other.le, fp);
		fputs("  lw $ra, 0($sp)\n", fp);
		fputs("  addi $sp, $sp, 4\n", fp);
		char str[32];
		memset(str, 0, sizeof(str));
		sprintf(str, "  move %s, $v0\n", Regs[reg_no].name);
		fputs(str, fp);
	}
	else if(ir->kind == WRITE_){//printf("12345678910\n");
		int reg_no = getReg(ir->u.other.ri, fp);
		char str[32];
		memset(str, 0, sizeof(str));
		printf("%s\n",Regs[reg_no].name);
		sprintf(str, "  move $a0, %s\n", Regs[reg_no].name);
		fputs(str, fp);
		fputs("  addi $sp, $sp, -4\n", fp);
		fputs("  sw $ra, 0($sp)\n", fp);
		fputs("  jal write\n", fp);
		fputs("  lw $ra, 0($sp)\n", fp);
		fputs("  addi $sp, $sp, 4\n", fp);
	}
	else if(ir->kind == CALL_){printf("CALL\n");
		char str[32];
		memset(str, 0, sizeof(str));
		int off = -4 * arg - 4;
		sprintf(str, "  addi $sp, $sp, %d\n", off);
		fputs(str, fp);
		fputs("  sw $ra, 0($sp)\n", fp);
		Operand op;
		if (arg == 1) {printf("arg1\n");
			storeAllVar(fp);
			fputs("  sw $a0, 4($sp)\n", fp);
			InterCode pre1 = ir->last;
			op=pre1->u.other.le;
			int reg_no = getReg(pre1->u.other.le, fp);
			memset(str, 0, sizeof(str));
			sprintf(str, "  move $a0, %s\n", Regs[reg_no].name);
			fputs(str, fp);
			memset(str, 0, sizeof(str));
			sprintf(str, "  jal %s\n", ir->u.other.ri->u.name);
			fputs(str, fp);
			fputs("  lw $a0, 4($sp)\n", fp);
			fputs("  lw $ra, 0($sp)\n", fp);
			fputs("  addi $sp, $sp, 8\n", fp);
			loadAllVar(fp);
		}
		else if (arg == 2) {
			storeAllVar(fp);
			fputs("  sw $a0, 4($sp)\n", fp);
			fputs("  sw $a1, 8($sp)\n", fp);
			InterCode pre1 = ir->last;
			int reg_no1 = getReg(pre1->u.other.le, fp);
			char str[32];
			memset(str, 0, sizeof(str));
			sprintf(str, "  move $a0, %s\n", Regs[reg_no1].name);
			fputs(str, fp);

			InterCode pre2 = pre1->last;
			int reg_no2 = getReg(pre2->u.other.le, fp);
			memset(str, 0, sizeof(str));
			sprintf(str, "  move $a1, %s\n", Regs[reg_no2].name);
			fputs(str, fp);

			memset(str, 0, sizeof(str));
			sprintf(str, "  jal %s\n", ir->u.other.ri->u.name);
			fputs(str, fp);
			fputs("  lw $a1, 8($sp)\n", fp);
			fputs("  lw $a0, 4($sp)\n", fp);
			fputs("  lw $ra, 0($sp)\n", fp);
			fputs("  addi $sp, $sp, 12\n", fp);
			loadAllVar(fp);
		}
		else if (arg == 3) {
			storeAllVar(fp);
			fputs("  sw $a0, 4($sp)\n", fp);
			fputs("  sw $a1, 8($sp)\n", fp);
			fputs("  sw $a2, 12($sp)\n", fp);

			InterCode pre1 = ir->last;
			int reg_no1 = getReg(pre1->u.other.le, fp);
			char str[32];
			memset(str, 0, sizeof(str));
			sprintf(str, "  move, $a0 %s\n", Regs[reg_no1].name);
			fputs(str, fp);

			InterCode pre2 = pre1->last;
			int reg_no2 = getReg(pre2->u.other.le, fp);
			memset(str, 0, sizeof(str));
			sprintf(str, "  move, $a1 %s\n", Regs[reg_no2].name);
			fputs(str, fp);

			InterCode pre3 = pre2->last;
			int reg_no3 = getReg(pre3->u.other.le, fp);
			memset(str, 0, sizeof(str));
			sprintf(str, "  move, $a2 %s\n", Regs[reg_no3].name);
			fputs(str, fp);

			memset(str, 0, sizeof(str));
			sprintf(str, "  jal %s\n", ir->u.other.ri->u.name);
			fputs(str, fp);

			fputs("  lw $a2, 12($sp)\n", fp);
			fputs("  lw $a1, 8($sp)\n", fp);
			fputs("  lw $a0, 4($sp)\n", fp);
			fputs("  lw $ra, 0($sp)\n", fp);
			fputs("  addi $sp, $sp, 16\n", fp);
			loadAllVar(fp);
		}
		else if (arg == 4) {
			storeAllVar(fp);
			fputs("  sw $a0, 4($sp)\n", fp);
			fputs("  sw $a1, 8($sp)\n", fp);
			fputs("  sw $a2, 12($sp)\n", fp);
			fputs("  sw $a3, 16($sp)\n", fp);
			InterCode pre1 = ir->last;
			int reg_no1 = getReg(pre1->u.other.le, fp);
			char str[32];
			memset(str, 0, sizeof(str));
			sprintf(str, "  move $a0, %s\n", Regs[reg_no1].name);
			fputs(str, fp);

			InterCode pre2 = pre1->last;
			int reg_no2 = getReg(pre2->u.other.le, fp);
			memset(str, 0, sizeof(str));
			sprintf(str, "  move $a1, %s\n", Regs[reg_no2].name);
			fputs(str, fp);

			InterCode pre3 = pre2->last;
			int reg_no3 = getReg(pre3->u.other.le, fp);
			memset(str, 0, sizeof(str));
			sprintf(str, "  move $a2, %s\n", Regs[reg_no3].name);
			fputs(str, fp);

			InterCode pre4 = pre3->last;
			int reg_no4 = getReg(pre4->u.other.le, fp);
			memset(str, 0, sizeof(str));
			sprintf(str, "  move $a3 %s\n", Regs[reg_no4].name);
			fputs(str, fp);

			memset(str, 0, sizeof(str));
			sprintf(str, "  jal %s\n", ir->u.other.ri->u.name);
			fputs(str, fp);

			fputs("  lw $a3, 16($sp)\n", fp);
			fputs("  lw $a2, 12($sp)\n", fp);
			fputs("  lw $a1, 8($sp)\n", fp);
			fputs("  lw $a0, 4($sp)\n", fp);
			fputs("  lw $ra, 0($sp)\n", fp);
			fputs("  addi $sp, $sp, 20\n", fp);
			loadAllVar(fp);
		}
		arg = 0;
		int reg_no = getReg(ir->u.other.le, fp);
		memset(str, 0, sizeof(str));
		sprintf(str, "  move %s, $v0\n", Regs[reg_no].name);
		fputs(str, fp);
	}
	else if(ir->kind == ARG_){//printf("???\n");
		Operand op = ir->u.other.le;
		while(op!=NULL){
			if(op->kind!=CONSTANT){
				getReg(op, fp);
			}
			else{
				
			}
			arg++;
			op=op->next;
		}
	}
	else if(ir->kind == FUNC_){//printf("func\n");
		char str[32];
		memset(str, 0, sizeof(str));
		Operand op = ir->u.other.le;
		sprintf(str, "\n%s:\n", op->u.name);
		fputs(str, fp);
		VarDescipter *var = List;
		int s,e=25;
		while (var != NULL) {
			List = var->next;
			free(var);
			var = List;
		}

		for (s=9;s<=e;s++){
			Regs[s].old = 0;
			Regs[s].var = NULL;
		}
	}
	else if(ir->kind == PARAM_){//printf("param\n");
		int i=0;
		infunction=1;
		char str[32];
		Operand op=ir->u.other.le;
		while (op!=NULL) {
			anow++;
			int reg_no = getReg(op, fp);
			printf("159753\n");
			op=op->next;
		}
		anow=-1;
	}
	else if(ir->kind == DEC_){
		
	}
	else if(ir->kind == ARR_){
		
	}
}
void storeAllVar(FILE *fp){
	VarDescipter *var = List;
	while (var != NULL) {
		if (var->reg_no >= 0) {
			Stack.varstack[Stack.length] = var;

			Regs[var->reg_no].var = NULL;
			Stack.old[Stack.length] = Regs[var->reg_no].old;
			Regs[var->reg_no].old = 0;
			Stack.length++;
		}
		var = var->next;
	}
	List = NULL;
}
void loadAllVar(FILE *fp) {
	VarDescipter *var = List;
	int i;
	while (var != NULL) {
		List = var->next;
		free(var);
		var = List;
	}
	for (i = Stack.length - 1; i >= 0; i--) {
		VarDescipter* var = Stack.varstack[i];
		if (var == NULL) {
			continue;
		}
		var->next = List;
		List = var;
		Regs[var->reg_no].var = var;
		Regs[var->reg_no].old = Stack.old[i];
	}
	Stack.length = 0;
}
int getReg(Operand op, FILE *fp){
	char name[4];
	memset(name, 0, sizeof(name));
	VarDescipter* var = List;
	VarDescipter* temp = List;
	VarDescipter* last;
	if(anow!=-1&&infunction==1){printf("define:$a\n");
		if (var == NULL){
			var = (VarDescipter*)malloc(sizeof(VarDescipter));
			var->op = op;
			var->next=NULL;
			var->reg_no=4+anow;
			Regs[4+anow].var = var;
			List=var;
			return var->reg_no;
		}
		else{
			while(var!=NULL){
				last=var;
				var=var->next;
			}
			var = (VarDescipter*)malloc(sizeof(VarDescipter));
			var->op = op;
			var->next=NULL;
			var->reg_no=4+anow;
			last->next=var;
			Regs[4+anow].var = var;
			return var->reg_no;
		}
	}
	if (var == NULL){
		var = (VarDescipter*)malloc(sizeof(VarDescipter));
		var->op = op;
		var->reg_no = -1;
		var->next=NULL;
		List=var;
	}
	else {
		/*while(temp!=NULL){
		if (op->kind == TEMPVAR){
			printf("%d:%d\n",op->u.var_no,var->op->u.var_no);
		}
		else if (op->kind == VARIABLE){
			printf("%s:%s\n",op->u.name,var->op->u.name);
		}
		temp=temp->next;
		}*/
		while (var != NULL){
		
		if (op->kind == TEMPVAR&&var->op->u.var_no==op->u.var_no){
			
			return var->reg_no;
		}
		else if (op->kind == VARIABLE&&strcmp(var->op->u.name,op->u.name) == 0){
			printf("159753");
			return var->reg_no;
		}
		last=var;
		var = var->next;
		}
		if (var == NULL){
		var = (VarDescipter*)malloc(sizeof(VarDescipter));
		var->op = op;
		var->reg_no = -1;
		var->next=NULL;
		last->next=var;
		}
	}
	if (var->reg_no == -1){
		int reg_no = -1,s=8,e=25;
		for(;s<=e;s++){
			if (Regs[s].var != NULL){
				Regs[s].old++;
			}
		}
		for(s=8;s<=e;s++){
			if (Regs[s].var == NULL) {
				reg_no = s;
				break;
			}
		}
		if (reg_no == -1) {
			int maxOld = -100;
			for(s=8;s<=e;s++){
				if(Regs[s].var->op->kind==TEMPVAR)
				{
					if (Regs[s].old > maxOld) {
						maxOld = Regs[s].old;
						reg_no = s;
					}
				}
			}
		}
		if (reg_no == -1) {
			int maxOld = -100;
			for(s=8;s<=e;s++){
				if (Regs[s].old > maxOld) {
					maxOld = Regs[s].old;
					reg_no = s;
				}
			}
			char str[32];
			memset(str, 0, sizeof(str));
			sprintf(str, "  move %s, %s:\n",Regs[reg_no].var->op->u.name,Regs[reg_no].name);
			fputs(str, fp);
		}
		var->reg_no = reg_no;
		Regs[reg_no].var = var;
	}
	Regs[var->reg_no].old = 0;
	return var->reg_no;
}
