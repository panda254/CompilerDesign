//Batch 66
//Akshit Johry 2014A7PS077P
//Arpit Panda 2014A7PS114P

#include "ff.c"

//#include "st_def.h"

#include "parserDef.h"

/*Lookup function detects the presence of an ID by string..gotParent specifies whether we need to look for it in the parent..like in case of checking if an ID is
already declared in the present scope or not*/

/*void insert(char *str, STtable* tab_node,int line, char *typeof){

int ins_pos=hash_gen(str);

}*/


// parser code from here 

int rule_count = 0;
int rulesCount = 0;

int isNonTerm(int value)
{
    if(value>=62)
        return 1;
    else
        return 0;
}

void inOrderTraversal(struct tree_unit *tree_root,FILE *fp)
{
	int i=0;
	if(isNonTerm(tree_root->val)==0)
	{
		if(tree_root->val!=EMPTY){

		fprintf(fp,"%s %d %s ",tokens[token2_len2].lexeme,tokens[token2_len2].line_num,stringss[tokens[token2_len2].token_num]);
		fprintf(fp,"%s ",tokens[token2_len2].lexeme);
			/*
			if(tokens[token2_len2].token_num==NUM)
`				{printf("%d ",calcint(tokens[token2_len2].lexeme);}
			else if(tokens[token2_len2].token_num==RNUM)
				{printf("%d ",calcRint(tokens[token2_len2].lexeme);}
			*/
		fprintf(fp,"%s yes\n",stringss[tree_root->parent]);


		//printf(" %s %s %d %s\n",,);
		/*lexemeCurrentNode     lineno   token   valueIfNumber   parentNodeSymbol   isLeafNode(yes/no)    NodeSymbol

The lexeme of the current node is printed when it is the leaf node else a dummy string of characters "----" is printed. The line number is one of the information collected by the lexical analyzer during single pass of the source code. The token name corresponding to the current node is printed third. If the lexeme is an integer or real number, then its value computed by the lexical analyzer should be printed at the fourth place.  Print the grammar symbol (non terminal symbol) of the parent node of the currently visited node appropriately at fifth place (for the root node  print ROOT for parent symbol) . The sixth column is for printing yes or no appropriately. Print the nonterminal symbol of the node being currently visited at the 7th place, if the node is not the leaf node [Print the actual nonterminal symbol and not the enumerated values for the nonterminal]. Ensure appropriate justification so that the columns appear neat and straight.*/
		token2_len2++;}
		return;
	}
	else
	{

		inOrderTraversal(&tree_root->nodes[0],fp);
   fprintf(fp,"---- ---- ---- ---- "); 
			if(tree_root->parent==-1)
			fprintf(fp,"ROOT no\n");
			else
			fprintf(fp,"%s no %s\n",stringss[tree_root->parent],stringss[tree_root->val]);
		//printf("%s yes\n",stringss[tree_root->parent])
	}

	//printf(" %s  ",stringss[tree_root->val]);


	for(i=1 ;i< tree_root->len ;i++)
	{
		inOrderTraversal(&tree_root->nodes[i],fp);
	}

  }
