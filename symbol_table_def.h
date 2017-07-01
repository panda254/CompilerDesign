	//this contains all structures and global variables
#define size 47
//#define max_inputs 100
struct STNode{

char lexeme[100];
int line_num;
char type[100];
//char *scope if needed
int module_is_defined;//if the function is "defined" only then it is turned to true
/*if declared first and defined later only in that case ok else error */

struct STNode* order;//to have nodes printed out in sequential order

int width;//sizeof identifier

struct STNode* next_node;//for chaining at collision

struct STTable* next_table;//points to next table if nesting occurs
struct STTable* curr_table;//points to current table of a node

};



struct STTable{

int start_line;//position of start
int end_line;//position of end

int nest_level;//scope (nesting level)

struct STNode *input[100];//input list of module
int num_inputs;
int num_outputs;
struct STNode *output[100];//output list of module

char name[100];//name of implementing module

struct STTable *parent;//parent
struct STNode *table[size];


};


typedef struct STNode STnode;
typedef struct STTable STtable;


