	#include <stdio.h>
	#include <stdlib.h>

		struct Token2

		{

		int parent;
		int token_num;
		int line_num;
		char lexeme[100];

		};

		struct Token2 tokens[10000];

		int token2_len;
		int token2_len2;
		
		struct tree_unit *tree_root; 

	struct rule
		{
			int len;
			int left; // enum NonTerm left;
			int r_arr[20];
		};
	struct stackNode
		{
			int ui;
			struct stackNode *next;
		};

	 struct tree_unit
	 {
		char lexeme[100];
		int lno;
		int val;
		int len;
		int parent;

		int isassigned;
		
		struct tree_unit *node_parent;
		struct tree_unit *nodes;

		struct tree_unit *child; // Added just to make creation of AST Code simple
		struct tree_unit *next;  // Added just to make creation of AST Code simple

		// Information for the ast incorporated in the parse tree itself

		int isLeaf;
		struct tree_unit *AST_parent;
		struct tree_unit *AST_child;
		struct tree_unit *AST_next;

		struct STNode *ST_entry;
	 };

