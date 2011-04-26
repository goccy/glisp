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
#define MAX_STACK_SIZE 128

#ifdef DEBUG_MODE
#define DBG_P(fmt, ...) {\
	fprintf(stderr, fmt, ## __VA_ARGS__);	\
	fprintf(stderr, "\n");						\
	}											
#define DBG_PL(fmt, ...) {\
	fprintf(stderr, fmt, ## __VA_ARGS__);		\
	}

#else
#define DBG_P(fmt, ...) {						\
	}
#define DBG_PL(fmt, ...) {						\
	}
#endif

typedef struct _GString {
	char *str;
	size_t len;
} GString;

typedef struct _GStringArray {
	GString **a;
} GStringArray;

typedef struct _Conscell {
	union {
		struct _Conscell *car;
		char *string;
		int num;
	};
	struct _Conscell *cdr;
	int type;
	/*FOR cannot insert result number to car's ptr
	  because I puzzled when union's cell free.*/
	int result;//deprecated
	void (*printTypeName)(struct _Conscell *c);
} Conscell;

typedef struct _GMap {
	const char *key;
	void *value;
} GMap;

typedef struct _Tokenizer {
	char **(*split)(char *line);
	void (*delete)(char **token);
} Tokenizer;

typedef struct _Parser {
	Conscell *(*parse)(char **token);
	void (*delete)(Conscell *root);
} Parser;

typedef struct _HashTable {
	char *key;
	int value;
	struct _HashTable *next;
} HashTable;

typedef struct _FuncTable {
	char *func_name;
	char **args;
	Conscell *root;
	struct _FuncTable *next;
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
	/* FAST CALL INST */
	OPiADDC,
	OPiSUBC,
	OPiJLC,
	OPiJGC,
	OPFASTCALL,
	OPiPUSHC,
	OPTHCODE,
	OPNOP,
} OpCode;

struct VirtualMachineCodeAPI;
typedef struct _VirtualMachineCode {
	OpCode op; /* operation code */
	int dst;   /* register number */
	int src;   /* src value or register number */
	int jmp;   /* jmp register number */
	const char *name; /* variable or function name */
	const char **args; /* function's args name set */
	void *opnext; /* for direct threading */
	struct VirtualMachineCodeAPI *api; /* methods */
} VirtualMachineCode;

typedef struct VirtualMachineCodeAPI {
	void (*dump)(struct _VirtualMachineCode *vmcode);
	void (*delete)(struct _VirtualMachineCode *vmcode);
} VirtualMachineCodeAPI;

typedef struct _VirtualMachineCodeArray {
	VirtualMachineCode **a; /* original array */
	size_t size; /* current vmcode size */
	void (*add)(struct _VirtualMachineCodeArray *array, VirtualMachineCode *code);
	struct _VirtualMachineCodeArray *(*copy)(struct _VirtualMachineCodeArray *, int base_offset);
	void (*dump)(struct _VirtualMachineCodeArray *array);
	void (*remove)(struct _VirtualMachineCodeArray *array, int num);
} VirtualMachineCodeArray;

typedef struct _Compiler {
	VirtualMachineCodeArray *vmcodes;
	int isExecFlag;
	VirtualMachineCodeArray *(*compile)(struct _Compiler *, Conscell *conscell);
	void (*compileToFastCode)(VirtualMachineCodeArray *vmcode);
} Compiler;

typedef struct _VirtualMachine {
	int (*run)(VirtualMachineCodeArray *vmcode);
	void (*setVariable)(VirtualMachineCodeArray *vmcode, int var);
	void (*setFunction)(VirtualMachineCodeArray *vmcode);
	void (*clear)(void);
} VirtualMachine;

typedef enum {
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
} CellType;

void glisp_start_shell(void);
void glisp_start_script(char *file_name);
Tokenizer *new_Tokenizer(void);
Parser *new_Parser(void);
Compiler *new_Compiler(void);
VirtualMachineCode *new_VirtualMachineCode(Conscell *c, int base_count);
VirtualMachineCodeArray *new_VirtualMachineCodeArray(void);
VirtualMachine *new_VirtualMachine(void);


Conscell* eval(Conscell *root);

HashTable* new_HashTable(void);
int getValFromHashTable(char *key);
void setToHashTable(char *key, int value);
HashTable* searchHashTable(char *key);

FuncTable* new_FuncTable(void);
FuncTable* searchFuncTable(char* func_name);
void setToFuncTable(char *func_name, char **args, Conscell *root);
FuncTable* getFuncTable(char *func_name);

void deleteHashTable(void);
void deleteFuncTable(Parser *p);
void deleteKeyFromHashTable(char *key);

void* gmalloc(size_t size);
int gmatch(const char *i, const char *you);
/*Global Variable*/
HashTable *hash_table;
HashTable *search_htop;
FuncTable *func_table;
FuncTable *search_ftop;
FuncTable *func_ptr;
