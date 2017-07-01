//Batch 66
//Akshit Johry 2014A7PS077P
//Arpit Panda 2014A7PS114P

#include<string.h>
#include "tables.h"

int isletter(char c)
{
    if((c>='a'&&c<='z')||(c>='A'&&c<='Z'))
    return 1;
    return 0;
}
int isletter_num(char c)
{

    if((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9')||(c=='_'))
    {
        return 1;
    }
    return 0;



}

int isnum(char c)
{

    if(c>='0'&&c<='9')
        return 1;
    return 0;

}

//num


int tokenize(FILE *fp, char c,char lexeme[100])
{

int ctr=0;
    switch(c)
    {
        case ';':lexeme[ctr++]=';';lexeme[ctr]='\0';return SEMICOL;break;
        case ',':lexeme[ctr++]=',';lexeme[ctr]='\0';return COMMA;break;
        case '+':lexeme[ctr++]='+';lexeme[ctr]='\0';return PLUS;break;
        case '-':lexeme[ctr++]='-';lexeme[ctr]='\0';return MINUS;break;
        case '/':lexeme[ctr++]='/';lexeme[ctr]='\0';return DIV;break;
        case '[':lexeme[ctr++]='[';lexeme[ctr]='\0';return SQBO;break;
        case ']':lexeme[ctr++]=']';lexeme[ctr]='\0';return SQBC;break;
        case '(':lexeme[ctr++]='(';lexeme[ctr]='\0';return BO;break;
        case ')':lexeme[ctr++]=')';lexeme[ctr]='\0';return BC;break;



    };
    if(c=='=')
    {
        lexeme[ctr++]='=';
        char c2=fgetc(fp);
        if(c2=='='){
                lexeme[ctr++]='=';lexeme[ctr]='\0';
            return EQ;
        }
        else
        {
            //mein error hai
             fseek(fp,-1,SEEK_CUR);
       lexeme[ctr]='\0';
            return UNKNOWN_LEXEME;

        }
    }
    if(c=='.')
    {
         lexeme[ctr++]='.';
        char c2=fgetc(fp);
        if(c2=='.'){
               lexeme[ctr++]='.';lexeme[ctr]='\0';
            return RANGEOP;
        }
        else{
             fseek(fp,-1,SEEK_CUR);
       lexeme[ctr]='\0';
            return UNKNOWN_LEXEME;

        }
    }

    if(c==':')
    {
        lexeme[ctr++]=':';
       char c2=fgetc(fp);
        if(c2=='='){
                lexeme[ctr++]='=';lexeme[ctr]='\0';
            return ASSIGNOP;
        }
        else
        {
           fseek(fp,-1,SEEK_CUR);
       lexeme[ctr]='\0';
            return COLON;
        }
    }
    if(c=='!')
    {
        lexeme[ctr++]='!';
        char c2=fgetc(fp);
        if(c2=='='){
                 lexeme[ctr++]='=';lexeme[ctr]='\0';
            return NE;
        }
        else{

            fseek(fp,-1,SEEK_CUR);
       lexeme[ctr]='\0';
            return UNKNOWN_LEXEME;
        }
    }
    if(c=='<')
    {
        lexeme[ctr++]='<';
        char c2=fgetc(fp);
        if(c2=='='){
                lexeme[ctr++]='=';lexeme[ctr]='\0';
            return LE;
        }
        else if(c2=='<'){
                lexeme[ctr++]='<';
                char c3=fgetc(fp);
                if(c3=='<'){
                    lexeme[ctr++]='<';
                    lexeme[ctr]='\0';
                    return DRIVERDEF;
                }
                else{
                lexeme[ctr]='\0';
        fseek(fp,-1,SEEK_CUR);
            return DEF;
                }
        }
        else
        {
            fseek(fp,-1,SEEK_CUR);
        lexeme[ctr]='\0';
            return LT;
        }

    }
    if(c=='>')
    {
        lexeme[ctr++]='>';
        char c2=fgetc(fp);
        if(c2=='='){
                lexeme[ctr++]='=';lexeme[ctr]='\0';
            return GE;
        }
        else if(c2=='>'){

                lexeme[ctr++]='>';
                char c3=fgetc(fp);
                if(c3=='>'){
                    lexeme[ctr++]='>';
                    lexeme[ctr]='\0';
                    return DRIVERENDDEF;
                }
                else{
                   lexeme[ctr]='\0';
                   fseek(fp,-1,SEEK_CUR);
            return ENDDEF;
                }
        }
        else
        {
            fseek(fp,-1,SEEK_CUR);
        lexeme[ctr]='\0';
            return GT;
        }

    }

    if(c=='*')
    {

       // int len=0;
       //take error line for comments into consideration

       char c2=fgetc(fp);
       if(c2=='*')
       {
           //lexeme[ctr++]='*';lexeme[ctr]='\0';
           char c3=fgetc(fp);
           char c4=fgetc(fp);
           while(!(c3=='*'&&c4=='*'))
           {
               if(c3=='\n'||c3=='\r')
                lexeme[ctr++]='\n';
               c3=c4;
               c4=fgetc(fp);
               if(c4==EOF)
               return   -1;

           }
           lexeme[ctr]='\0';

            return COMMENTMARK;

       }

       else
       {

           fseek(fp,-1,SEEK_CUR);
        lexeme[ctr++]='*';lexeme[ctr]='\0';
            return MUL;
       }



    }


    if(isletter(c)==1)
    {

        char c2=fgetc(fp);
        char str[20];
        str[0]=c;

        int len=1;
        while(isletter_num(c2)==1&&c2!=EOF)
        {
            str[len]=c2;
            len++;

            c2=fgetc(fp);

        }
        str[len]='\0';
        strcpy(lexeme,str);
        fseek(fp,-1,SEEK_CUR);
        int i;
        for(i=21;i<=50;i++)
        {
            if(strcmp(str,strings[i])==0)
                return i;
        }

        if(len<=8)
            return ID;
          else
                return MORETHAN8;



    }

    if(isnum(c)==1)
    {

        while(isnum(c)==1)
        {
            lexeme[ctr++]=c;
            c=fgetc(fp);
        }
        if(c!='.')
        {
            fseek(fp,-1,SEEK_CUR);
            lexeme[ctr]='\0';
            return NUM;
        }
        else
        {
            lexeme[ctr++]='.';
            c=fgetc(fp);
            if(isnum(c)==1)
            {
               while(isnum(c)==1)
                {
                    lexeme[ctr++]=c;
                    c=fgetc(fp);
                }
                if(c=='e'||c=='E')
                {
                    lexeme[ctr++]=c;
                    c=fgetc(fp);
                    if(c=='+'||c=='-')
                    {
                       lexeme[ctr++]=c;
                      c=fgetc(fp);

                      if(isnum(c)==1)
                {
                    while(isnum(c)==1){
                            lexeme[ctr++]=c;
                    c=fgetc(fp);
                    }

                    fseek(fp,-1,SEEK_CUR);
                    lexeme[ctr]='\0';
                return RNUM;

                }
                else
                    //123.345e+---error
                    return UNKNOWN_LEXEME;

                    }
                    else if(isnum(c)==1)
                    {

                        while(isnum(c)==1)
                {
                    lexeme[ctr++]=c;
                    c=fgetc(fp);
                }
                fseek(fp,-1,SEEK_CUR);
                lexeme[ctr]='\0';
                return RNUM;


                    }
                }
                else
                {
                        fseek(fp,-1,SEEK_CUR);
                        lexeme[ctr]='\0';
                return RNUM;
                }
            }
            else if(c=='.')//rangeop
            {
                lexeme[ctr++]='.';
                fseek(fp,-2,SEEK_CUR);
                lexeme[ctr]='\0';
                return NUM;

            }
            else
            {
                //error---123.
                return UNKNOWN_LEXEME;


            }



        }

        lexeme[ctr++]=c;
        lexeme[ctr]='\0';
        return UNKNOWN_SYMBOL;






    }

return -1;

}