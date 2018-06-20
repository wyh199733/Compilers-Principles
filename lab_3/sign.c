#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sign.h"
struct sign* create_sign(char type_[],int line_)
{
	struct sign* sign_=(struct sign*)malloc(sizeof(struct sign));
	sign_->line=line_;
	sign_->flag=0;
	sign_->token=1;
	strcpy(sign_->type,type_);
	sign_->root=NULL;
	sign_->leaf=NULL;
	sign_->brother=NULL;
	return sign_;
}
int brother(struct sign* brother1,struct sign* brother2)
{
	brother1->brother=brother2;
	brother2->root=brother1->root;
	return 0;
}
int links(struct sign* root,struct sign* leaf)
{
	leaf->root=root;
	root->leaf=leaf;
	root->token=0;
	return 0;
}
int print(struct sign* root)
{
	
	int i=0,j=1;
	printf("%s (%d)\n",root->type,root->line);
	struct sign* now=root->leaf;
	while(now!=root){
	for(i=0;i<j;i++){
	printf("  ");
	}
	if(now->token){
		if(now->flag)printf("%s:%s\n",now->type,now->text);
		else printf("%s\n",now->type);
	}
	else printf("%s (%d)\n",now->type,now->line);
	if(now->leaf!=NULL){now=now->leaf;j++;}
	else if(now->brother!=NULL)now=now->brother;
	else {
		while(now->brother==NULL&&now->root!=NULL){now=now->root;j--;}
		if(now!=root)now=now->brother;
	}
	}
	return 1;
}
