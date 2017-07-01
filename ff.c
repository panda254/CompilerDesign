//Batch 66
//Akshit Johry 2014A7PS077P
//Arpit Panda 2014A7PS114P


#include "fil1.c"

struct Units
	{
		int len;
		int isEmpty;
		int isDollar;
		int u_arr[300];
	};


typedef struct Units units;
int find_num(char *word){
int i;
for(i=0;i<=112;i++)
    {
        if(strcmp(word,stringss[i])==0)
            return i;
    }

    return -1;


}


void first_maker(units *first)
{

    FILE *fp=fopen("fil_first.txt","r");
    if(fp==NULL)
        return;
    char buffer[200];
  int i;
  char *word;
while(fgets(buffer,200,fp)!=NULL)
{

	
    int l=strlen(buffer);
   buffer[l-2]='\0';
//printf("%s",buffer);
    //printf("%s\n\n",buffer);
    word=strtok(buffer," ");

    //printf("%s--->",word);
    int token=find_num(word);
    first[token].isEmpty=0;
    first[token].isDollar=0;
     //printf("%s %d\n",word,token);
    int ctr=0;
    word=strtok(NULL," ");
    //printf("%s\n",word);
    while(word!=NULL)
    {

        (first[token]).u_arr[ctr++]=find_num(word);
        if(strcmp("EMPTY",word)==0)
            first[token].isEmpty=1;
        if(strcmp("DOLLAR",word)==0)
            first[token].isDollar=1;
         //printf("%s ** %d  ",word,find_num(word));
   //
    word=strtok(NULL," ");


    }
   //printf("***********\n");
    first[token].len=ctr;

   // int j;
    //int i=token;
   //printf("%s -> {",stringss[i]);
     //   for( j=0;j<first[i].len;j++)
       //     printf("%s ",stringss[(first[i]).u_arr[j]]);
        //printf("  }\n");


}




    fclose(fp);
}


void follow_maker(units *follow)
{


    FILE *fp=fopen("fil_follow.txt","r");
    if(fp==NULL)
        return;
    char buffer[200];
  int i;
  char *word;
while(fgets(buffer,200,fp)!=NULL)
{

//printf("%s\n",buffer);
    int l=strlen(buffer);
    buffer[l-2]='\0';
    //printf("%s %d\n",buffer,l);
    word=strtok(buffer," ");

    //printf("%s--->",word);
    int token=find_num(word);
     //printf("%s %d\n",word,token);
    int ctr=0;
    word=strtok(NULL," ");
    //printf("%s\n",word);
    while(word!=NULL)
    {

        (follow[token]).u_arr[ctr++]=find_num(word);
        if(strcmp("EMPTY",word)==0)
            follow[token].isEmpty=1;
        if(strcmp("DOLLAR",word)==0)
            follow[token].isDollar=1;
         //printf("%s ** %d  ",word,find_num(word));
   //
    word=strtok(NULL," ");


    }
   //printf("***********\n");
    follow[token].len=ctr;

   // int j;
    //int i=token;
   //printf("%s -> {",stringss[i]);
     //   for( j=0;j<first[i].len;j++)
       //     printf("%s ",stringss[(first[i]).u_arr[j]]);
        //printf("  }\n");


}




    fclose(fp);
}



int ff_main()
{
    units first[150];//=(units *)malloc(sizeof(units)*115);
    units follow[150];//=(units *)malloc(sizeof(units)*115);
    first_maker(first);
    follow_maker(follow);

    printf("First SET:\n");
    int i,j;
    //printf("%d %s ",find_num("arithmeticOrBooleanExpr"),stringss[109] );
    for( i=0;i<=112;i++)
    {
        printf("%s -> {",stringss[i]);
        for( j=0;j<first[i].len;j++)
            printf(" %s",stringss[first[i].u_arr[j]]);
        printf("}\n");


    }


     /*printf("Follow SET:\n");
    int i,j;
    printf("%d %s ",find_num("arithmeticOrBooleanExpr"),stringss[109] );
    for( i=62;i<=112;i++)
    {
        printf("%s -> {",stringss[i]);
        for( j=0;j<follow[i].len;j++)
            printf(" %s ",stringss[follow[i].u_arr[j]]);
        printf("}\n");


    }
   // printf("%d",find_num("SQBC"));*/





    return 0;



}