int parse_token_len=0;
 void buildParseTree(struct tree_unit *tree_root,struct rule rules[],int val,int parent)
 {
	int count = rule_count;
	rule_count++;
	tree_root->val = val;
	tree_root->len = rules[count].len;
	tree_root->parent = parent;
	int length = rules[count].len;
	tree_root->nodes = (struct tree_unit*)malloc((rules[count].len) * sizeof(struct tree_unit));
	tree_root->isLeaf=0;
	tree_root->AST_child = NULL;
	tree_root->AST_parent = NULL;
	tree_root->AST_next = NULL;
	int j=0;
	for(j=0;j<length;j++)
	{
		tree_root->nodes[j].node_parent=tree_root;
		
		// Change made for easier AST code here 

		if(j==0)
		{
			tree_root->child=&(tree_root->nodes[j]);
		}
		if(j!=length-1)
		{
			tree_root->nodes[j].next=&(tree_root->nodes[j+1]);
		}
		else
		{
			tree_root->nodes[j].next=NULL;
		}	
	}
	for(j=0;j<length;j++)
	{
		if(isNonTerm(rules[count].r_arr[j])==1)
			buildParseTree(&tree_root->nodes[j],rules,rules[count].r_arr[j],tree_root->val);
		else
		{
			tree_root->nodes[j].val = rules[count].r_arr[j];
			tree_root->nodes[j].len = 0;
			tree_root->nodes[j].parent = tree_root->val;
			tree_root->nodes[j].child = NULL;
			tree_root->isLeaf=1;
			tree_root->nodes[j].AST_child = NULL;
			tree_root->nodes[j].AST_parent = NULL;
			tree_root->nodes[j].AST_next = NULL;
			if(tree_root->nodes[j].val!=61)// 61 represents empty
			{
				strcpy(tree_root->nodes[j].lexeme,tokens[parse_token_len].lexeme);
				tree_root->nodes[j].lno=tokens[parse_token_len++].line_num;
				//parse_token_len++;
				//printf("* no here:- %d %d %s %d\n",parse_token_len,tree_root->nodes[j].val,tree_root->nodes[j].lexeme,tree_root->nodes[j].lno);
			}
		}
	}
	if(rule_count==rulesCount)
		return;

 }
 struct stackNode *top=NULL;


struct list
{
    int ui;
    struct list *next;
};
 void pushStack(int value)
 {
	struct stackNode *newNode;
	newNode = (struct stackNode*)malloc(sizeof(struct stackNode));
	newNode->ui = value;
	if(top == NULL)
	{
		newNode->next = NULL;
	}
	else
	{
		newNode->next = top;
	}

	top = newNode;

	//printf("Insertion is successful");
 }

 void popStack()
 {
	if(top==NULL)
	{
		printf("\nStack is empty\n");
	}
	else
	{
		struct stackNode *temp = top;
		top = temp->next;
		free(temp);
	}
 }

