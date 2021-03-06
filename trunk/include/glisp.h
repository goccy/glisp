#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <greadline.h>
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
	int type;
} GMap;

typedef struct _Tokenizer {
	char **(*split)(char *line);
	void (*free)(char **token);
} Tokenizer;

typedef struct _Parser {
	Conscell *(*parse)(char **token);
	void (*free)(Conscell *root);
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
	OPPFASTCALL,
	OPiPFASTCALL,
	OPiPUSHC,
	OPCOPY,
	OPTHCODE,
	OPNOP,
	/* FINAL FAST CALL INST */
	OPABADD,
	OPACADD,
	OPADADD,
	OPBCADD,
	OPBDADD,
	OPCDADD,
	OPAiADDC,
	OPBiADDC,
	OPCiADDC,
	OPDiADDC,
	OPAiSUBC,
	OPBiSUBC,
	OPCiSUBC,
	OPDiSUBC,
	OPAPOP,
	OPBPOP,
	OPCPOP,
	OPDPOP,
	OPAPUSH,
	OPBPUSH,
	OPCPUSH,
	OPDPUSH,
	OPAiPUSHC,
	OPBiPUSHC,
	OPCiPUSHC,
	OPDiPUSHC,
	OPACOPY,
	OPBCOPY,
	OPCCOPY,
	OPDCOPY,
	OPAiJLC,
	OPBiJLC,
	OPCiJLC,
	OPDiJLC,
	OPAiJGC,
	OPBiJGC,
	OPCiJGC,
	OPDiJGC,
	OPAFASTCALL,
	OPBFASTCALL,
	OPCFASTCALL,
	OPDFASTCALL,
	OPAPFASTCALL,
	OPBPFASTCALL,
	OPCPFASTCALL,
	OPDPFASTCALL,
	OPARET,
	OPBRET,
	OPCRET,
	OPDRET,
	OPAMOV,
	OPBMOV,
	OPCMOV,
	OPDMOV,
	GOTO_BLOCK,
	RET_BLOCK,
} OpCode;

struct VirtualMachineCodeAPI;
typedef struct _VirtualMachineCode {
	OpCode op; /* operation code */
	int dst;   /* register number */
	int src;   /* src value or register number */
	union {
		void (*code)(void);/* selective inlining code */
		int jmp;   /* jmp register number */
	};
	const char *name; /* variable or function name */
	const char **args; /* function's args name set */
	void *opnext; /* for direct threading */
	struct VirtualMachineCodeAPI *api; /* methods */
} VirtualMachineCode;

typedef struct VirtualMachineCodeAPI {
	void (*dump)(struct _VirtualMachineCode *vmcode);
	void (*free)(struct _VirtualMachineCode *vmcode);
} VirtualMachineCodeAPI;

typedef struct _VirtualMachineCodeArray {
	VirtualMachineCode **a; /* original array */
	size_t size; /* current vmcode size */
	void (*add)(struct _VirtualMachineCodeArray *array, VirtualMachineCode *code);
	struct _VirtualMachineCodeArray *(*copy)(struct _VirtualMachineCodeArray *, int base_offset);
	void (*dump)(struct _VirtualMachineCodeArray *array);
	void (*remove)(struct _VirtualMachineCodeArray *array, int num);
	void (*reverse)(struct _VirtualMachineCodeArray *array);
	VirtualMachineCode *(*getPureCode)(struct _VirtualMachineCodeArray *array);
	void (*free)(struct _VirtualMachineCodeArray *array);
} VirtualMachineCodeArray;

typedef struct _Compiler {
	VirtualMachineCodeArray *vmcodes;
	int isExecFlag;
	VirtualMachineCodeArray *(*compile)(struct _Compiler *, Conscell *conscell);
	void (*compileToFastCode)(VirtualMachineCodeArray *vmcode);
	void (*fixRegNumber)(VirtualMachineCodeArray *vmcode);
	int (*getMaxRegNumber)(VirtualMachineCodeArray *vmcode);
	void (*finalCompile)(VirtualMachineCodeArray *vmcode);
	void (*free)(struct _Compiler *c);
} Compiler;

typedef struct _VirtualMachine {
	int (*run)(VirtualMachineCode *vmcode);
	void (*setVariable)(VirtualMachineCode *vmcode, int size, int var);
	void (*setFunction)(VirtualMachineCode *vmcode, int size);
	void (*clear)(void);
	void (*free)(struct _VirtualMachine *vm);
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
void* gmalloc(size_t size);
int gmatch(const char *i, const char *you);
void clear_virtual_memory(void);

Tokenizer *new_Tokenizer(void);
Parser *new_Parser(void);
Compiler *new_Compiler(void);
VirtualMachineCode *new_VirtualMachineCode(Conscell *c, int base_count);
VirtualMachineCodeArray *new_VirtualMachineCodeArray(void);
VirtualMachine *new_VirtualMachine(void);

