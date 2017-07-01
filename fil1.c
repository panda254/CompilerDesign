/*
Lexical Analyzer: This module takes as input the file containing user source code in the given language and produces the tokens. The lexical analyzer module scans the input only once and collects all relevant information required by the other modules of compiler. The lexical analyzer ignores comments and white spaces, while recognizes the useful lexemes as valid tokens The lexical errors are reported by this module when it sees any symbol or pattern not belonging to the language. Your lexical analyzer must
1. Tokenize lexemes appropriately
2. Maintain all information collected during a single pass of the source code
3. Be efficient with respect to time and space complexity
4. Report lexical errors
*/





/*
ERROR_1 : Identifier at line <print the line no here>is longer than the prescribed length --8

ERROR_2: Unknown Symbol <print the symbol here> at line <line no here>

ERROR_3: Unknown pattern <print the lexeme seen so far which does not match any
pattern>

ERROR_4: Specify<Any other as per your perception>


*/

//Batch 66
//Akshit Johry 2014A7PS077P
//Arpit Panda 2014A7PS114P


#include<stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lookup.h"


struct Token{

int token_num;
int line_num;
char lexeme[100];
struct Token *next;

};



int errors=0;
typedef struct Token token;
token *list_head=NULL,*list_end=NULL;

int file1_main(char *filname, int printTokens)
{

    /*if(argc == 1){
		printf("FILE name not entered\n");
		return 0;
	}*/


	FILE *fp;
	//fp= fopen(argv[1],"r");
	fp= fopen(filname,"r");
	//printf("opened file\n");
	if(fp==NULL){
        printf("ERROR opening file\n");
    return 0;

	}


    int line=1;
    char c;
    char lex[100];

    while((c=fgetc(fp))!=EOF)
    {
        if(c==' '||c=='\t')
            continue;
       else if(c=='\n'||c=='\r'){
	c=fgetc(fp);            
	line++;
}
        else
        {

            //printf("%s\n",strings[tokenize(fp,c,lex)]);

            token *t1=(token*)malloc(sizeof(token));
		int x=tokenize(fp,c,lex);
		if(x==MORETHAN8)
        {
           printf("ERROR_1: Identifier at line %d is longer than the prescribed length --8\n",line);
errors++;
        }
        else if(x==UNKNOWN_LEXEME)
        {
            printf("ERROR_3: Unknown pattern %s \n",lex);
errors++;
        }
        else if(x==UNKNOWN_SYMBOL)
        {
            printf("ERROR_2: Unknown Symbol %s at line %d\n",lex,line);
errors++;
        }
        else if(x==-1){

            printf("ERROR_4: Comment not closed but EOF reached\n");
errors++;
        }

		else if(x!=COMMENTMARK){
            t1->token_num=x;
            //printf("%s\t",strings[tokenize(fp,c,lex)]);
           // printf("%s\n",lex);
            t1->next=NULL;
            if(list_end==NULL)
                list_end=list_head=t1;
            else{
            list_end->next=t1;
            list_end=t1;
            }
            t1->line_num=line;
            strcpy(t1->lexeme,lex);


            //printf("%s\n",stringss[t1->token_num]);
		}

		else{

            int i=0;
            for(i=0;lex[i]!='\0';i++)
            {
                if(lex[i]=='\n'||lex[i]=='\r')
                    line++;
            }


		}



        }










    }

    if(printTokens==1){

   token *t=list_head;
//printf("\n\n%s %s %d\n",strings[t1->token_num],t1->lexeme,t1->line_num);
        while(t!=NULL){

            printf("%s %s %d\n",stringss[t->token_num],t->lexeme,t->line_num);
           t=t->next;

        }
    }











    return 0;
}

token* func(char *filename){
file1_main(filename,0);
if(errors==0)
	return list_head;
else
{
	printf("Lexical errors not rectified, cannot proceed to parsing\n");
	return NULL;
}
}

void printWithoutComments(char *filname){




	FILE *fp;
	//fp= fopen(argv[1],"r");
	fp= fopen(filname,"r");
	//printf("opened file\n");
	if(fp==NULL){
        printf("ERROR opening file\n");


	}


    int line=1;
    char c;
    char lex[100];

    while((c=fgetc(fp))!=EOF)
    {
        if(c==' '||c=='\t'){
                printf("%c",c);
            continue;
        }
       else if(c=='\n'||c=='\r'){
            printf("%c",c);
            line++;
       }
        else
        {

            //printf("%s\n",strings[tokenize(fp,c,lex)]);

            token *t1=(token*)malloc(sizeof(token));
		int x=tokenize(fp,c,lex);
		/*if(x==MORETHAN8)
        {
           printf("ERROR_1: Identifier at line %d is longer than the prescribed length --8\n",line);
errors++;
        }
        else if(x==UNKNOWN_LEXEME)
        {
            printf("ERROR_3: Unknown pattern %s \n",lex);
errors++;
        }
        else if(x==UNKNOWN_SYMBOL)
        {
            printf("ERROR_2: Unknown Symbol %s at line %d\n",lex,line);
errors++;
        }
        else if(x==-1){

            printf("ERROR_4: Comment not closed but EOF reached\n");
errors++;
        }*/

		if(x!=COMMENTMARK){

            printf("%s",lex);


            //printf("%s\n",stringss[t1->token_num]);
		}

		else{

        printf("\n");


		}



        }










    }










}