int GetInt(char word[])
{
	int i=0;
	for(i=0;i<110;i++)
	{
		if(strcmp(word,stringss[i])==0)
			return i;
	}
	return -1;
}
   char* getString(int val)
	{
		return stringss[val];
	}

  void printRule(struct rule r)
  {
	printf("	%s	->	",getString(r.left));
	int i=0;
	for(i=0;i<r.len;i++)
	{
		printf("%s ",getString(r.r_arr[i]));
	}
  }

  int isEmptyy(struct Units first[],int arr[],int len)
  {
	int i=0;
	for(i=0;i<len;i++)
	{
		if(first[arr[i]].isEmpty==0)
			return 0;
	}
	return 1;
  }

  struct Units computeFirst(int x[],int gram_len,struct Units first[])
  {
	struct Units temp;
	temp.len=0;
	temp.isEmpty=0;
	temp.isDollar=0;

	int i=0;
	while(1)
	{
		int j=0;
		for(j=0;j<first[x[i]].len;j++)
		{
			if(i==gram_len-1)
			{
				if(first[x[i]].u_arr[j]==EMPTY)
				{
					temp.u_arr[temp.len++]=first[x[i]].u_arr[j];
					temp.isEmpty=1;
				}
			}
			if(first[x[i]].u_arr[j]!=EMPTY)
				temp.u_arr[temp.len++]=first[x[i]].u_arr[j];

			if(first[x[i]].u_arr[j]==DOLLAR)
			{
				temp.isDollar=1;
			}
		}
		if(first[x[i]].isEmpty==0)
		{
			break;
		}
		else
		{
			i++;
		}
		if(i==gram_len)
			break;
	}
	return temp;
  }
  void populateTable(struct rule matrix[][150],struct Units first[],struct Units follow[],struct rule grammer[],int len)
   {

	int i=0;

	int j=0;

	for(i=0;i<len;i++)
	{

		/*if(grammer[i].left==ret)
		{
			printf("*******************%d******************** ",i);
			printRule(grammer[i]);
			printf("***************************************\n");
		}*/
		//printRule(grammer[i]);
		//printf("\n");
		int m=0;
		//printf("\n 1. %s %d %d \n",stringss[grammer[i].left] ,i,first[grammer[i].left].len);
		struct Units tmp = computeFirst(grammer[i].r_arr,grammer[i].len,first);
		if(i==8)
		{
			//printf("\n*******************%d******************** \n",i);
			//printf("%s",stringss[first[RETURNS].u_arr[0]]);
			//printf("\n***************************************\n");
		}
		//printf("3. ");
		//printRule(grammer[i]);
		//printf("\n");
		//printf("%s -> ",stringss[grammer[i].left]);
		/*for(j=0;j<tmp.len;j++)
		{
			printf("%s ",stringss[tmp.u_arr[j]]);
		}*/

		//printf("\n");
		for(m=0;m<tmp.len;m++)
		{
			if(isNonTerm(tmp.u_arr[m])==0)
			{
				//printf("\n 1. %s %s ",stringss[grammer[i].left],stringss[tmp.u_arr[m]]);
				//printRule(grammer[i]);
				//printf("%d \n",i);
				//printf("\n");
				//printf("\n");
				//printf("\n------------------");
				//if(i==8)
				//  printf("\n..........ddd.........%s %s\n",stringss[grammer[i].left],stringss[tmp.u_arr[m]]);
				matrix[grammer[i].left][tmp.u_arr[m]]=grammer[i];
			}
		}
		//printf("\n...eee...\n");

		if(tmp.isEmpty==1)
		{
			struct Units temp_unit;
			temp_unit = follow[grammer[i].left];
			for(j=0;j<follow[grammer[i].left].len; j++)
			{
				/*printf("\n4. ");
					printRule(grammer[i]);
					printf("\n");
				*/
				if(isNonTerm(follow[grammer[i].left].u_arr[j])==0)
				{
					//printf("%s %s",stringss[grammer[i].left],stringss[follow[grammer[i].left].u_arr[j]]);
					//printRule(grammer[i]);
					//printf(" %d \n",i);
					//printf("\n");
					matrix[grammer[i].left][follow[grammer[i].left].u_arr[j]]=grammer[i];
				}
			}
		}
	}


   }
   void parseString(struct Token *root,struct rule matrix[][150],struct rule rules[])
   {

	while(1)
	{
		if(top->ui == DOLLAR || root==NULL)
		{
			break;
		}
		/* Stack is not empty */
		int x,a;

		x = top->ui;

		a = root->token_num;


		//printf("\n Top of Stack is %s and Top of input Symbol is %s \n",stringss[x],stringss[a]);

		if(x == a)
		{
			//printf("\n There is a match %s --- %s \n",stringss[root->token_num],stringss[top->ui]);
			struct Token2 tk2;
			tk2.token_num = root->token_num;
			tk2.line_num = root->line_num;
			strcpy(tk2.lexeme,root->lexeme);

			tokens[token2_len++]=tk2;

			//printf(" no here:- %d %d\n",token2_len,tk2.token_num);

			//printf("\n ........................popping stack: %s \n",stringss[top->ui]);
			popStack();
			root = root->next;
		}
		else if(isNonTerm(top->ui)==0)
		{
			printf("Compilation Unsuccessful\n");
			printf("Error_5: The token %s for lexeme %s does not match at line no. %d. The expected token here is %s", strings[a],root->lexeme,root->line_num,stringss[top->ui]);
			//printf("\n1.There is a parse error\n");
			return;
		}
		else if(matrix[x][a].left==-1)
		{
			printf("Compilation Unsuccessful\n");
			printf("Error_5: The token %s for lexeme %s does not match at line no. %d. The expected token here are", strings[a],root->lexeme,root->line_num);

			int i=0;
			/*for(i=0;i<150;i++)
			{
				if(matrix[top->ui][i].left!=-1 && i!=EMPTY && i!=DOLLAR)
					printf("%s ",stringss[i]);
			}*/
			//printf("\n2. There is a parse error\n");
			return;
		}
		else
		{
			//printRule(matrix[x][a]);
			rules[rulesCount++]=matrix[x][a];
			//printf("%d \n",matrix[x][a].r_arr[0]);
			popStack();
			int k=0;
			for(k=matrix[x][a].len-1;k>=0;k--)
			{
					if(matrix[x][a].r_arr[k]!=61) //Empty represents 61
					{
						//printf("\n Pushing onto stack %s\n",stringss[matrix[x][a].r_arr[k]]);
						pushStack(matrix[x][a].r_arr[k]);
					}
			}
			//root = root->next;
		}
	}
	//printf("\n The top of stack is:- %s \n",stringss[top->ui]);
	if(top->ui!=DOLLAR)
	{
		while(top->ui!=DOLLAR)
		{

			if(matrix[top->ui][DOLLAR].r_arr[0]==61)
			{
				rules[rulesCount++]=matrix[top->ui][DOLLAR];
				//printRule(matrix[top->ui][DOLLAR]);
				//printf("\n The top of stack is:- %s \n",stringss[top->ui]);
				popStack();
				//printf("\n The top of stack is:- %s \n",stringss[top->ui]);
			}
			else
			{
				break;
			}
		}
	}

	if(top->ui!=DOLLAR)
	{
		printf("\n Compilation Unsuccessful\n");
	}
	printf("\n Compilation Successful\n");

   }
  void buildGrammer(struct rule grammer[])
{
	FILE *fp;
	//FILE *fp1;
	char word[400];
	fp = fopen("grammer.txt","r");
	//fp1 = fopen("hackk.txt","w");
	const char s[2] = " ";
	int g=0;
	//while(fgets(word,400,fp)!=NULL)
	while(!feof(fp))
	{
		char* sa = (char*) malloc(sizeof(char) * 100 );
		fscanf(fp,"%s",sa);
		//fprintf(fp1,"%s -> ",sa);
		int i=0;
		while(1)
		{
			if(strcmp(sa,".")==0 || feof(fp))
			{
				//fprintf(fp1,"\n");
				break;
			}
			if(i==0)
			{
			//printf("%s -> ",sa);
			grammer[g].left=find_num(sa);
			grammer[g].len=0;
			}
			else
			{
			//printf(" %s ",sa);
			grammer[g].r_arr[grammer[g].len++]=find_num(sa);
			}
			i++;
			fscanf(fp,"%s",sa);
			//fprintf(fp1,"%s ",sa);
		}
		g++;
		//printf("\n --- New rule --- \n");

	}
	//printf("\n-------------- closed ---------------\n");
	fclose(fp);


}
void initializeTable(struct rule matrix[][150])
{
    int i=0,j=0;
    for(i=0;i<150;i++)
    {
        for(j=0;j<150;j++)
            matrix[i][j].left = -1;
    }
}
struct tree_unit *ptr;

