#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>
#define true 1
#define false 0
#define EOL '\0'
#define MAX_ARGS 16
#define MAX_LINE_SIZE 128
#define MAX_BRANCH_SIZE 32

typedef struct cons_t {
	union {
		struct cons_t *car;
		char *string;
		int num;
	};
	struct cons_t *cdr;
	int type;
	/*FOR cannot insert result number to car's ptr
	  because I puzzled when union's cell free.*/
	int result;//deprecated
} Conscell;

typedef struct hash_t {
	char *key;
	int value;
	struct hash_t *next;
} HashTable;

typedef struct map_t {
	char *key;
	void *value;
	
} GMap;

typedef struct func_t {
	char *func_name;
	char **args;
	Conscell *root;
	struct func_t *next;
} FuncTable;

typedef enum {
	OPMOV,
	OPADD,
	OPSUB,
	OPMUL,
	OPDIV,
	OPCALL,
	OPJMP,
	OPCMP,
	OPPOP,
	OPPUSH,
	OPRET,
	OPJL,
	OPJG,
	OPSTORE,
	OPLOAD,
} OpCode;

typedef struct _VirtualMachineCode {
	OpCode op; /* operation code */
	int dst;   /* register number */
	int src;   /* src value or register number */
	const char *name; /* variable or function name */
	const char **args; /* function's args name set */
	//int hash_num;
	void (*dump)(struct _VirtualMachineCode *vmcode);
} VirtualMachineCode;

typedef struct compiler_t {
	
	VirtualMachineCode **(*compile)(Conscell *conscell);
} Compiler;

typedef struct vm_t {
	
	void (*run)(VirtualMachineCode **vmcode);
} VirtualMachine;

enum CELL_TYPE {
	ADD,
	SUB,
	MULTI,
	DIV,
	GRATER,
	LESS,
	EQUAL,
	GRATER_EQUAL,
	LESS_EQUAL,
	IF,
	SETQ,
	DEFUN,
	STRING,
	NUM,
	LEFT_PARENTHESIS,
	RIGHT_PARENTHESIS,
	//For Result Status
	BOOL,//prev cell type is num && return type bool
	STR_BOOL,//prev cell type is string && return type bool
	FUNC,
	FUNC_NAME,
	FUNC_ARGS,
};

void init(void);
void initTable(void);
void startGlispShell(void);
void startGlispForFile(char *file_name);
int checkBrace(char *line);
void displayResult(Conscell *ans);

char** tokenizer(char *line);

Conscell* new_Conscell(void);
Conscell* parse(char **token);

Conscell* eval(Conscell *root);

HashTable* new_HashTable(void);
int getValFromHashTable(char *key);
void setToHashTable(char *key, int value);
HashTable* searchHashTable(char *key);

FuncTable* new_FuncTable(void);
FuncTable* searchFuncTable(char* func_name);
void setToFuncTable(char *func_name, char **args, Conscell *root);
FuncTable* getFuncTable(char *func_name);

void deleteToken(char **token);
void deleteTree(Conscell *root);
void deleteHashTable(void);
void deleteFuncTable(void);
void deleteKeyFromHashTable(char *key);

void* gmalloc(size_t size);

Compiler *new_Compiler(void);
VirtualMachine *new_VirtualMachine(void);

/*Global Variable*/
HashTable *hash_table;
HashTable *search_htop;
FuncTable *func_table;
FuncTable *search_ftop;
FuncTable *func_ptr;
