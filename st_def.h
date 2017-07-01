#include <stdio.h>
#include <stdlib.h>


#define SIZE 97

int off_track;
int error_flag;
//int flag;
typedef struct STNode 
{	
	char lexeme[100];
	char type[100];
	char scope[100];

	int no_of_elements;

	int width;


	int array_low;
	int array_high;

	int isiterator;

	int isassigned;

	int offset;

	int isArray;

	struct STScope *STNext;

	struct STNode *next;

	int module_is_defined;
}STNode;

typedef struct STScope
{
	char lexeme[100];
	int nesting_depth;

	int isFunction;

	struct STScope *parent;

	char parent_lexeme[100];

	char iterator[100];

	struct STScope *child_scope;

	struct STScope *next_scope;

	struct STNode *head;

	int start_lno;

	int end_lno;

	int count;
}STScope;

typedef struct node
{
	char lexeme[100];
	char type[100];

	int isassigned;

	int isiterator;

	struct STScope *scope;

	int width;

	int offset;

	int isArray;

	int no_of_elements;

	struct node *next;
	struct node *next_lexeme;
}node;

typedef struct node_list
{
	struct node *head;
	int list_count;
}node_list;

struct STScope global_scope[SIZE];


struct node_list out_table[SIZE];
struct node_list in_table[SIZE];