int toBeNotIgnored(int val)
{

	/*
	   Set of terminals to be not ignored

	   id
	   num
	   integer
	   real
	   boolean
	   and
	   or
	   lt
	   le
	   gt
	   ge
	   eq
	   ne
	   plus
	   minus
	   mul
	   div

	*/

	/* 
		Set of non terminals to be ignored

		moduleDeclaration
		N1
		range
		type
		N2
		moduleDef
		statement
		whichId
		simpleStmt
		whichStmt
		idList
		N3
		expression
		N7
		logicalOp
		N4
		op1
		op2
		factor
		value
		N9
		datatype
		lvalueIDStmt
		lvalueARRStmt
		Index
		optional

	*/

	/*
	    set of non terminals to be not ignored

	    moduleDeclarations
		otherModules
		module
		input_plist
		ret
		output_plist
		statements
		arithmeticExpr
		arithmeticOrBooleanExpr
		AnyTerm
		term
		ioStmt
		assignmentStmt
		moduleReuseStmt
		declareStmt
		conditionalStmt
		iterativeStmt 
		caseStmts
		Default
		driverModule
		var 
		relationoalOp
	*/

	if 
	(
		val == ID || val == NUM || val == INTEGER || val == REAL || val == 23 
		|| val == AND || val == OR || val == LT || val == LE || val == GT 
		|| val == GE || val == EQ || val== NE || val == PLUS || val == MINUS || val == MUL || val == DIV || val == TRUE || val == FALSE

		|| val == moduleDeclarations || val == otherModules || val == module || val == input_plist || val == ret || val == ret || val == output_plist
		|| val == statements || val == ioStmt || val == assignmentStmt || val == moduleReuseStmt || val == arithmeticExpr || val == arithmeticOrBooleanExpr || val == AnyTerm || val == term || val == declareStmt || val == conditionalStmt 
		|| val == caseStmts || val == Default || val == iterativeStmt || val == driverModule || val == var || val == relationalOp || val == optional

	)
	{
		return 1;
	}
	
	else
	{
		return 0;
	}
}

