#include "parser1.c"

#include "st.h"

#include <ctype.h>

#include <string.h>

#define SIZE 97


int hogaya = 0;

int offset = 0;

int sno;

struct STScope* getParentModuleScope(struct STScope *scope)
{
	if(scope ==NULL)
	{
		return NULL;
	}
	if(strcmp(scope->lexeme , "1DRIVER_MODULE")==0)
	{
		return scope;
	}
	if(scope->isFunction == 1)
	{
		return scope;
	}

	return scope->parent;
}

void printSymbolTable(struct STScope *scope)
{
	int i=0;
	for(i=0;i<SIZE;i++)
	{
		struct STNode *node = scope[i].head;
		if(node!=NULL)
		{
			while(node!=NULL)
			{
				if(node->isArray==0)
				{
					printf("%d %s %s %s %d %d %d %d \n",sno++,node->lexeme,node->type,getParentModuleScope(scope)->lexeme,scope->start_lno,scope->end_lno,node->width,node->offset);
				}
				else
				{
					printf("%d %s array (%d ,%s) %s %d %d %d %d \n",sno++,node->lexeme,node->no_of_elements,node->type,getParentModuleScope(scope)->lexeme,scope->start_lno,scope->end_lno,node->width,node->offset);
				}
				node = node -> next;
			}

		}	
	}
	
}
void printInOutTable()
{
	int i=0;
	for(i=0;i<SIZE;i++)
	{
		struct node *inp = in_table[i].head;
		while(inp!=NULL)
		{	
			struct node *node = inp->next_lexeme;
			while(node!=NULL)
			{
				if(node->isArray == 0)
				{
					printf("%d %s %s %s %d %d %d %d\n",sno++,node->lexeme,node->type,inp->lexeme,node->scope->start_lno,node->scope->end_lno,node->width,node->offset);
				}
				else
				{
					printf("%d %s arrar (%d,%s) %s %d %d %d %d\n",sno++,node->lexeme,node->no_of_elements,node->type,inp->lexeme,node->scope->start_lno,node->scope->end_lno,node->width,node->offset);
				}

				node = node -> next_lexeme;
			}	

			inp = inp -> next;	
		}
	}
	
}
void printSymbolTable1(struct STScope *scope)
{
	//printSymbolTable(scope);

	struct STScope *scope_child = scope->child_scope;

	while(scope_child!=NULL)
	{
		printSymbolTable(scope_child);
		printSymbolTable1(scope_child);
		scope_child = scope_child -> next_scope; 
	}



}

int outputAssinged(struct tree_unit *AST)
{
	if(AST==NULL)
		return 0;

	else
	{
		struct node *temp = out_table[hash_gen(AST->lexeme)].head;
		if(temp==NULL)
		{

		}
		else
		{
			while(temp!=NULL)
			{
				if(strcmp(temp->lexeme,AST->lexeme)==0)
				{
					break;
				}
				temp = temp -> next ;
			}
			if(temp!=NULL)
				temp = temp->next_lexeme;
		
			while(temp!=NULL)
			{
				if(temp->isassigned!=1)
					return 0;
				temp = temp->next_lexeme;
			}
		}
	}
	return 1;
}

// Return the node corresponding to the input or output list of the function
struct node* getNodeFunc(struct tree_unit *AST, struct STScope *scope)
{
	while(scope!=NULL)
	{
		if(scope->isFunction==1)
		{
			break;
		}
		scope = scope->parent;
	}

	if(scope == NULL)
	{
		return NULL;
	}

