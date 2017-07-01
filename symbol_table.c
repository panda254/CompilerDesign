        #include<stdio.h>
        #include<stdlib.h>

        #
        #include "parser1.c"


        STnode *print_head;
        STnode *print_tail;

        int hash_gen(char *str){

        int hash_val=0,i;
        for(i=0;str[i]!='\0';i++){
            hash_val+=str[i];
        }

        return (hash_val%size);


        }

        int lookup(char *str,STtable *tab,int gotoParent){

            int ins_pos=hash_gen(str);
            int found=0;

        STnode* ptr=tab->table[ins_pos];
        while(ptr!=NULL&&found==0){
            if(strcmp(ptr->lexeme,str)==0)
            {
                found++;
            }


        ptr=ptr->next_node;

        }

        if(found!=0)
                return 1;
        else if(gotoParent==1&&tab->parent!=NULL)
            return lookup(str,tab->parent,1);
        return 0;
        }





        int lookupandcompare(char *str,STtable *tab,int gotoParent,char *typer){

            int ins_pos=hash_gen(str);
            int found=0;

        STnode* ptr=tab->table[ins_pos];
        while(ptr!=NULL&&found==0){
            if(strcmp(ptr->lexeme,str)==0&&strcmp(ptr->type,typer)==0)
            {
                found++;
            }


        ptr=ptr->next_node;

        }

        if(found!=0)
                return 1;
        else if(gotoParent==1&&tab->parent!=NULL)
            return lookupandcompare(str,tab->parent,1,typer);

        return 0;




        }

        void statements_start( STtable* tab,struct tree_unit* node );
        //void statements_start( STtable* ,struct tree_unit* ); 

        /*Lookup function detects the presence of an ID by string..gotParent specifies whether we need to look for it in the parent..like in case of checking if an ID is
        already declared in the present scope or not*/

        /*void insert(char *str, STtable* tab_node,int line, char *typeof){

        int ins_pos=hash_gen(str);






        }*/

        void new_module_dec(struct tree_unit* node, STtable* tab_node){


            //int pos=hash_gen(node->nodes[2].lexeme);
            int ans=lookup(node->nodes[2].lexeme,tab_node,0);
            if(ans==1)
                printf("Multiple Declarations for %s\n",node->nodes[2].lexeme);
            else{

                 int pos=hash_gen(node->nodes[2].lexeme);
        //printf("pos is %d\n",pos);
                 STnode* ptr=(STnode*)malloc(sizeof(STnode));
                 strcpy(ptr->lexeme,node->nodes[2].lexeme);
        //  printf("%s\n",ptr->lexeme);
                 //put line number if needed....ptr->line_num=node->
                 strcpy(ptr->type,"PROC");
                 ptr->module_is_defined=0;
                 if(print_tail!=NULL)
                 print_tail->order=ptr;
                    else
                    {
                        print_head=print_tail=ptr;

                    }
                 ptr->order=NULL;
                 print_tail=ptr;

                 ptr->width=-1;
                 ptr->next_node=tab_node->table[pos];
                 tab_node->table[pos]=ptr;

                 ptr->curr_table=tab_node;
                 ptr->next_table=NULL;



            }




        }

        char* getType(struct tree_unit* node){

        if((node->nodes[0]).val==INTEGER)
        return "INTEGER";
        else if((node->nodes[0]).val==REAL)
        return "REAL";
        else if((node->nodes[0]).val==BOOLEAN)
        return "BOOLEAN";
        else{
        char *str=(char*)malloc(sizeof(char)*100);
        strcpy(str,"");
        strcat(str,"array(");
        strcat(str,((node->nodes[2]).nodes[2]).lexeme);//update if strart of rangeis expected to change
        strcat(str,", ");
        strcat(str,getType(&(node->nodes[5])));
        strcat(str,")");
        return str;
        }

        }

        int getWidth(struct tree_unit* node){


        if((node->nodes[0]).val==INTEGER)
        return 2;
        else if((node->nodes[0]).val==REAL)
        return 4;
        else if((node->nodes[0]).val==BOOLEAN)
        return 1;
        else{
        return (1+atoi(((node->nodes[2]).nodes[2]).lexeme)-atoi(((node->nodes[2]).nodes[0]).lexeme))*getWidth(&(node->nodes[5]));

        }

        }

        STnode* insert_var_into_tab(char *str,struct tree_unit* node,STtable* tab_node  ){

        STnode* ptr=(STnode*)malloc(sizeof(STnode));

         int ans=lookup(str,tab_node,0);

        if(ans==0){

        int pos=hash_gen(str);
         strcpy(ptr->lexeme,str);
             //put line number if needed....ptr->line_num=node->
        strcpy(ptr->type,getType(node));

        ptr->module_is_defined=-1;
                 if(print_tail!=NULL)
                 print_tail->order=ptr;
                    else
                    {
                        print_head=print_tail=ptr;

                    }
                 ptr->order=NULL;
                 print_tail=ptr;

                 ptr->width=getWidth(node);
                 ptr->next_node=tab_node->table[pos];
                 tab_node->table[pos]=ptr;

                 ptr->curr_table=tab_node;





        return ptr;
        }
        else{
        printf("Error!!");
        return NULL;
        }


        }


        void ioSt(STtable* tab,struct tree_unit* node){

        //mostly semantic


        }

        void decSt(STtable* tab,struct tree_unit* node){


        //return;
        struct tree_unit* temp=&(node->nodes[1]);
        struct tree_unit* type=&(node->nodes[3]);
        STnode* tt;


        tt=insert_var_into_tab((temp->nodes[0]).lexeme,type,tab);
        temp=&(temp->nodes[1]);
        while((temp->nodes[0]).val!=EMPTY){

        tt=insert_var_into_tab((temp->nodes[1]).lexeme,type,tab);
        temp=&(temp->nodes[2]);

        }



        }

        void simpSt(STtable* tab,struct tree_unit* node){


        //mostly semantic

        }

        void condSt(STtable* tab,struct tree_unit* node){


        int ans=lookup((node->nodes[2]).lexeme,tab,1);
        if(ans==0)
        printf("**Switch Variable %s not defined\n",(node->nodes[2]).lexeme);

        else{
        STnode* ptr=(STnode*)malloc(sizeof(STnode));
        int pos=hash_gen("SWITCH");
         strcpy(ptr->lexeme,"SWITCH");
             //put line number if needed....ptr->line_num=node->
        strcpy(ptr->type,"SWITCH_CASE");

        ptr->module_is_defined=-1;
                 if(print_tail!=NULL)
                 print_tail->order=ptr;
                    else
                    {
                        print_head=print_tail=ptr;

                    }
                 ptr->order=NULL;
                 print_tail=ptr;

                 ptr->width=-1;
                 ptr->next_node=tab->table[pos];
                 tab->table[pos]=ptr;

                 ptr->curr_table=tab;
            
        ptr->next_table=(STtable*)malloc(sizeof(STtable));  


            (ptr->next_table)->nest_level=(ptr->curr_table)->nest_level+1;
            
            strcpy((ptr->next_table)->name,ptr->lexeme);
            (ptr->next_table)->parent=ptr->curr_table;
            (ptr->next_table)->start_line=(node->nodes[4]).lno;
                (ptr->next_table)->end_line=(node->nodes[7]).lno;
            //statements_start(ptr->next_table,&(node->nodes[7]));
            struct tree_unit* temp=&(node->nodes[5]);
            while((temp->nodes[0]).val!=EMPTY){
            statements_start(ptr->next_table,&(temp->nodes[3]));
            temp=&(temp->nodes[6]);
            
            }
            temp=&(node->nodes[6]);
            if((temp->nodes[0]).val!=EMPTY){
            statements_start(ptr->next_table,&(temp->nodes[2]));
            //temp=&(temp->nodes[6]);
            
            }


            


        }


        }

        void iterSt(STtable* tab,struct tree_unit* node){

        if((node->nodes[0]).val==FOR){

        int ans=lookupandcompare((node->nodes[2]).lexeme,tab,1,"INTEGER");
        if(ans==0)
        printf("**Iterator %s not defined\n",(node->nodes[2]).lexeme);

        else{
        STnode* ptr=(STnode*)malloc(sizeof(STnode));
        int pos=hash_gen("FOR");
         strcpy(ptr->lexeme,"FOR");
             //put line number if needed....ptr->line_num=node->
        strcpy(ptr->type,"FOR_LOOP");

        ptr->module_is_defined=-1;
                 if(print_tail!=NULL)
                 print_tail->order=ptr;
                    else
                    {
                        print_head=print_tail=ptr;

                    }
                 ptr->order=NULL;
                 print_tail=ptr;

                 ptr->width=-1;
                 ptr->next_node=tab->table[pos];
                 tab->table[pos]=ptr;

                 ptr->curr_table=tab;
            
        ptr->next_table=(STtable*)malloc(sizeof(STtable));  


            (ptr->next_table)->nest_level=(ptr->curr_table)->nest_level+1;
            
            strcpy((ptr->next_table)->name,ptr->lexeme);
            (ptr->next_table)->parent=ptr->curr_table;
            (ptr->next_table)->start_line=(node->nodes[6]).lno;
                (ptr->next_table)->end_line=(node->nodes[8]).lno;
            statements_start(ptr->next_table,&(node->nodes[7]));




        }
        }

        else{

        //check boolean expr
        STnode* ptr=(STnode*)malloc(sizeof(STnode));
        int pos=hash_gen("WHILE");
         strcpy(ptr->lexeme,"WHILE");
             //put line number if needed....ptr->line_num=node->
        strcpy(ptr->type,"WHILE_LOOP");

        ptr->module_is_defined=-1;
                 if(print_tail!=NULL)
                 print_tail->order=ptr;
                    else
                    {
                        print_head=print_tail=ptr;

                    }
                 ptr->order=NULL;
                 print_tail=ptr;

                 ptr->width=-1;
                 ptr->next_node=tab->table[pos];
                 tab->table[pos]=ptr;

                 ptr->curr_table=tab;
            
        ptr->next_table=(STtable*)malloc(sizeof(STtable));  


            (ptr->next_table)->nest_level=(ptr->curr_table)->nest_level+1;
            
            strcpy((ptr->next_table)->name,ptr->lexeme);
            (ptr->next_table)->parent=ptr->curr_table;
            (ptr->next_table)->start_line=(node->nodes[4]).lno;
                (ptr->next_table)->end_line=(node->nodes[6]).lno;
            statements_start(ptr->next_table,&(node->nodes[5]));






        }

        }

        void statements_start( STtable* tab,struct tree_unit* node ){

            struct tree_unit* tode=&(node->nodes[0]);
            while(tode->val!=EMPTY){

                switch((tode->nodes[0]).val){
                    
                    case ioStmt:break;
                    case simpleStmt:break;
                    case declareStmt:decSt(tab,&(tode->nodes[0]));break;
                    case conditionalStmt:condSt(tab,&(tode->nodes[0]));break;
                    case iterativeStmt: iterSt(tab,&(tode->nodes[0]));break;
            
                }
            node=&(node->nodes[1]);
            tode=&(node->nodes[0]); 
            }
            
            

        }


        void new_module(struct tree_unit* node, STtable* tab_node){


            //int pos=hash_gen(node->nodes[2].lexeme);
            int ans=lookup(node->nodes[2].lexeme,tab_node,0);
        STnode* ptr;
            if(ans==1)
                {
                   int ins_pos=hash_gen(node->nodes[2].lexeme);
            int found=0;

        ptr=tab_node->table[ins_pos];
        while(ptr!=NULL&&found==0){
            if(strcmp(ptr->lexeme,node->nodes[2].lexeme)==0)
            {
                found++;
            if(strcmp(ptr->type,"PROC")==0&&ptr->module_is_defined==0){
            
                ptr->module_is_defined=1;
                ptr->next_table=(STtable*)malloc(sizeof(STtable));
        break;  
            
            }
            else    {
            printf("Error!!");//define types of errors
            return;
                }


            }


        ptr=ptr->next_node;

        }
            
            }
            else{

                 int pos=hash_gen(node->nodes[2].lexeme);
        //printf("pos is %d\n",pos);
                 ptr=(STnode*)malloc(sizeof(STnode));
                 strcpy(ptr->lexeme,node->nodes[2].lexeme);
        //  printf("%s\n",ptr->lexeme);
                 //put line number if needed....ptr->line_num=node->
                 strcpy(ptr->type,"PROC");
                 ptr->module_is_defined=1;
                 if(print_tail!=NULL)
                 print_tail->order=ptr;
                    else
                    {
                        print_head=print_tail=ptr;

                    }
                 ptr->order=NULL;
                 print_tail=ptr;

                 ptr->width=-1;
                 ptr->next_node=tab_node->table[pos];
                 tab_node->table[pos]=ptr;

                 ptr->curr_table=tab_node;
                 ptr->next_table=(STtable*)malloc(sizeof(STtable)); 



            }


            (ptr->next_table)->nest_level=(ptr->curr_table)->nest_level+1;
            
            strcpy((ptr->next_table)->name,ptr->lexeme);
            (ptr->next_table)->parent=ptr->curr_table;
            //inputs
            struct tree_unit *temp=&(node->nodes[7]);
            //int pp=lookup((temp->nodes[0]).lexeme,ptr->next_table,0);
            //if(pp==0){
            
            (ptr->next_table)->num_inputs=0;
            (ptr->next_table)->input[(ptr->next_table)->num_inputs++]=insert_var_into_tab((temp->nodes[0]).lexeme,&(temp->nodes[2]),ptr->next_table);
            temp=&(temp->nodes[3]);
            
            while(temp->len!=1){
            
            (ptr->next_table)->input[(ptr->next_table)->num_inputs++]=insert_var_into_tab((temp->nodes[1]).lexeme,&(temp->nodes[3]),ptr->next_table);
            temp=&(temp->nodes[4]);
            }
            
            //}

            //outputs
            temp=&(node->nodes[10]);
            
            
            (ptr->next_table)->num_outputs=0;
            if(temp->len!=1){
            temp=&(temp->nodes[2]);
            (ptr->next_table)->output[(ptr->next_table)->num_outputs++]=insert_var_into_tab((temp->nodes[0]).lexeme,&(temp->nodes[2]),ptr->next_table);
            temp=&(temp->nodes[3]);
            
            while(temp->len!=1){
            
            (ptr->next_table)->output[(ptr->next_table)->num_outputs++]=insert_var_into_tab((temp->nodes[1]).lexeme,&(temp->nodes[3]),ptr->next_table);
            temp=&(temp->nodes[4]);
            
            
            }}

            node=&(node->nodes[11]);
            
            ptr->next_table->start_line=(node->nodes[0]).lno;
        //  printf("%s\n\n",node->nodes[0].lexeme);
            ptr->next_table->end_line=(node->nodes[2]).lno;
            node=&(node->nodes[1]);//now statements

            statements_start(ptr->next_table,node);
            


            
            



        }


        void driverer(struct tree_unit* node, STtable* tab_node){

        int pos=hash_gen("DRIVER");
        STnode *ptr=(STnode*)malloc(sizeof(STnode));
        strcpy(ptr->lexeme,"*DRIVER*");
        strcpy(ptr->type,"DRIVER_PROC");
        ptr->module_is_defined=1;

        if(print_tail!=NULL)
                 print_tail->order=ptr;
                    else
                    {
                        print_head=print_tail=ptr;

                    }
                 ptr->order=NULL;
                 print_tail=ptr;

                 ptr->width=-1;
                 ptr->next_node=tab_node->table[pos];
                 tab_node->table[pos]=ptr;

                 ptr->curr_table=tab_node;
                 ptr->next_table=(STtable*)malloc(sizeof(STtable));

            (ptr->next_table)->nest_level=(ptr->curr_table)->nest_level+1;
            
            strcpy((ptr->next_table)->name,ptr->lexeme);
            (ptr->next_table)->parent=ptr->curr_table;
            //printf("---%s\n",(node->nodes[3]).lexeme);

            node=&(node->nodes[4]);
            ptr->next_table->start_line=(node->nodes[0]).lno;
        //  printf("%s\n\n",node->nodes[0].lexeme);
            ptr->next_table->end_line=(node->nodes[2]).lno;
            node=&(node->nodes[1]);//now statements

            statements_start(ptr->next_table,node);
            



        }




        void STgenerator(struct tree_unit* node, STtable* tab_node){

        if(node==NULL)
                return;
        //printf("%s  %d*\n",node->lexeme,node->val);

        int i=0;
        switch(node->val){

            case EMPTY:return; 
            case moduleDeclaration:new_module_dec(node,tab_node);break;
            case module:new_module(node,tab_node);break;
            case driverModule:driverer(node,tab_node);break;
            




        };



        int j=0;
        for(j=0;j<node->len;j++){

            STgenerator(&node->nodes[j],tab_node);
        }






        }


        int main(){

        //here we will get the root of parse tree
        //parse_node* parse_root=getroot();
        parser_main("testcase1.txt");
        buildTreeFILE("outing.txt");
        STtable *st_root=(STtable*)malloc(sizeof(STtable));
        strcpy(st_root->name,"program");
        st_root->parent=NULL;
        st_root->nest_level=0;
        print_head=print_tail=NULL; 


        STgenerator(tree_root,st_root);

        while(print_head!=NULL){

        printf("%s %s %s (%d to %d) %d %d\n",print_head->lexeme,print_head->type,print_head->curr_table->name,print_head->curr_table->start_line,print_head->curr_table->end_line,print_head->width,print_head->curr_table->nest_level);
        print_head=print_head->order;


        }
        makeAST(tree_root,tree_root->child);
        printAST(tree_root);
        return 0;

        }