void printAST(struct tree_unit *root)
{
	printf("..%s %s \n",getString(root->val), root->lexeme );
	struct tree_unit *temp = root->AST_child;
	while(temp!=NULL)
	{
		printAST(temp);
		temp = temp->AST_next;
	}

}

void makeAST(struct tree_unit *parent,struct tree_unit *child)
{
	if(child==NULL)
	{
		return;
	}

	/*if((child->val>=program && child->child==NULL)) // 61 represents empty
	{
		return;
	}*/

	if(child->val == EMPTY) // 61 represents empty
	{
		return;
	}

	/*if(child->val >= program && child->child->val==EMPTY)
	{
		return;
	} */
	
	if( toBeNotIgnored(child->val)== 1 && child->val != parent->val)
	{
			

			if(child->val >= program && child->child->val==EMPTY)
			{
				
			} 
			else
			{
				struct tree_unit *temp= parent->AST_child;
				struct tree_unit *temp2;

				if(temp==NULL)
				{
					parent->AST_child=child;
					child->AST_parent=parent;
			
				}
				else
				{
				

					while(temp!=NULL)
					{
						temp2=temp;
						temp=temp->AST_next;
					}
					temp2->AST_next = child;
					child->AST_parent = parent;
				}	
			}
				makeAST(child,child->child);


	}
	else
	{
		makeAST(parent,child->child);
	}

		makeAST(parent,child->next);

	return;
}

int parser_main(char *filename)
{

	struct rule grammer[150];
	buildGrammer(grammer);
	int len = 96;

	struct Token *root=(struct Token*)malloc(sizeof(struct Token));

	struct rule matrix[150][150];

	struct Units first[150];

	struct Units follow[150];

	struct rule rules[400];

    first_maker(first);
	printf("First set generated\n");
    follow_maker(follow);
	printf("Follow set generated\n");

    initializeTable(matrix);
    //printf("\n-----Here-----\n");
	int i=0;

	populateTable(matrix,first,follow,grammer,len);



	pushStack(DOLLAR);
	pushStack(program);
	root=func(filename);

	parseString(root,matrix,rules);

	tree_root = (struct tree_unit*)malloc(sizeof(struct tree_unit));
		
	buildParseTree(tree_root,rules,program,-1);
	
	ptr=tree_root;

	
	


	printf("..\n %d %d \n",tree_root->val,tree_root->child->val);
	
	return 0;

}

void buildTreeFILE(char *dest){

FILE * fp=fopen(dest,"w+");
inOrderTraversal(ptr,fp);



}

