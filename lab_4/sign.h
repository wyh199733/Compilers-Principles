#ifndef SIGN_H
#define SIGN_H
struct sign{
		int line;
		int column;
		int flag;
		int token;
		char type[32];
		char text[32];
		struct sign* root;
		struct sign* leaf;
		struct sign* brother;
};
struct sign* create_sign(char type_[],int line_);
int brother(struct sign* brother1,struct sign* brother2);
int links(struct sign* root,struct sign* leaf);
int print(struct sign* root);

#endif
