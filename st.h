#include <stdio.h>
#include <stdlib.h>

#include "st_def.h"


void printGlobalTable();
void initializeGlobalScope(struct tree_unit *AST);
void addToGlobalScope(struct tree_unit *child);
char* ID_type(struct tree_unit *AST,struct STScope *scope);
struct STNode* findLexemeNode(struct tree_unit *AST,struct STScope *scope,int gotToParent);
struct STNode* findLexemeNodeFromLexeme(char *lexeme,struct STScope *scope,int goToParent);
int findLexeme(struct tree_unit *AST,struct STScope *scope,int goToParent);
int hash_gen(char *str);
void check_arithmetic(struct tree_unit *AST,char *type_left,struct STScope *scope,int typecheck);
void check_boolean(struct tree_unit *AST,struct STScope *scope);
int arithmeticOrBoolean(struct tree_unit *AST);
void printScope(struct STScope *scope);
void printGlobalTable();
struct STNode* create_st_node(char *lexeme,char *type,char *scope,int offset,struct STNode *next,int isDefined,int no_of_elements);
void addToGlobalScope(struct tree_unit *child);
void initializeGlobalScope(struct tree_unit *AST);
struct STScope* GenerateScope(char *lexeme,struct tree_unit *AST,struct STScope *scope,int offset);
struct STScope* initializeScope(int size,char *lexeme,int nesting_depth,char *parent_lexeme,int isFunction,struct STScope *parent);
void create_symbol_table(struct tree_unit *AST);
int out_func_par_error_check(struct tree_unit *AST,struct STScope *scope);
int func_par_error_check(struct tree_unit *AST,struct STScope *scope);
void initializeFunctionTable(struct tree_unit *AST);
int outputAssinged(struct tree_unit *AST);