	if(func_par_error_check(AST,scope) || out_func_par_error_check(AST,scope))
	{
		struct STScope *parent_scope = scope;

		if(scope!=NULL && scope->isFunction==1)
		{
			struct node *temp = in_table[hash_gen(parent_scope->lexeme)].head;

			if(temp!=NULL)
			{
				while(temp!=NULL)
				{
					if(strcmp(temp->lexeme,scope->lexeme)==0)
					{
						break;
					}
					temp=temp->next;
				}
			}

			if(temp!=NULL)
			{
				temp = temp->next_lexeme;
				while(temp!=NULL)
				{
					if(strcmp(temp->lexeme,AST->lexeme)==0)
					{
						return temp;
					}
					temp = temp->next_lexeme;
				}
			}
		}

		if(parent_scope !=NULL && parent_scope->isFunction==1)
		{
			struct node *temp = out_table[hash_gen(parent_scope->lexeme)].head;

			if(temp!=NULL)
			{
				while(temp!=NULL)
				{
					if(strcmp(temp->lexeme,parent_scope->lexeme)==0)
					{
						break;
					}
					temp=temp->next;
				}
			}

			if(temp!=NULL)
			{
				temp = temp->next_lexeme;
				while(temp!=NULL)
				{
					if(strcmp(temp->lexeme,AST->lexeme)==0)
					{
						return temp;
					}
					temp = temp->next_lexeme;
				}
			}
		}
	}
	return NULL;
}
int checkAnyTermBoolean2(struct tree_unit *AST)
{
	if(AST->AST_child->AST_next!=NULL)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
int checkAnyTermBoolean(struct tree_unit *AST,struct STScope *scope)
{
	struct tree_unit *temp = AST->AST_child; // corresponds to the first arithmetic expression in the list of arithmetic expressions

	while(temp!=NULL)
	{
		if(temp->AST_child->AST_child->AST_child!=NULL) // jugaad here as var pointing to empty and still added to the AST
			check_arithmetic(temp,ID_type(temp->AST_child->AST_child->AST_child,scope),scope,1);
		
		if(temp->AST_next!=NULL)
		{
			temp = temp->AST_next->AST_next;
		}
		else 
		{
			temp = temp->AST_next;
		}
	}
	if(AST->AST_child->child->val==N8)
		return 1;
	else
		return 0; // would not return 0 if this become true of falso 
}
void check_boolean(struct tree_unit *AST,struct STScope *scope)
{
	struct tree_unit *temp = AST->AST_child;

	while(temp!=NULL)
	{
		if(checkAnyTermBoolean(temp,scope)==0)
		{
			printf("Line no ---- Error:- Boolean expression error\n");
			return ;
		}
		if(temp->AST_next!=NULL)
		{
			temp = temp->AST_next->AST_next;
		}
		else
		{
			temp = temp->AST_next;
		}
		
			
	}

}
char* upper_string(char s[]) 
{
   int c = 0;
 
   while (s[c] != '\0') 
   {
      if (s[c] >= 'a' && s[c] <= 'z') 
      {
         s[c] = s[c] - 32;
      }
      c++;
   }

   return s;
}
struct node* findLexemeNodeFuncGoToParent(struct tree_unit *AST, struct STScope *scope)
{
	while(scope!=NULL)
	{
		if(scope->isFunction==0)
		{
			scope = scope->parent;
			continue;
		}
		struct node *temp = in_table[hash_gen(scope->lexeme)].head;

		if(temp!=NULL)
		{
			while(temp!=NULL)
			{
				if(strcmp(temp->lexeme,scope->lexeme)==0)
				{
					//return temp;
					break;
				}
				temp=temp->next;
			}
		}

		if(temp!=NULL)
		{
			temp = temp->next_lexeme;
			while(temp!=NULL)
			{
				if(strcmp(temp->lexeme,AST->lexeme)==0)
				{
					return temp;
				}
				temp=temp->next_lexeme;
			}	
		}
		temp = out_table[hash_gen(scope->lexeme)].head;

		if(temp!=NULL)
		{
			while(temp!=NULL)
			{
				if(strcmp(temp->lexeme,scope->lexeme)==0)
				{
					//return temp;
					break;
				}
				temp=temp->next;
			}
		}
		if(temp!=NULL)
		{
			temp = temp->next_lexeme;
			while(temp!=NULL)
			{
				if(strcmp(temp->lexeme,AST->lexeme)==0)
				{
					return temp;
				}
				temp=temp->next_lexeme;
			}	
		}



		scope=scope->parent;

	}
	return NULL;
}
struct node* findLexemeNodeFunc(struct tree_unit *AST, struct STScope *scope)
{
	struct node *temp = in_table[hash_gen(scope->lexeme)].head;

		if(temp!=NULL)
		{
			while(temp!=NULL)
			{
				if(strcmp(temp->lexeme,scope->lexeme)==0)
				{
					//return temp;
					break;
				}
				temp=temp->next;
			}
		}

	if(temp!=NULL)
	{
		temp = temp->next_lexeme;
		while(temp!=NULL)
		{
			if(strcmp(temp->lexeme,AST->lexeme)==0)
			{
				return temp;
			}
			temp=temp->next;
		}	
	}

	temp = out_table[hash_gen(scope->lexeme)].head;

	if(temp!=NULL)
	{
		while(temp!=NULL)
		{
			if(strcmp(temp->lexeme,scope->lexeme)==0)
			{
				//return temp;
				break;
			}
			temp=temp->next;
		}
	}
	if(temp!=NULL)
	{
		temp = temp->next_lexeme;
		while(temp!=NULL)
		{
			if(strcmp(temp->lexeme,AST->lexeme)==0)
			{
				return temp;
			}
			temp=temp->next;
		}	
	}
	return NULL;
}
int func_par_error_check(struct tree_unit *AST,struct STScope *scope)
{
	struct STScope *parent_scope = scope;

	if(parent_scope!=NULL && parent_scope->isFunction==1)
	{
		struct node *temp = in_table[hash_gen(parent_scope->lexeme)].head;

		if(temp!=NULL)
		{
			while(temp!=NULL)
			{
				if(strcmp(temp->lexeme,parent_scope->lexeme)==0)
				{
					break;
				}
				temp=temp->next;
			}
		}

		if(temp!=NULL)
		{
			temp = temp->next_lexeme;
			while(temp!=NULL)
			{
				if(strcmp(temp->lexeme,AST->lexeme)==0)
				{
					return 1;
				}
				temp = temp->next_lexeme;
			}
		}
	}
	return 0;
}
int out_func_par_error_check(struct tree_unit *AST,struct STScope *scope)
{
	struct STScope *parent_scope = scope;

	if(parent_scope!=NULL && parent_scope->isFunction==1)
	{
		struct node *temp = out_table[hash_gen(parent_scope->lexeme)].head;

		if(temp!=NULL)
		{
			while(temp!=NULL)
			{
				if(strcmp(temp->lexeme,parent_scope->lexeme)==0)
				{
					break;
				}
				temp=temp->next;
			}
		}

		if(temp!=NULL)
		{
			temp = temp->next_lexeme;
			while(temp!=NULL)
			{
				if(strcmp(temp->lexeme,AST->lexeme)==0)
				{
					return 1;
				}
				temp = temp->next_lexeme;
			}
		}
	}
	return 0;
}
int hash_gen(char *str)
{

	//if(str==NULL)
	//	return 0;
    int hash_val=0,i;
    for(i=0;str[i]!='\0';i++)
    {
        hash_val+=str[i];
    }

    return (hash_val% SIZE);

}

int error_module_flag = 0;

// find Lexeme also looks in the function's input and output list
int findLexeme(struct tree_unit *AST,struct STScope *scope,int goToParent)
	{

	/* Uncomment the while loop if variable cannot be declared when present in the global scope*/
	if(goToParent==1)
	{
		while(scope!=NULL)
		{
			if(scope->isFunction==1)
			{
				if(func_par_error_check(AST,scope) || out_func_par_error_check(AST,scope))
				{
					return 1;
				}
			}
			struct STNode *temp = scope[hash_gen(AST->lexeme)].head;
			if(temp==NULL)
			{

			}
			else
			{
				while(temp!=NULL)
				{
					if(strcmp(temp->lexeme,AST->lexeme)==0)
					{
						return 1;
					}
					temp=temp->next;
				}
			}
			scope=scope->parent;
		}
	}
	else
	{
		struct STNode *temp = scope[hash_gen(AST->lexeme)].head;
		if(temp==NULL)
		{
			return 0;
		}
		else
		{
			while(temp!=NULL)
			{
				if(strcmp(temp->lexeme,AST->lexeme)==0)
				{
					return 1;
				}
				temp=temp->next;
			}
		}
	}
	return 0;
}

struct STNode* findLexemeNodeFromLexeme(char *lexeme,struct STScope *scope,int goToParent)
{
	if(goToParent==1)
	{
	/* Uncomment the while loop if variable cannot be declared when present in the global scope*/
		while(scope!=NULL)
		{
			struct STNode *temp = scope[hash_gen(lexeme)].head;
			if(temp==NULL)
			{

			}
			else
			{
				while(temp!=NULL)
				{
					if(strcmp(temp->lexeme,lexeme)==0)
					{
						return temp;
					}
					temp=temp->next;
				}
			}
			scope=scope->parent;
		}
	}
	else
	{
		struct STNode *temp = scope[hash_gen(lexeme)].head;
		if(temp==NULL)
		{

		}
		else
		{
			while(temp!=NULL)
			{
				if(strcmp(temp->lexeme,lexeme)==0)
				{
					return temp;
				}
				temp=temp->next;
			}
		}
	}
	return NULL;
}

struct STNode* findLexemeNode(struct tree_unit *AST,struct STScope *scope,int goToParent)
{
	if(goToParent==1)
	{
	/* Uncomment the while loop if variable cannot be declared when present in the global scope*/
		while(scope!=NULL)
		{
			struct STNode *temp = scope[hash_gen(AST->lexeme)].head;
			if(temp==NULL)
			{

			}
			else
			{
				while(temp!=NULL)
				{
					if(strcmp(temp->lexeme,AST->lexeme)==0)
					{
						return temp;
					}
					temp=temp->next;
				}
			}
			scope=scope->parent;
		}
	}
	else
	{
		struct STNode *temp = scope[hash_gen(AST->lexeme)].head;
		if(temp==NULL)
		{

		}
		else
		{
			while(temp!=NULL)
			{
				if(strcmp(temp->lexeme,AST->lexeme)==0)
				{
					return temp;
				}
				temp=temp->next;
			}
		}	
	}
	return NULL;
}

char* ID_type(struct tree_unit *AST,struct STScope *scope)
{
 
	struct STNode *x = findLexemeNode(AST,scope,1);
	char *type="";
	if(AST->val==ID)
	{

		if(findLexeme(AST,scope,1) == 0)
		{
			// if(findLexemeNodeFuncGoToParent(AST,scope)!=NULL)
			// {
			// 	return findLexemeNodeFuncGoToParent(AST,scope)->type;
			// }
			printf("Line no %d Error:-The variable %s not declared earlier\n",AST->lno,AST->lexeme);
			return "";
		}
		else if(x==NULL)
		{
			struct node *y = getNodeFunc(AST,scope);
			if(y==NULL)
				return "";
			else 
				return y->type;
		}
		else
		{
			return x->type;
		}
	}
	else if(AST->val == NUM)
	{
		return "INTEGER";
	}
	else if(AST->val == RNUM)
	{
		return "REAL";
	}

	if (strcmp(type,"array INTEGER") ==0)
		return "INTEGER";
	if (strcmp(type,"array BOOLEAN") ==0)
		return "BOOLEAN";
	if (strcmp(type,"array REAL") ==0)
		return "REAL";

	return type;
}


void printTable(struct node_list *table)
{
	int i=0;

	for(i=0;i<SIZE;i++)
	{
		if(table[i].head!=NULL)
		{
			struct node *temp = table[i].head;
			printf("slot %d ",i);
			while(temp!=NULL)
			{
				printf("%s %s - ",temp->lexeme,temp->type);
				temp=temp->next;	
			}
			printf("\n");
		}
	}
}
void check_arithmetic(struct tree_unit *AST,char *type_left,struct STScope *scope,int typecheck)
{
	//Didn't account for arrays as of now 
	int skip_next=0;
	if(AST==NULL)
	{
		return;
	}
	else if(!typecheck)
	{
		if(AST->val==ID)
			ID_type(AST,scope);
	}
	else
	{
		
		if(AST->val==ID)
		{
			if(AST->AST_next == NULL && AST->AST_parent->val==var && AST->AST_parent->AST_child!=AST)
			{
				return;
			}
			char type_right[100];
			strcpy(type_right,ID_type(AST,scope));

			struct STNode *st_temp = findLexemeNode(AST,scope,1);



			if(st_temp!=NULL && findLexeme(AST,scope,1)!= 0) 
			{
				if(strcmp(type_left,"")==0)
				{

				}

				else if(strcmp(type_left,type_right)!=0)
				{
					printf("Line no %d Error =: There is a type mismatch <%s,%s>, Type should be %s\n",AST->lno,AST->lexeme,type_right,type_left);
					error_flag=1;
				}
			}
			if(st_temp!=NULL && findLexeme(AST,scope,1)!= 0)
			{
				if (st_temp->isArray==1)
				{
					if(AST->AST_next==NULL)
					{
						printf("Line no %d Error:= Array can't be used as such \n",AST->lno);	
						error_flag = 1;
					}
					else if(AST->AST_next->val==ID) // in one of the test cases even array is written only as name eg p ;
					{
						if(strcmp(ID_type(AST->AST_next,scope),"INTEGER")!=0)
						{
							printf("Line no %d Error =: Index of array must be integer \n",AST->lno);
							error_flag=1;
						}
						skip_next = 1;
					}
				}
			}
			else if(st_temp == NULL && findLexeme(AST,scope,1) == 1)
			{
				struct node *y = getNodeFunc(AST,scope);

				if(strcmp(type_left,"")==0)
				{

				}

				else if(strcmp(type_left,type_right)!=0)
				{
					printf("Line no %d Error =: There is a type mismatch <%s,%s>, Type should be %s\n",AST->lno,AST->lexeme,type_right,type_left);
					error_flag=1;
				}

				else if(y!=NULL)
				{
					if (y->isArray==1)
					{
						if(AST->AST_next==NULL)
						{
							printf("Line no %d Error:= Array can't be used as such \n",AST->lno);
							error_flag = 1;
						}
						if(AST->AST_next->val==ID) // in one of the test cases even array is written only as name eg p ;
						{
							if(strcmp(ID_type(AST->AST_next,scope),"INTEGER")!=0)
							{
								printf("Line no %d Error =: Index of array must be integer \n",AST->lno);
								error_flag=1;
							}
							skip_next = 1;
						}
					}
				}
			}
			else
			{
				//printf("Line No %d Error:- The variable %s is not declared\n",AST->lno,AST->lexeme);
				
				// If variable is not declared how to know if its an array
				if(AST->next->val == whichId)
				{
					skip_next = 1; 
				}
			}
		}
		else if(AST->val==NUM)
		{
			char type_right[100];
			strcpy(type_right,"INTEGER");

			if(strcmp(type_left,"")==0)
			{

			}

			else if(strcmp(type_left,type_right)!=0)
			{
				printf("Line no %d Error =: There is a type mismatch <%s,%s>, Type should be %s\n",AST->lno,AST->lexeme,type_right,type_left);
				error_flag=1;
			}

		}
		else if(AST->val==RNUM)
		{
			char type_right[100];
			strcpy(type_right,"REAL");

			if(strcmp(type_left,"")==0)
			{

			}

			else if(strcmp(type_left,type_right)!=0)
			{
				printf("Line no %d Error =: There is a type mismatch <%s,%s>, Type should be %s\n",AST->lno,AST->lexeme,type_right,type_left);
				error_flag=1;
			}
		}
	}
	// if(AST->AST_child!=NULL)
	// {
	// 	if(strcmp(AST->AST_child->lexeme ,"index")==0)
	// 	{
	// 		printf("----- here -----\n");
	// 		printf("%d %d %s %s\n",AST->AST_child->lno, AST->val,getString(AST->val),AST->lexeme);
	// 	}
	// }
	check_arithmetic(AST->AST_child,type_left,scope,typecheck);
	
	if(skip_next==1)
	{
		// if(AST->AST_next!=NULL)
		// {
		// 	if(AST->AST_next->AST_next!=NULL)
		// 	{
		// 		if(strcmp(AST->AST_next->AST_next->lexeme,"index")==0)
		// 		{
		// 			printf("----- hereTHere -----\n");
		// 			printf("%d %d %s %s\n",AST->AST_next->AST_next->lno, AST->val,getString(AST->val),AST->lexeme);
		// 		}
		// 	}
		// }
		if(AST->AST_next!=NULL)
			check_arithmetic(AST->AST_next->AST_next,type_left,scope,typecheck);
	}
	else
	{
		// if(AST->AST_next!=NULL)
		// {
		// 	if(strcmp(AST->AST_next->lexeme,"index")==0)
		// 	{
		// 		printf("----- here -----\n");
		// 		printf("%d %d %s\n",AST->lno, AST->val,AST->lexeme);
		// 	}
		// }
		check_arithmetic(AST->AST_next,type_left,scope,typecheck);
	}
}
char* getIdNodeType(struct tree_unit *AST,struct STScope *scope)
{
	if(AST==NULL)
	{
		return "";
	}
	if(AST->val == ID )
	{
		struct STNode *temp = findLexemeNode(AST,scope,1);
		return temp->type;
	}
	if(AST->val == NUM)
	{
		return "INTEGER";
	}
	if(AST->val == RNUM)
	{
		return "REAL";
	}
	getIdNodeType(AST->AST_child,scope);
}


int arithmeticOrBoolean(struct tree_unit *AST)
{

	if(AST->val == arithmeticExpr)
	{
		return 1;
	}
	else if (AST->val == arithmeticOrBooleanExpr)
	{ 
		if(AST->AST_child->AST_next==NULL) // N7 is null
		{
			return 1;
		}
		else
		{
			if(checkAnyTermBoolean2(AST->AST_child))
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
	}

	return 1;

}

// To check the type of identifier


void create_function_table(struct tree_unit *AST,struct STScope *scope)// AST corresponding to module 
{
	if(AST->val!=module)
	{
		printf("Given wrong input to the function\n");
		return;
	}
	else
	{
		int slot;
		slot = hash_gen(AST->AST_child->lexeme);

		struct node *inp = in_table[slot].head;
		struct node *out = out_table[slot].head;


		struct tree_unit *inputList = AST->AST_child->AST_next->AST_child; // input_plist's 1st child

		//struct tree_unit *inputList = AST->AST_child->AST_next->AST_child; 



		struct node *inputs;
		struct node *outputs;

		if(inp==NULL)
		{
			in_table[slot].head=(struct node*)malloc(sizeof(struct node));

			strcpy(in_table[slot].head->lexeme,AST->AST_child->lexeme);
			strcpy(in_table[slot].head->type ,"proc");
			in_table[slot].head->next = NULL;
			in_table[slot].head->next_lexeme = NULL;
			inputs = in_table[slot].head;			// inputs here would be the node containing information about the function
		}
		else
		{
			inputs = inp;
			struct node *inputs_temp = inputs;
			while(inputs!=NULL)
			{
				inputs_temp = inputs;
				if(strcmp(inputs->lexeme,AST->AST_child->lexeme)==0)
				{	
					printf("Error: Redefiniton of function:- %s",AST->AST_child->lexeme);
					error_module_flag=1;
					return;
				}
				inputs=inputs->next;
			}

			inputs = (struct node*)malloc(sizeof(struct node));
			inputs_temp->next=inputs;
			strcpy(inputs->lexeme,AST->AST_child->lexeme);
			strcpy(inputs->type,"proc");
			inputs->next = NULL;
			inputs->isArray=0;
			inputs->next_lexeme = NULL;

		}

		

		while(inputList!=NULL)
		{
			inputs->next_lexeme=(struct node*)malloc(sizeof(struct node));
			strcpy(inputs->next_lexeme->lexeme,inputList->lexeme);
			//inputs->type = AST->type;
			
			if(inputList->AST_next !=NULL)
			{
				if(inputList->AST_next->val==NUM)
				{
					strcpy(inputs->next_lexeme->type,upper_string(inputList->AST_next->AST_next->AST_next->lexeme)); // correspondint to dataType array num..num integer

					int no_of_elements = atoi(inputList->AST_next->AST_next->lexeme) - atoi(inputList->AST_next->lexeme) + 1;

					inputs->next_lexeme->no_of_elements = no_of_elements;

					if(strcmp(inputs->next_lexeme->type,"INTEGER")==0)
					{
						inputs->next_lexeme->width = no_of_elements * 2;
						inputs->next_lexeme->offset = offset;
						offset  = offset + (no_of_elements * 2);
					}
					else if(strcmp(inputs->next_lexeme->type,"REAL")==0)
					{
						inputs->next_lexeme->width = no_of_elements * 4;
						inputs->next_lexeme->offset = offset;
						offset  = offset + (no_of_elements * 4);
					}
					else if(strcmp(inputs->next_lexeme->type,"BOOLEAN")==0)
					{
						inputs->next_lexeme->width = no_of_elements * 1;
						inputs->next_lexeme->offset = offset;
						offset  = offset + (no_of_elements * 1);		
					}

					inputs->next_lexeme->isArray=1;
					inputs->next_lexeme->scope = scope;
					inputList = inputList->AST_next->AST_next->AST_next->AST_next;
				}
				else
				{
					strcpy(inputs->next_lexeme->type,upper_string(inputList->AST_next->lexeme));
					inputs->next_lexeme->isArray=0;

					if(strcmp(inputs->next_lexeme->type,"INTEGER")==0)
					{
						inputs->next_lexeme->width = 2;
						inputs->next_lexeme->offset = offset;
						offset = offset + 2;
					}
					else if(strcmp(inputs->next_lexeme->type,"REAL")==0)
					{
						inputs->next_lexeme->width = 4;
						inputs->next_lexeme->offset = offset;
						offset = offset + 4;
					}
					else if(strcmp(inputs->next_lexeme->type,"BOOLEAN")==0)
					{
						inputs->next_lexeme->width = 1;
						inputs->next_lexeme->offset = offset;
						offset = offset + 1;			
					}

				//	inputs->next_lexeme->scope = (struct STScope*)malloc(sizeof(struct STScope));

					inputs->next_lexeme->scope = scope;
					inputList = inputList->AST_next->AST_next;
				}
			}
			else
			{
				inputList = inputList->AST_next;
			}	
			
			inputs=inputs->next_lexeme;
			//inputs=inputs->next_lexeme;
		}

		


		if(out == NULL)
		{
			out_table[slot].head=(struct node*)malloc(sizeof(struct node));

			strcpy(out_table[slot].head->lexeme,AST->AST_child->lexeme);
			strcpy(out_table[slot].head->type ,"proc");
			out_table[slot].head->next = NULL;
			out_table[slot].head->next_lexeme = NULL;
			outputs = out_table[slot].head;
		}
		else
		{
			outputs = out;
			struct node *outputs_temp = outputs;
			while(outputs!=NULL)
			{
				outputs_temp = outputs;
				if(strcmp(outputs->lexeme,AST->AST_child->lexeme)==0)
				{	
					printf("Error: Redefiniton of function:- %s",AST->AST_child->lexeme);
					error_module_flag=1;
					return;
				}
				outputs=outputs->next;

				outputs = (struct node*)malloc(sizeof(struct node));
				outputs_temp->next=outputs;
				strcpy(outputs->lexeme,AST->AST_child->lexeme);
				strcpy(outputs->type,"proc");
				outputs->isArray=0;
				outputs->next = NULL;
				outputs->next_lexeme = NULL;
			}
		}

		if(AST->AST_child->AST_next->AST_next->val!=ret)
		{
			return;
		}

		struct tree_unit *outputList = AST->AST_child->AST_next->AST_next->AST_child->AST_child; //module->id->input_plist->ret->output_plist->id

		while(outputList!=NULL)
		{
			outputs->next_lexeme=(struct node*)malloc(sizeof(struct node));
			strcpy(outputs->next_lexeme->lexeme,outputList->lexeme);
			//inputs->type = AST->type;

			if(outputList->AST_next !=NULL)
			{
				if(outputList->AST_next->val==NUM)
				{
					strcpy(outputs->next_lexeme->type,upper_string(outputList->AST_next->AST_next->AST_next->lexeme)); // correspondint to dataType array num..num integer
					
					int no_of_elements = atoi(outputList->AST_next->AST_next->lexeme) - atoi(outputList->AST_next->lexeme) + 1;

					outputs->next_lexeme->no_of_elements = no_of_elements;

					if(strcmp(outputs->next_lexeme->type,"INTEGER")==0)
					{
						outputs->next_lexeme->width = no_of_elements * 2;
						outputs->next_lexeme->offset = offset;
						offset = offset + (no_of_elements * 2);
					}
					else if(strcmp(outputs->next_lexeme->type,"REAL")==0)
					{
						outputs->next_lexeme->width = no_of_elements * 4;
						outputs->next_lexeme->offset = offset;
						offset = offset + (no_of_elements * 4);
					}
					else if(strcmp(outputs->next_lexeme->type,"BOOLEAN")==0)
					{
						 outputs->next_lexeme->width = no_of_elements * 1;
						 outputs->next_lexeme->offset = offset;
						 offset = offset + (no_of_elements * 1);		
					}

					outputs->next_lexeme->isArray=1;
					outputs->next_lexeme->scope = scope;
					outputList = outputList->AST_next->AST_next->AST_next->AST_next;
				}
				else
				{
					strcpy(outputs->next_lexeme->type,upper_string(outputList->AST_next->lexeme));
					outputs->next_lexeme->isArray=0;

					if(strcmp(outputs->next_lexeme->type,"INTEGER")==0)
					{
						outputs->next_lexeme->width = 2;
						outputs->next_lexeme->offset = offset;
						offset += 2;
					}
					else if(strcmp(outputs->next_lexeme->type,"REAL")==0)
					{
						outputs->next_lexeme->width = 4;
						outputs->next_lexeme->offset = offset;
						offset += 4;
					}
					else if(strcmp(outputs->next_lexeme->type,"BOOLEAN")==0)
					{
						outputs->next_lexeme->width = 1;
						outputs->next_lexeme->offset = offset;
						offset += 1;			
					}

					outputs->next_lexeme->scope = scope;
					outputList = outputList->AST_next->AST_next;
				}
			}
			else
			{
				outputList = outputList->AST_next;
			}	
			outputs=outputs->next_lexeme;
		}
	}
}
void printScope(struct STScope *scope)
{
	int i=0;

	for(i=0;i<SIZE;i++)
	{
		if(scope[i].head!=NULL)
		{
			struct STNode *temp = scope[i].head;
			printf("slot %d ",i);
			while(temp!=NULL)
			{
				printf("%s %s- ",temp->lexeme,temp->type);
				temp=temp->next;	
			}
			printf("\n");
		}
	}
}
void printGlobalTable()
{
	int i=0;

	for(i=0;i<SIZE;i++)
	{
		if(global_scope[i].head!=NULL)
		{
			struct STNode *temp = global_scope[i].head;
			printf("slot %d ",i);
			while(temp!=NULL)
			{
				printf("%s - ",temp->lexeme);
				temp=temp->next;	
			}
			printf("\n");
		}
	}
}
struct STNode* create_st_node(char *lexeme,char *type,char *scope,int offset,struct STNode *next,int isDefined,int no_of_elements)
{
	struct STNode *node = (struct STNode*) malloc(sizeof(struct STNode));

	strcpy(node->lexeme,lexeme);
	strcpy(node->scope,scope);
	strcpy(node->type,type);

	node->offset = offset;
	node->STNext = NULL;
	node->next = next;

	node->module_is_defined=isDefined;

	node->no_of_elements = no_of_elements;

	node->width = 0;

	return node;	

}
void addToGlobalScope(struct tree_unit *child)
{
	if(child==NULL)
		return;

	if(child->val == ID)
	{
		int slot = hash_gen(child->lexeme);

		struct STNode *temp = global_scope[slot].head;
		//struct STNode *node = create_st_node(child->lexeme,"proc","global",0,temp);
		int isDefined=0;
		if(temp==NULL)
		{
			
			if(child->AST_parent->val==otherModules)
				isDefined=1;
			global_scope[slot].head = create_st_node(child->lexeme,"proc","global",0,temp,isDefined,1);
		}
		else
		{
			struct STNode *temp2 = global_scope[slot].head;
			while(temp2!=NULL)
			{
				if(strcmp(child->lexeme,temp2->lexeme)==0 && child->AST_parent->val==moduleDeclarations)
				{
					printf("Error: Re-declaration of module %s\n",child->lexeme);
				}
				if(strcmp(child->lexeme,temp2->lexeme)==0 && child->AST_parent->val==otherModules && temp2->module_is_defined==1)
				{
					printf("Error: Re-definition of module %s\n",child->lexeme);
				}
				temp2=temp2->next;
			}

			if(child->AST_parent->val==otherModules)
				isDefined=1;

			global_scope[slot].head = create_st_node(child->lexeme,"proc","global",0,temp,isDefined,1);


		}
	}
	if(child->val== driverModule)
	{
		int slot = hash_gen(child->lexeme);
		struct STNode *temp = global_scope[slot].head;	
		global_scope[slot].head = create_st_node("1DRIVER_MODULE","driver_proc","global",0,temp,0,1);
	}
	addToGlobalScope(child->AST_next);

	
}
void initializeGlobalScope(struct tree_unit *AST)
{
	//struct tree_unit *AST = tree_root->child;
	if(AST==NULL)
		return;
	switch(AST->val)
	{
		case moduleDeclarations:
			//printf("\n..hello here..%s \n",AST->AST_child->lexeme);
			addToGlobalScope(AST->AST_child);
			break;
		case driverModule:
			addToGlobalScope(AST);
			break;
		/*case otherModules:
			printf("\n..hello here..\n %s",AST->AST_child->AST_child->lexeme);
			addToGlobalScope(AST->AST_child->AST_child);
			break;*/
	}
	initializeGlobalScope(AST->AST_next);
	return;
}
void addChildScope(struct STScope *parent_scope,struct STScope *child_scope)
{
	struct STScope *temp = parent_scope->child_scope;
	if(temp==NULL)
	{
		parent_scope->child_scope = child_scope;
		child_scope->parent = parent_scope;
	}
	else
	{
		struct STScope *temp2 = temp;
		while(temp!=NULL)
		{
			temp2=temp;
			temp = temp->next_scope;
		}
		temp2->next_scope = child_scope;
		temp2->next_scope->parent = parent_scope;
	}
}
/*
	here lexeme represents function name in case of scope of function
*/
int matchInputList(struct tree_unit *AST,char *func_name,struct STScope *scope)
{
	struct node *temp = in_table[hash_gen(func_name)].head;

	struct tree_unit *inp = AST;
	if(temp==NULL)
	{
		printf("Line no: %d Error:-Function definition doesn't exist\n",AST->lno);
		return 0;
	}
	else
	{
		while(temp!=NULL)
		{
			if(strcmp(temp->lexeme,func_name)==0)
			{
				break;
			}
			temp = temp->next;
		}
		if(temp==NULL)
		{
			printf("Line no: %d Error:-Function definition doesn't exist\n",AST->lno);
			return 0;
		}
		else
		{
			temp = temp->next_lexeme;
			while(temp!=NULL && inp!=NULL)
			{
				if(strcmp(temp->type,ID_type(inp,scope))!=0)
				{
					 return 0;
				}
				temp = temp->next_lexeme;
				inp=inp->AST_next;
			}
		}
	}

	if(temp==NULL && inp==NULL)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int matchOutputList(struct tree_unit *AST,char *func_name,struct STScope *scope)
{
	struct node *temp = out_table[hash_gen(func_name)].head;

	struct tree_unit *out = AST;
	if(temp==NULL)
	{
		printf("Line no: %d Error:-Function definition doesn't exist\n",AST->lno);
		return 0;
	}
	else
	{
		while(temp!=NULL)
		{
			if(strcmp(temp->lexeme,func_name)==0)
			{
				break;
			}
		}
		if(temp==NULL)
		{
			printf("Line no: %d Error:-Function definition doesn't exist\n",AST->lno);
			return 0;
		}
		else
		{
			temp = temp->next_lexeme;
			while(temp!=NULL && out!=NULL)
			{
				if(strcmp(temp->type,ID_type(out,scope))!=0)
				{
					return 0;
				}
				temp = temp->next_lexeme;
				out=out->AST_next;
			}
		}
	}

	if(temp!=NULL || out!=NULL)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
struct tree_unit* getStatements(struct tree_unit *AST)
{
	if(AST==NULL)
		return NULL;
	if(AST->val == statements)
	{
		return AST;
	}
	else
	{
		return getStatements(AST->AST_next);
	}
	//return NULL;
}

void checkSwitch(struct tree_unit *AST,struct STScope *scope)
{
	char type[100];
	char type2[100];

	strcpy(type,ID_type(AST->AST_child,scope));

	struct tree_unit *temp =AST->AST_child->AST_next->AST_child;

	while(temp!=NULL)
	{
		if(strcmp(temp->lexeme,"NUM")==0)
		{
			strcpy(type2,"INTEGER");
		}

		if(strcmp(temp->lexeme,"RNUM")==0)
		{
			strcpy(type2,"REAL");
		}

		if(strcmp(temp->lexeme,"true")==0)
		{
			strcpy(type2,"BOOLEAN");
		}
		if(strcmp(temp->lexeme,"false")==0)
		{
			strcpy(type2,"BOOLEAN");
		}

		

		if(strcmp(type2,type)!=0)
		{
			printf("Line No: %d Error case arguments error \n",temp->lno);
		}

		temp = temp->AST_next->AST_next;
	}
}

struct tree_unit *getParentFunc(struct tree_unit *AST)
{
	if(AST==NULL)
	{
		return NULL;
	}
	if(AST->val == module)
	{
		return AST->AST_child;
	}

	return getParentFunc(AST->AST_parent);
}
struct tree_unit *getModuleNameFromCall(struct tree_unit *AST)
{
	if(AST==NULL)
	{
		return NULL;
	}
	if(AST->val == ID)
	{
		return AST;
	}

	return getModuleNameFromCall(AST->AST_next);
}
struct STScope* GenerateScope(char *lexeme,struct tree_unit *AST,struct STScope *scope,int offset)
{
	
	int flag=0;
	int skip_child=0;
	if(AST==NULL)
	{
		return scope;
	}
	if(AST->val == iterativeStmt)
	{
		skip_child=1;
		struct STScope *scope_new = initializeScope(SIZE,"ITERATIVE",scope->nesting_depth+1,scope->lexeme,0,scope);
		addChildScope(scope,scope_new);

		if(AST->child->val == FOR)
		{
			struct tree_unit *strt = AST->AST_child->AST_next->AST_next->node_parent->next->next;
			scope_new->start_lno = strt->lno;
			struct tree_unit *end = AST->AST_child->AST_next->AST_next->node_parent->next->next->next->next;
			scope_new->end_lno = end->lno;

			strcpy(scope_new->iterator,AST->AST_child->lexeme);

			if(strcmp(ID_type(AST->AST_child,scope),"INTEGER")!=0)
			{
				printf("Line No %d Error:- The iterator must be an integer",AST->AST_child->lno);
			}
		}
		int offset_temp = offset;
		offset = 0;

		if(AST->child->val == FOR)
		{
			// if(findLexemeNode(AST->AST_child,scope,1)!=NULL)
			// {
			// 	findLexemeNode(AST->AST_child,scope,1)->isiterator = 1; 
			// }
			// else if(getNodeFunc(AST->AST_child,scope)!=NULL)
			// {
			// 	getNodeFunc(AST->AST_child,scope)->isiterator = 1; 	
			// }
			// else 
			// {
			// 	printf("Line no %d Error, The iterator variable not declared",AST->AST_child->lno);
			// }
		}
		scope_new = GenerateScope("iterative",getStatements(AST->AST_child),scope_new,0);

    	if(AST->child->val == WHILE)
    	{
			struct tree_unit *strt = AST->AST_child->next->next;
			scope_new->start_lno = strt->lno;
			struct tree_unit *end = AST->AST_child->next->next->next->next;
			scope_new->end_lno = end->lno;
		}

		offset = offset_temp;
		if(AST->child->val == FOR)
		{
			// if(findLexemeNode(AST->AST_child,scope,1)!=NULL)
			// {
			// 	findLexemeNode(AST->AST_child,scope,1)->isiterator = 0; 
			// }
			// else if(getNodeFunc(AST->AST_child,scope)!=NULL)
			// {
			// 	getNodeFunc(AST->AST_child,scope)->isiterator = 0; 	
			// }
			// else
			// {
			// 	printf("Line no %d Error, The iterator variable not declared",AST->AST_child->lno);
			// }
		}
		//addChildScope(scope,scope_new);

		int x=0;
	}
	else if(AST->val == conditionalStmt)
	{
		skip_child=1;
		struct STScope *scope_new = initializeScope(SIZE,"CONDITIONAL",scope->nesting_depth+1,scope->lexeme,0,scope);
		
		int offset_temp = offset;
		offset = 0;
		addChildScope(scope,scope_new);
		checkSwitch(AST,scope);

		struct tree_unit *strt = AST->AST_child->next->next;
		struct tree_unit *end = AST->AST_child->next->next->next->next->next;

		scope_new->start_lno = strt->lno;
		scope_new->end_lno = end->lno;

		scope_new = GenerateScope("conditional",AST->AST_child->AST_next,scope_new,0);
		
		offset = offset_temp;
		//addChildScope(scope,scope_new);

		int x=0;
	}
	else if(AST->val == moduleReuseStmt)
	{
		skip_child = 1;
		int x=0;
		int x1=0;
		int y1=0;

		struct tree_unit *parent_func = getParentFunc(AST);
		struct tree_unit *module_name_from_call = getModuleNameFromCall(AST->AST_child);

		if(parent_func!=NULL)
		{
			if(strcmp(parent_func->lexeme,module_name_from_call->lexeme)==0)
			{
				printf("Line no %d Error Recursive function definition not allowed\n",module_name_from_call->lno);
			}
		}
		if(AST->AST_child->val == optional)
		{
			x = matchOutputList(AST->AST_child->AST_child,AST->AST_child->AST_next->lexeme,scope);
			x1 = matchInputList(AST->AST_child->AST_next->AST_next,AST->AST_child->AST_next->lexeme,scope);
		}
		else
		{
			//int y = matchOutputList(AST->AST_child,AST->AST_child->AST_next->lexeme).
			y1 = matchInputList(AST->AST_child->AST_next,AST->AST_child->lexeme,scope);
		}

		if(x == 0)
		{
			printf("Line No %d Error:- The output parameters dont't match with the function definition\n",AST->AST_child->AST_next->lno);
		}
		else if(x1 == 0)
		{
			printf("Line No %d Error:- The input parameters dont't match with the function definition\n",AST->AST_child->AST_next->lno);
		}
		else if(y1 == 0)
		{
			printf("Line No %d Error:- The input parameters dont't match with the function definition\n",AST->AST_child->lno);
		}
	}
	else if(AST->val == ID)
	{
		if(AST->AST_next!=NULL)
		{
			if(AST->AST_next->val == ID)
			{
				if(AST->AST_parent->val==ioStmt)
				{
					flag=findLexeme(AST,scope,1);
					if(flag==0)
					{
						printf("Error: var %s not declared",AST->lexeme);
					}
					flag=0;
					flag=findLexeme(AST->AST_next,scope,1);
					if(flag==0)
					{
						printf("Error: var %s not declared",AST->AST_next->lexeme);
					}
					AST=AST->next;

				}
				else if(AST->AST_parent->val==assignmentStmt)
				{
					char type1[100];
					char type2[100];

					error_flag=0;
					
					strcpy(type1,ID_type(AST,scope)); // AST is ID
					struct tree_unit *temp = AST;
					if(strcmp(type1,"array INTEGER")==0)
					{
						strcpy(type1,"INTEGER");
					}
					if(strcmp(type1,"array REAL")==0)
					{
						strcpy(type1,"REAL");
					}
					if(strcmp(type1,"array BOOLEAN")==0)
					{
						strcpy(type1,"BOOLEAN");
					}
					int isarithmetic;
					/*if(AST->AST_next->val!=NUM || AST->AST_next->val!=ID)
					{
						 isarithmetic = arithmeticOrBoolean(AST->AST_next);
					}*/
				//	isarithmetic = arithmeticOrBoolean(AST->AST_next->AST_next);
					strcpy(type2,ID_type(AST->AST_next,scope));
					if(strcmp(type2,"INTEGER")!=0)
					{
						printf("Line no %d Error:- The index of the array has to be of integer type\n",AST->lno);
						error_flag=1;
					}
					//Array bounds checking not implemented here now 
					temp=AST->AST_next;
					
					if(strcmp(type1,"BOOLEAN")==0)
					{
						check_boolean(AST->AST_next->AST_next,scope);
					}
					else
					{
						check_arithmetic(AST->AST_next->AST_next,type1,scope,1);
					}

					// if(isarithmetic==1)
					// {
					// 	struct tree_unit *temp = AST;
					// 	if(strcmp(type1,"")!=0)
					// 	{
					// 		if(AST->AST_next->val!=NUM || AST->AST_next->val!=ID)
					// 		{
					// 			check_arithmetic(AST,type1,scope,1);
					// 		}
					// 		else
					// 		{
					// 			check_arithmetic(AST,type1,scope,1);
					// 		}
					// 	}
						
					// 	AST=AST->AST_next;
					// }

					if(error_flag!=1)
					{
						if(strcmp(AST->lexeme,"i")==0)
							hogaya=1;
						
						struct STNode *temp =findLexemeNode(AST,scope,1);
						if(temp!=NULL)
							temp->isassigned = 1;
						else if(getNodeFunc(AST,scope)!=NULL)
						{
							getNodeFunc(AST,scope)->isassigned=1;
						}
					}

				}
				else if(AST->AST_parent->val==declareStmt)
				{
					char type[100];
					struct tree_unit *temp = AST;
					while(temp->val==ID)
					{
						temp=temp->AST_next;
					}
					if(temp->val != NUM)
					{
						flag = findLexeme(AST,scope,0);
						struct STNode *temp2=scope[hash_gen(AST->lexeme)].head; // can replace this with STNode *temp2 = getNode(scope,AST);
						if(func_par_error_check(AST,scope)==1)
						{
							printf("Line No %d Error: Previous definition %s of variable in function input parameter list\n",AST->lno,AST->lexeme);
						}
						else if (out_func_par_error_check(AST,scope)==1)
						{
							printf("Line No %d Error: Previous definition %s of variable in function output parameter list\n",AST->lno,AST->lexeme);
						}
						else if (strcmp(scope->iterator,AST->lexeme)==0)
						{
							printf("Line no %d Error:= The iterator value cannot be redefined\n",AST->lno);
						}
						else if(temp2==NULL)
						{
							scope[hash_gen(AST->lexeme)].head=create_st_node(AST->lexeme,getString(temp->val),lexeme,offset,temp2,0,1);
							if (strcmp(getString(temp->val),"INTEGER")==0)
							{
								offset +=2;
								scope[hash_gen(AST->lexeme)].head->width =  2;

							}
							else if(strcmp(getString(temp->val),"REAL")==0)
							{
								offset +=4;
								scope[hash_gen(AST->lexeme)].head->width =  4;
							}
							else if (strcmp(getString(temp->val),"BOOLEAN")==0)
							{
								offset+=1;
								scope[hash_gen(AST->lexeme)].head->width =   1;
							}
							// if()
							// offset + = 2;
						}
						else
						{
							struct STNode *temp3=temp2;
							while(temp3!=NULL)
							{
								if(strcmp(temp3->lexeme,AST->lexeme)==0)
								{			
									error_flag=1;
									flag=1;
									break;
								}
								temp3=temp3->next;
							}
							if(flag==0)
							{
								scope[hash_gen(AST->lexeme)].head=create_st_node(AST->lexeme,getString(temp->val),lexeme,offset,temp2,0,1);
								if (strcmp(getString(temp->val),"INTEGER")==0)
								{
									offset +=2;
									scope[hash_gen(AST->lexeme)].head->width =  2;
								}
								else if(strcmp(getString(temp->val),"REAL")==0)
								{
									offset +=4;
									scope[hash_gen(AST->lexeme)].head->width =  4;
								}
								else if (strcmp(getString(temp->val),"BOOLEAN")==0)
								{
									offset+=1;
									scope[hash_gen(AST->lexeme)].head->width =  1;
								}
								}
							else
							{
								printf("Line %d: The identifier <%s> is declared more than once.\n",AST->lno,AST->lexeme);
							}
						}
					}
					else
					{
						char dataType[100];
						strcpy(dataType,getString(temp->AST_next->AST_next->val));

						int no_of_elements = atoi(temp->AST_next->lexeme) -  atoi(temp->lexeme) + 1 ; // This +2 because johry's output is not accuratie and temp->lexeme is 1.. instead of 1

						int check = atoi(temp->lexeme);
						int check2 = atoi(temp->AST_next->lexeme);

						no_of_elements = abs(no_of_elements);

					//	no_of_elements = no_of_elements + 2 ; // This +2 because johry's output is not accuratie and temp->lexeme is 1.. instead of 1

						flag = findLexeme(AST,scope,0);

						struct STNode *temp2=scope[hash_gen(AST->lexeme)].head; // can replace this with STNode *temp2 = getNode(scope,AST);
						if(func_par_error_check(AST,scope)==1)
						{
							printf("Line No %d Error: Previous definition %s of variable in function input parameter list\n",AST->lno,AST->lexeme);
						}
						else if (out_func_par_error_check(AST,scope)==1)
						{
							printf("Line No %d Error: Previous definition %s of variable in function output parameter list\n",AST->lno,AST->lexeme);
						}
						else if (strcmp(scope->iterator,AST->lexeme)==0)
						{
							printf("Line no %d Error:= The iterator value cannot be redefined\n",AST->lno);
						}
						else if(temp2==NULL)
						{
							scope[hash_gen(AST->lexeme)].head=create_st_node(AST->lexeme,dataType,lexeme,offset,temp2,0,no_of_elements);
							scope[hash_gen(AST->lexeme)].head->isArray=1;

							if (strcmp(dataType,"INTEGER")==0)
							{
								offset += (no_of_elements*2);
								scope[hash_gen(AST->lexeme)].head->width = no_of_elements * 2;
							}
							else if(strcmp(dataType,"REAL")==0)
							{
								offset += (no_of_elements * 4);
								scope[hash_gen(AST->lexeme)].head->width = no_of_elements * 4;
							}
							else if (strcmp(dataType,"BOOLEAN")==0)
							{
								offset+= (no_of_elements * 1);
								scope[hash_gen(AST->lexeme)].head->width = no_of_elements * 1;
							}
							
							// Uncomment the lines below to do array bounds checking	
							//scope[hash_gen(AST->lexeme)].head->array_low=atoi(temp->lexeme);
							//scope[hash_gen(AST->lexeme)].head->array_high=atoi(temp->AST_next->lexeme);
						}
						else
						{
							struct STNode *temp3=temp2;
							while(temp3!=NULL)
							{
								if(strcmp(temp3->lexeme,AST->lexeme)==0)
								{			
									error_flag=1;
									flag=1;
									break;
								}
								temp3=temp3->next;
							}
							if(flag==0)
							{
								scope[hash_gen(AST->lexeme)].head=create_st_node(AST->lexeme,dataType,lexeme,0,temp2,0,no_of_elements);
								scope[hash_gen(AST->lexeme)].head->isArray=1;
								if (strcmp(dataType,"INTEGER")==0)
								{
									offset += (no_of_elements * 2);
									scope[hash_gen(AST->lexeme)].head->width = no_of_elements * 2;
								}
								else if(strcmp(dataType,"REAL")==0)
								{
									offset += ( no_of_elements * 4);
									scope[hash_gen(AST->lexeme)].head->width = no_of_elements * 4;
								}
								else if (strcmp(dataType,"BOOLEAN")==0)
								{
									offset += (no_of_elements * 1);
									scope[hash_gen(AST->lexeme)].head->width = no_of_elements * 1;
								}
							}
							else
							{
								printf("Line %d: The identifier <%s> is declared more than once.\n",AST->lno,AST->lexeme);
							}
						}
					}
				}
			}

			else if(AST->AST_next->val == INTEGER || AST->AST_next->val == REAL || AST->AST_next->val == BOOLEAN)
			{
				if(AST->AST_parent->val == declareStmt)
				{
					STNode *temp2=scope[hash_gen(AST->lexeme)].head; // can replace this with STNode *temp2 = getNode(scope,AST);
					flag = findLexeme(AST,scope,0);
					//printf("\n*** hellllloooo ****\n");
					if(func_par_error_check(AST,scope)==1)
					{
							printf("Line No %d Error: Previous definition %s of variable in function input parameter list\n",AST->lno,AST->lexeme);
					}
					else if (out_func_par_error_check(AST,scope)==1)
					{
						printf("Line No %d Error: Previous definition %s of variable in function output parameter list\n",AST->lno,AST->lexeme);
					}
					else if (strcmp(scope->iterator,AST->lexeme)==0)
					{
						printf("Line no %d Error:= The iterator value cannot be redefined\n",AST->lno);
					}
					else if(temp2==NULL)
					{
						scope[hash_gen(AST->lexeme)].head=create_st_node(AST->lexeme,getString(AST->AST_next->val),lexeme,offset,temp2,0,1);
						if (strcmp(getString(AST->AST_next->val),"INTEGER")==0)
						{
							offset +=2;
							scope[hash_gen(AST->lexeme)].head->width = 2;

						}
						else if(strcmp(getString(AST->AST_next->val),"REAL")==0)
						{
							offset +=4;
							scope[hash_gen(AST->lexeme)].head->width = 4;
						}
						else if (strcmp(getString(AST->AST_next->val),"BOOLEAN")==0)
						{
							offset+=1;
							scope[hash_gen(AST->lexeme)].head->width = 1;
						}
					}
					else
					{
						STNode *temp3=temp2;
						while(temp3!=NULL)
						{
							if(strcmp(temp3->lexeme,AST->lexeme)==0)
							{			
								error_flag=1;
								flag=1;
								break;
							}
							temp3=temp3->next;
						}
						if(flag==0)
						{
							scope[hash_gen(AST->lexeme)].head=create_st_node(AST->lexeme,getString(AST->AST_next->val),lexeme,offset,temp2,0,1);
							if (strcmp(getString(AST->AST_next->val),"INTEGER")==0)
							{
								offset +=2;
								scope[hash_gen(AST->lexeme)].head->width = 2;
							}
							else if(strcmp(getString(AST->AST_next->val),"REAL")==0)
							{
								offset +=4;
								scope[hash_gen(AST->lexeme)].head->width = 4;
							}
							else if (strcmp(getString(AST->AST_next->val),"BOOLEAN")==0)
							{
								offset+=1;
								scope[hash_gen(AST->lexeme)].head->width = 1;
							}
						}
						else
						{
							printf("Line %d: The identifier <%s> is declared more than once.\n",AST->lno,AST->lexeme);
						}
					}
				}

			}

			else if(AST->AST_next->val == NUM )
			{
				char dataType[100];
				//strcpy(dataType,"");
				int no_of_elements = atoi(AST->AST_next->AST_next->lexeme) - atoi(AST->AST_next->lexeme) + 1;

				//int check = atoi(AST->AST_next->lexeme);

 				no_of_elements = abs(no_of_elements);
				
				if(AST->AST_parent->val == declareStmt)
				{
					strcpy(dataType,getString(AST->AST_next->AST_next->AST_next->val));
					STNode *temp2=scope[hash_gen(AST->lexeme)].head; // can replace this with STNode *temp2 = getNode(scope,AST);
					flag = findLexeme(AST,scope,0);
					//printf("\n*** hellllloooo ****\n");
					if(func_par_error_check(AST,scope)==1)
					{
						printf("Line No %d Error: Previous definition %s of variable in function input parameter list\n",AST->lno,AST->lexeme);
					}
					else if (out_func_par_error_check(AST,scope)==1)
					{
						printf("Line No %d Error: Previous definition %s of variable in function output parameter list\n",AST->lno,AST->lexeme);
					}
					else if (strcmp(scope->iterator,AST->lexeme)==0)
					{
						printf("Line no %d Error:= The iterator value cannot be redefined\n",AST->lno);
					}
					else if(temp2==NULL)
					{
						scope[hash_gen(AST->lexeme)].head=create_st_node(AST->lexeme,dataType,lexeme,offset,temp2,0,no_of_elements);
						scope[hash_gen(AST->lexeme)].head->isArray=1;
						if (strcmp(dataType,"INTEGER")==0)
						{
							offset += (no_of_elements * 2);
							scope[hash_gen(AST->lexeme)].head->width = no_of_elements * 2;
						}
						else if(strcmp(dataType,"REAL")==0)
						{
							offset += (no_of_elements * 4);
							scope[hash_gen(AST->lexeme)].head->width = no_of_elements * 4;
						}
						else if (strcmp(dataType,"BOOLEAN")==0)
						{
							offset+= (no_of_elements * 1);
							scope[hash_gen(AST->lexeme)].head->width = no_of_elements * 1;
						}
					}
					else
					{
						STNode *temp3=temp2;
						while(temp3!=NULL)
						{
							if(strcmp(temp3->lexeme,AST->lexeme)==0)
							{			
								error_flag=1;
								flag=1;
								break;
							}
							temp3=temp3->next;
						}
						if(flag==0)
						{
							scope[hash_gen(AST->lexeme)].head=create_st_node(AST->lexeme,dataType,lexeme,offset,temp2,0,no_of_elements);
							scope[hash_gen(AST->lexeme)].head->isArray=1;
							if (strcmp(dataType,"INTEGER")==0)
							{
								offset +=no_of_elements * 2;
								scope[hash_gen(AST->lexeme)].head->width = no_of_elements * 2;
							}
							else if(strcmp(dataType,"REAL")==0)
							{
								offset +=no_of_elements * 4;
								scope[hash_gen(AST->lexeme)].head->width = no_of_elements * 4;
							}
							else if (strcmp(dataType,"BOOLEAN")==0)
							{
								offset+=no_of_elements * 1;
								scope[hash_gen(AST->lexeme)].head->width = no_of_elements * 1;
							}
						}
						else
						{
							printf("Line %d: The identifier <%s> is declared more than once.\n",AST->lno,AST->lexeme);
						}
					}
				}
				/*if(AST->AST_parent->val == iterativeStmt)
				{
					struct STScope = GenerateScope("ITERATIVE",AST->AST_parent);
				}*/
				if(AST->AST_parent->val == assignmentStmt)
				{
					char type1[100];
					char type2[100];

					error_flag = 0;

					strcpy(type1,ID_type(AST,scope));
					struct tree_unit *temp = AST;
					if(strcmp(type1,"array INTEGER")==0)
					{
						strcpy(type1,"INTEGER");
					}
					if(strcmp(type1,"array REAL")==0)
					{
						strcpy(type1,"REAL");
					}
					if(strcmp(type1,"array BOOLEAN")==0)
					{
						strcpy(type1,"BOOLEAN");
					}
					int isarithmetic;
					/*if(AST->AST_next->val!=NUM || AST->AST_next->val!=ID)
					{
						 isarithmetic = arithmeticOrBoolean(AST->AST_next);
					}*/
					isarithmetic = arithmeticOrBoolean(AST->AST_next->AST_next);
					strcpy(type2,ID_type(AST->AST_next,scope));
					if(strcmp(type2,"INTEGER")!=0)
					{
						printf("Error:- The index of the array has to be of integer type\n");
						error_flag=1;
					}
					//Array bounds checking not implemented here now 
					temp=AST->AST_next;
					
					if(isarithmetic==1)
					{
						struct tree_unit *temp = AST;
						if(strcmp(type1,"")!=0)
						{
								check_arithmetic(temp->AST_next->AST_next,type1,scope,1);
						}
						/*else
						{
							if(AST->AST_next!=NUM || AST->AST_next!=ID)
							{
								check_arithmetic(temp->AST_next->AST_next,type1,scope,0);
							}
							else
							{
								check_arithmetic(temp->AST_next,type1,scope,0);
							}
						}*/
						AST=AST->AST_next;
					}
					else
					{
						struct tree_unit *temp = AST;
						if(strcmp(type1,"")!=0)
						{
							// if(AST->AST_next->val!=NUM || AST->AST_next->val!=ID)
							// {
							// 	check_boolean(temp->AST_next->AST_next,scope);
							// }
							// else
							// {
							// 	check_boolean(temp->AST_next,scope);
							// }

							check_boolean(AST->AST_next->AST_next,scope);
						}
						/*else
						{
							if(AST->AST_next!=NUM || AST->AST_next!=ID)
							{
								check_boolean(temp->AST_next->AST_next,type1,scope,0);
							}
							else
							{
								check_boolean(temp->AST_next,type1,scope,0);
							}
						}*/
						AST=AST->AST_next;
					}

					if(error_flag!=1)
					{
						if(strcmp(AST->lexeme,"i")==0)
							hogaya=1;
						struct STNode *temp =findLexemeNode(AST,scope,1);
						if(temp!=NULL)
							temp->isassigned = 1;
						else if(getNodeFunc(AST,scope)!=NULL)
						{
							getNodeFunc(AST,scope)->isassigned=1;
						}
					}


				}

			}
			else if(AST->AST_next->val == arithmeticExpr || AST->AST_next->val == arithmeticOrBooleanExpr)
			{
					char type1[100];
					char type2[100];

					strcpy(type1,ID_type(AST,scope));
					struct tree_unit *temp = AST;

					error_flag = 0;

					if(strcmp(type1,"array INTEGER")==0)
					{
						strcpy(type1,"INTEGER");
					}
					if(strcmp(type1,"array REAL")==0)
					{
						strcpy(type1,"REAL");
					}
					if(strcmp(type1,"array BOOLEAN")==0)
					{
						strcpy(type1,"BOOLEAN");
					}
					int isarithmetic;
					if(AST->AST_next->val!=NUM || AST->AST_next->val!=ID)
					{
						 isarithmetic = arithmeticOrBoolean(AST->AST_next);
					}
					

					if(strcmp(type1,"BOOLEAN")==0)
					{
						check_boolean(AST->AST_next,scope);
					}
					
					else
					{
						check_arithmetic(AST->AST_next,type1,scope,1);
						
					}

					if(error_flag != 1)
					{
						if(strcmp(AST->lexeme,"i")==0)
							hogaya=1;
						struct STNode *temp =findLexemeNode(AST,scope,1);
						if(temp!=NULL)
							temp->isassigned = 1;
						else if(getNodeFunc(AST,scope)!=NULL)
						{
							getNodeFunc(AST,scope)->isassigned=1;
						}
					}
			}
		}
		else
		{
			if(AST->AST_parent->val == ioStmt)
			{
				flag = 0;
				flag = findLexeme(AST,scope,1);
				if(flag == 0)
				{
					printf("Error: The variable %s is not declared",AST->lexeme);
				}
			}
		}
	}
	/*if(AST->val == iterativeStmt)
	{
		struct STScope *scope_new = initializeScope(SIZE,"ITER",parent->nesting_depth+1,lexeme,0,scope);
	}*/
	//struct STScope* GenerateScope(char *lexeme,struct tree_unit *AST,struct STScope *scope,int offset)
	if(skip_child==0)
	{
		scope=GenerateScope(lexeme,AST->AST_child,scope,offset);
	}
	//}
	scope=GenerateScope(lexeme,AST->AST_next,scope,offset);
	return scope;
}
struct STScope* initializeScope(int size,char *lexeme,int nesting_depth,char *parent_lexeme,int isFunction,struct STScope *parent)
{
	int i=0;
	struct STScope *scope = (struct STScope*)malloc(size * sizeof(struct STScope));
	for(i=0;i<size;i++)
	{
		scope[i].head=NULL;
		strcpy(scope[i].lexeme,lexeme);
		scope[i].nesting_depth = nesting_depth;
		strcpy(scope[i].parent_lexeme,lexeme);
		scope[i].count = 0;
		scope[i].isFunction=isFunction;
		scope[i].parent = parent;
		scope[i].child_scope=NULL;
		scope[i].next_scope=NULL;
		scope[i].parent=NULL;
	}
	return scope;
}

void create_symbol_table(struct tree_unit *AST)
{

	if(AST==NULL)
	{
		return;
	}
	int slot;
	//int error = 0;
	//int flg =0;
	//struct tree_unit *tmp = AST->AST_child;
	switch(AST->val)
	{
		;
		int error =0 ;
		//int flg = 0 ;

		case driverModule:
			//create_symbol_table(AST->AST_next);
			//printf("\n**hello here**\n");
			slot = hash_gen("1DRIVER_MODULE"); 
			struct STScope *scope = initializeScope(SIZE,"1DRIVER_MODULE",0,"global_scope",0,global_scope);
			//struct STScope* GenerateScope(char *lexeme,struct tree_unit *AST,struct STScope *scope,int offset)
			struct tree_unit *strt = AST->child->next->next->next->next->child; 
			scope->start_lno = strt->lno;
			scope = GenerateScope("1DRIVER_MODULE",AST->AST_child,scope,0);
			struct tree_unit *end = AST->AST_child->next;
			scope->end_lno = end->lno;
			addChildScope(global_scope,scope);
			//printf("\n Scope of driver Module:- \n");
			//printScope(scope);
			//AST=AST->AST_next;
			break;


		case otherModules:

			
			;struct tree_unit *temp_module = AST->AST_child; // corresponds to module
			int flg=0;

			while(temp_module!=NULL)
			{
				error_module_flag=0;
				flg = 0;
				struct STNode *module_node = global_scope[hash_gen(temp_module->AST_child->lexeme)].head;

				if(module_node == NULL)
				{
					global_scope[hash_gen(temp_module->AST_child->lexeme)].head=create_st_node(temp_module->AST_child->lexeme,"proc","GLOBAL",0,module_node,1,1);
				//	create_function_table(temp_module);
				}
				else
				{
					while(module_node!=NULL)
					{

						if(strcmp(module_node->lexeme,temp_module->AST_child->lexeme)==0)
						{
							flg = 1;
							break;
						}
						module_node = module_node -> next;
					}
					if(flg==0)
					{
						global_scope[hash_gen(temp_module->AST_child->lexeme)].head=create_st_node(temp_module->AST_child->lexeme,"PROC","GLOBAL",0,module_node,1,1);
				//		create_function_table(temp_module);
					}
					else
					{
				//		create_function_table(temp_module);
					}
				}
				if(flg==1 && error_module_flag==0)
				{
					module_node->STNext = initializeScope(SIZE,temp_module->AST_child->lexeme,0,"global_scope",1,global_scope);
					int offset_temp = offset;
					offset = 0;

					addChildScope(global_scope,module_node->STNext);

					create_function_table(temp_module,module_node->STNext);

					struct tree_unit *strt = temp_module->AST_child->AST_next->next->next->next->next->child;
					module_node->STNext->start_lno=strt->lno;
					
					module_node->STNext= GenerateScope(temp_module->AST_child->lexeme,getStatements(temp_module->AST_child),module_node->STNext,offset);

					struct tree_unit *end = temp_module->AST_child->AST_next->next->next->next->next->child->next->next;
					module_node->STNext->end_lno=end->lno;

					if(outputAssinged(temp_module->AST_child)!=1)
					{
						printf("Line no %d The output parameter of function is not assigned\n",temp_module->AST_child->lno);
					}	

					offset = offset_temp;
				}
				else if(error_module_flag==0)
				{
					global_scope[hash_gen(temp_module->AST_child->lexeme)].head->STNext=initializeScope(SIZE,temp_module->AST_child->lexeme,0,"global_scope",1,global_scope);

					int offset_temp = offset;
					offset = 0;

					addChildScope(global_scope,global_scope[hash_gen(temp_module->AST_child->lexeme)].head->STNext);

					create_function_table(temp_module,global_scope[hash_gen(temp_module->AST_child->lexeme)].head->STNext);

					struct tree_unit *strt = temp_module->AST_child->AST_next->next->next->next->next->child;
					global_scope[hash_gen(temp_module->AST_child->lexeme)].head->STNext->start_lno=strt->lno;

					global_scope[hash_gen(temp_module->AST_child->lexeme)].head->STNext=GenerateScope(temp_module->AST_child->lexeme,getStatements(temp_module->AST_child),global_scope[hash_gen(temp_module->AST_child->lexeme)].head->STNext,offset);
					
					struct tree_unit *end = temp_module->AST_child->AST_next->next->next->next->next->child->next->next;
					global_scope[hash_gen(temp_module->AST_child->lexeme)].head->STNext->end_lno=end->lno;

					if(outputAssinged(temp_module->AST_child)!=1)
					{
						printf("Line no %d The output parameter of function is not assigned\n",temp_module->AST_child->lno);
					}	

					offset = offset_temp;
				}

				temp_module = temp_module->AST_next; // corresponds to the next module 
			}

				break;
	}
	create_symbol_table(AST->AST_next);
}

// void initializeFunctionTable(struct tree_unit *AST) // AST otherModules
// {
// 	if(AST==NULL)
// 	{
// 		return;
// 	}
// 	struct tree_unit *temp = AST;
// 	if(AST->val == otherModules)
// 	{
// 		temp = AST->AST_child;
// 		while(temp!=NULL)
// 		{
// 			create_function_table(temp);
// 			temp = temp->AST_next;
// 		}
// 	}
// 	initializeFunctionTable(AST->AST_next);
// }
int main()
{
	parser_main("testcase1.txt");
    buildTreeFILE("outing.txt");

    makeAST(tree_root,tree_root->child);

    int i=0;
    for(i=0;i<SIZE;i++)
    {
    	global_scope[i].nesting_depth = 0;
    	global_scope[i].parent = NULL;
    	global_scope[i].head = NULL;
    	global_scope[i].count = 0;
    	strcpy(global_scope[i].lexeme,"global_scope");
    	strcpy(global_scope[i].parent_lexeme,"NULL");
    	global_scope[i].isFunction = 0;
    	global_scope[i].child_scope = NULL;
    	global_scope[i].parent = NULL;

    }

    for(i=0;i<SIZE;i++)
    {
    	in_table[i].head=NULL;
    	in_table[i].list_count=0;

    	out_table[i].head=NULL;
    	out_table[i].list_count=0;

    }

    //initializeFunctionTable(tree_root->AST_child);
    initializeGlobalScope(tree_root->AST_child);
   // printAST(tree_root);
    printf("\n");
   // printGlobalTable();
    printf("\n");

    create_symbol_table(tree_root->AST_child);

    //printf("hello here");''

    printSymbolTable1(global_scope);


	printInOutTable();

    return 0;
}