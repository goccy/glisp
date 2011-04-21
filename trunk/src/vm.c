#include <glisp.h>
#define MAX_STACK_SIZE 128

static void printTypeName(Conscell *cell)
{
	switch (cell->type) {
	case ADD:
		fprintf(stderr, "ADD\n");
		break;
	case SUB:
		fprintf(stderr, "SUB\n");
		break;
	case MULTI:
		fprintf(stderr, "MULTI\n");
		break;
	case DIV:
		fprintf(stderr, "DIV\n");
		break;
	case GRATER:
		fprintf(stderr, "GRATER\n");
		break;
	case LESS:
		fprintf(stderr, "LESS\n");
		break;
	case EQUAL:
		fprintf(stderr, "EQUAL\n");
		break;
	case IF:
		fprintf(stderr, "IF\n");
		break;
	case DEFUN:
		fprintf(stderr, "DEFUN\n");
		break;
	case SETQ:
		fprintf(stderr, "SETQ\n");
		break;
	case STRING:
		fprintf(stderr, "STRING : %s\n", cell->string);
		break;
	case NUM:
		fprintf(stderr, "NUM : %d\n", cell->num);
		break;
	case LEFT_PARENTHESIS:
		fprintf(stderr, "LEFT_PARENTHESIS\n");
		break;
	case RIGHT_PARENTHESIS:
		fprintf(stderr, "RIGHT_PARENTHESIS\n");
		break;
	case FUNC:
		fprintf(stderr, "FUNC\n");
		break;
	case FUNC_NAME:
		fprintf(stderr, "FUNC_NAME\n");
		break;
	case FUNC_ARGS:
		fprintf(stderr, "FUNC_ARGS\n");
		break;
	default:
		break;
	}
}

static void VirtualMachineCode_dump(VirtualMachineCode *code)
{
	switch (code->op) {
	case OPADD:
		fprintf(stderr, "OPADD : ");
		break;
	case OPSUB:
		fprintf(stderr, "OPSUB : ");
		break;
	case OPMUL:
		fprintf(stderr, "OPMUL : ");
		break;
	case OPDIV:
		fprintf(stderr, "OPDIV : ");
		break;
	case OPMOV:
		fprintf(stderr, "OPMOV : ");
		break;
	case OPCMP:
		fprintf(stderr, "OPCMP : ");
		break;
	case OPJL:
		fprintf(stderr, "OPJL  : ");
		break;
	case OPJG:
		fprintf(stderr, "OPJG  : ");
		break;
	case OPSTORE:
		fprintf(stderr, "OPSTORE : ");
		fprintf(stderr, "name = %s ", code->name);
		break;
	case OPLOAD:
		fprintf(stderr, "OPLOAD : ");
		break;
	case OPPUSH:
		fprintf(stderr, "OPPUSH : ");
		break;
	case OPPOP:
		fprintf(stderr, "OPPOP : ");
		break;
	case OPCALL:
		fprintf(stderr, "OPCALL : ");
		break;
	case OPRET:
		fprintf(stderr, "OPRET : ");
		break;
	default:
		break;
	}
	fprintf(stderr, "dst = %d src = %d\n", code->dst, code->src);
}

static int stack_count = 0;
VirtualMachineCode *new_VirtualMachineCode(Conscell *c, int base_count)
{
	VirtualMachineCode *ret = (VirtualMachineCode *)gmalloc(sizeof(VirtualMachineCode));
	ret->name = NULL;
	ret->args = NULL;
	ret->dump = VirtualMachineCode_dump;
	if (c == NULL) {
		ret->op = OPRET;
		ret->dst = 0;
		ret->src = 0;
		return ret;
	}
	switch (c->type) {
	case ADD:
		ret->op = OPADD;
		ret->dst = stack_count;
		ret->src = stack_count + base_count;
		break;
	case SUB:
		ret->op = OPSUB;
		ret->dst = stack_count;
		ret->src = stack_count + base_count;
		break;
	case MULTI:
		ret->op = OPMUL;
		ret->dst = stack_count;
		ret->src = stack_count + base_count;
		break;
	case DIV:
		ret->op = OPDIV;
		ret->dst = stack_count;
		ret->src = stack_count + base_count;
		break;
	case NUM:
		ret->op = OPMOV;
		ret->dst = stack_count + base_count;
		ret->src = c->num;
		stack_count++;
		break;
	case FUNC_ARGS:
		ret->op = OPPUSH;
		ret->dst = c->num;
		ret->src = -1;
		stack_count++;
		break;
	case IF:
		ret->op = OPCMP;
		ret->dst = stack_count;
		ret->src = stack_count + base_count;
		break;
	case LESS:
		ret->op = OPJL;
		ret->dst = stack_count;
		ret->src = stack_count + base_count;
		break;
	case GRATER:
		ret->op = OPJG;
		ret->dst = stack_count;
		ret->src = stack_count + base_count;
		break;
	case DEFUN:
		if (c->cdr->cdr->car != NULL) {
			Conscell *args_cell = c->cdr->cdr->car;
			Conscell *tmp = args_cell;
			int i = 1;
			while (tmp->cdr != NULL) {
				tmp = tmp->cdr;
				i++;
			}
			const char **args = (const char **)gmalloc(sizeof(char *) * i);
			const char *arg_name = args_cell->string;
			args[0] = (char *)gmalloc(strlen(arg_name) + 1);
			strncpy((char *)args[0], arg_name, strlen(arg_name) + 1);
			i = 1;
			while (args_cell->cdr != NULL) {
				args_cell = args_cell->cdr;
				arg_name = args_cell->string;
				args[i] = (char *)gmalloc(strlen(arg_name) + 1);
				strncpy((char *)args[i], arg_name, strlen(arg_name) + 1);
				i++;
			}
			ret->args = args;
		}
		//through
	case SETQ:
		ret->op = OPSTORE;
		ret->dst = stack_count;
		ret->src = stack_count + base_count;
		Conscell *variable_cell = c->cdr;
		char *variable_name = variable_cell->string;
		size_t variable_name_length = strlen(variable_name) + 1;
		ret->name = (char *)gmalloc(variable_name_length);
		strncpy((char *)ret->name, variable_name, variable_name_length);
		break;
	case STRING:
		ret->op = OPLOAD;
		ret->dst = stack_count;
		ret->src = -1; //unused parameter
		variable_name = c->string;
		variable_name_length = strlen(variable_name) + 1;
		ret->name = (char *)gmalloc(variable_name_length);
		strncpy((char *)ret->name, variable_name, variable_name_length);
		stack_count++;
		break;
	case FUNC:
		ret->op = OPCALL;
		ret->dst = stack_count;
		ret->src = -1;
		variable_name = c->string;
		variable_name_length = strlen(variable_name) + 1;
		ret->name = (char *)gmalloc(variable_name_length);
		strncpy((char *)ret->name, variable_name, variable_name_length);
		break;
	default:
		break;
	}
	return ret;
}

static VirtualMachineCode *local_func_code = NULL;
static int Compiler_opCompile(Compiler *c, Conscell *path, int isRecursive)
{
	int ret = 0;
	Conscell *root = path;
	int opcount = 0;
	while (root->cdr != NULL) {
		root = root->cdr;
		opcount++;
	}
	int create_inst_num = (isRecursive) ? opcount : opcount - 1;
	//fprintf(stderr, "opcount = [%d]\n", opcount);
	int optype = path->type;
	if (optype == ADD || optype == SUB || optype == MULTI || optype == DIV ||
		optype == IF || optype == LESS || optype == GRATER) {
		int i, n = 0;
		Conscell *tmp = path;
		tmp = tmp->cdr;
		for (i = 0; i < create_inst_num; i++) {
			if (tmp->type == LEFT_PARENTHESIS) {
				//fprintf(stderr, "Recursive Call\n");
				n = Compiler_opCompile(c, tmp->car, 1);//recursive
				ret += n;
				//fprintf(stderr, "n = [%d], i = [%d] ret = [%d]\n", n, i, ret);
				VirtualMachineCode *code = NULL;
				if (!isRecursive) {
					code = new_VirtualMachineCode(path, ret);
					code->dump(code);
					c->vmcodes->add(c->vmcodes, code);
					//vmcodes[vm_count] = code;
					//vm_count++;
				}
			} else {
				ret++;
				//fprintf(stderr, "ret = [%d]\n", ret);
				//fprintf(stderr, "Value Cell\n");
				VirtualMachineCode *code = NULL;
				if (!isRecursive) {
					code = new_VirtualMachineCode(path, ret);
					code->dump(code);
					c->vmcodes->add(c->vmcodes, code);
					//vmcodes[vm_count] = code;
					//vm_count++;
				}
			}
			tmp = tmp->cdr;
		}
	} else if (optype == SETQ || optype == DEFUN || optype == FUNC) {
		VirtualMachineCode *code = new_VirtualMachineCode(path, 0);
		code->dump(code);
		c->vmcodes->add(c->vmcodes, code);
		//vmcodes[vm_count] = code;
		//vm_count++;
		if (optype == DEFUN) {
			local_func_code = code;
		}
	} else {
		while (path->cdr != NULL) {
			path = path->cdr;
			ret++;
		}
	}
	return ret;
}

static VirtualMachineCodeArray *Compiler_compile(Compiler *compiler, Conscell *path)
{
	//asm("int3");
	fprintf(stderr, "---init---\n");
	Conscell *tmp = NULL;
	while (path->car != NULL && path->type != FUNC) {
		path = path->car;
		if (path->cdr == NULL) break;
		fprintf(stderr, "opcode = ");
		printTypeName(path);
		puts("=== create calculate operation code ===");
		Compiler_opCompile(compiler, path, 0);
		puts("=======================================");
		tmp = path;
		while (tmp->cdr != NULL) {
			if (tmp->type == SETQ || tmp->type == DEFUN) tmp = tmp->cdr;//skip string cell
			tmp = tmp->cdr;
			if (tmp->type != LEFT_PARENTHESIS) {
				printTypeName(tmp);
				VirtualMachineCode *c = new_VirtualMachineCode(tmp, 0);
				c->dump(c);
				compiler->vmcodes->add(compiler->vmcodes, c);
				//vmcodes[vm_count] = c;
				//vm_count++;
			} else {
				Compiler_compile(compiler, tmp);//recursive call
			}
		}
	}
	fprintf(stderr, "---end---\n");
	return compiler->vmcodes;
}

static VirtualMachineCodeArray *new_VirtualMachineCodeArray(void);
static void VirtualMachineCodeArray_add(VirtualMachineCodeArray *array, VirtualMachineCode *c)
{
	array->a[array->size] = c;
	array->size++;
}

static VirtualMachineCodeArray *VirtualMachineCodeArray_copy(VirtualMachineCodeArray *from, int base_offset)
{
	VirtualMachineCodeArray *f = from;
	f->a += base_offset;
	VirtualMachineCodeArray *ret = new_VirtualMachineCodeArray();
	size_t i = 0;
	ret->add(ret, new_VirtualMachineCode(NULL, 0));//OPRET
	for (i = 1; i < from->size - 1; i++) {
		ret->add(ret, f->a[i]);
	}
	return ret;
}

static void VirtualMachineCodeArray_dump(VirtualMachineCodeArray *array)
{
	int i = 0;
	while (array->a[i] != NULL) {
		fprintf(stderr, "L%d : ", i);
		array->a[i]->dump(array->a[i]);
		i++;
	}
}

static VirtualMachineCodeArray *new_VirtualMachineCodeArray(void)
{
	VirtualMachineCodeArray *vm_array = (VirtualMachineCodeArray *)gmalloc(sizeof(VirtualMachineCodeArray));
	vm_array->a = (VirtualMachineCode **)gmalloc(sizeof(VirtualMachineCode) * MAX_STACK_SIZE);
	vm_array->size = 0;
	vm_array->add = VirtualMachineCodeArray_add;
	vm_array->copy = VirtualMachineCodeArray_copy;
	vm_array->dump = VirtualMachineCodeArray_dump;
	return vm_array;
}

Compiler *new_Compiler(void)
{
	Compiler *ret = (Compiler *)malloc(sizeof(Compiler));
	ret->vmcodes = new_VirtualMachineCodeArray();
	ret->compile = Compiler_compile;
	VirtualMachineCode *code = new_VirtualMachineCode(NULL, 0);//OPRET
	ret->vmcodes->add(ret->vmcodes, code);
	//vmcodes[vm_count] = code;
	//vm_count++;
	return ret;
}

#define MAX_VIRTUAL_MEMORY_SIZE 32
static GMap *virtual_memory[MAX_VIRTUAL_MEMORY_SIZE] = {NULL};
static int virtual_memory_address = 0;
GMap *new_GMap(const char *key, void *value)
{
	GMap *ret = (GMap *)gmalloc(sizeof(GMap));
	ret->key = (const char *)gmalloc(strlen(key) + 1);
	strncpy((char *)ret->key, key, strlen(key) + 1);
	ret->value = value;
	fprintf(stderr, "key = [%s]\n", key);
	fprintf(stderr, "value = [%d]\n", (int)value);
	return ret;
}
static void store_to_virtual_memory(GMap *map)
{
	virtual_memory[virtual_memory_address] = map;
	virtual_memory_address++;
}

static void *fetch_from_virtual_memory(const char *key)
{
	int i = 0;
	for (i = 0; i < virtual_memory_address; i++) {
		GMap *map = virtual_memory[i];
		if (strlen(key) == strlen(map->key) &&
			!strncmp(map->key, key, strlen(key))) {
			return map->value;
		}
	}
	return NULL;
}

static int search_func_args_from_vmcode(VirtualMachineCode *c, const char *key)
{
	int ret = 0;
	int i = 0;
	while (c->args[i] != NULL) {
		const char *arg_name = c->args[i];
		if (strlen(arg_name) == strlen(key) &&
			!strncmp(arg_name, key, strlen(arg_name) + 1)) {
			ret = 1;
		}
		i++;
	}
	return ret;
}

static int function_arg_stack[MAX_STACK_SIZE] = {0};
static int arg_stack_count = 0;
static int VirtualMachine_run(VirtualMachineCodeArray *vmcode)
{
	//asm("int3");
	int stack[MAX_STACK_SIZE] = {0};
	int vm_stack_top = vmcode->size;
	//fprintf(stderr, "stack_top = [%d]\n", vm_stack_top);
	VirtualMachineCode **pc = vmcode->a + vm_stack_top - 1;
L_HEAD:
	switch ((*pc)->op) {
	case OPMOV:
		fprintf(stderr, "OPMOV\n");
		stack[(*pc)->dst] = (*pc)->src;
		pc--;
		goto L_HEAD;
	case OPADD:
		fprintf(stderr, "OPADD\n");
		stack[(*pc)->dst] += stack[(*pc)->src];
		pc--;
		goto L_HEAD;
	case OPSUB:
		fprintf(stderr, "OPSUB\n");
		stack[(*pc)->dst] -= stack[(*pc)->src];
		pc--;
		goto L_HEAD;
	case OPMUL:
		fprintf(stderr, "OPMUL\n");
		stack[(*pc)->dst] *= stack[(*pc)->src];
		pc--;
		goto L_HEAD;
	case OPDIV:
		fprintf(stderr, "OPDIV\n");
		stack[(*pc)->dst] /= stack[(*pc)->src];
		pc--;
		goto L_HEAD;
	case OPCMP:
		fprintf(stderr, "OPCMP\n");
		stack[(*pc)->dst] = stack[(*pc)->src];
		if ((*--pc)->op == OPCMP) {
			pc--;
		}
		goto L_HEAD;
	case OPJL:
		fprintf(stderr, "OPJL\n");
		if (stack[(*pc)->dst] < stack[(*pc)->src]) {
			puts("true");
			pc -= 2;
		} else {
			puts("false");
			pc--;
		}
		goto L_HEAD;
	case OPJG:
		fprintf(stderr, "OPJG\n");
		if (stack[(*pc)->dst] > stack[(*pc)->src]) {
			puts("true");
			pc -= 2;
		} else {
			puts("false");
			pc--;
		}
		goto L_HEAD;
	case OPSTORE:
		fprintf(stderr, "OPSTORE\n");
		GMap *map = NULL;
		if ((*pc)->args == NULL) {
			//variable
			map = new_GMap((*pc)->name, (void *)stack[(*pc)->src]);
		} else {
			//function
			local_func_code = *pc;
			int base_code_num = 1; //for excluding OPSTORE code
			VirtualMachineCodeArray *vmcodes = vmcode->copy(vmcode, base_code_num);
			map = new_GMap((*pc)->name, (void *)vmcodes);
		}
		store_to_virtual_memory(map);
		pc--;
		goto L_HEAD;
	case OPLOAD:
		fprintf(stderr, "OPLOAD\n");
		//load map from virtual memory
		if (local_func_code != NULL && search_func_args_from_vmcode(local_func_code, (*pc)->name)) {
			fprintf(stderr, "This variable is function's argument!!\n");
			fprintf(stderr, "convert virtual machine code from OPLOAD to OPPOP\n");
			(*pc)->op = OPPOP;
			pc--;
			goto L_HEAD;
		}
		void *value = fetch_from_virtual_memory((*pc)->name);
		if (value == NULL) {
			fprintf(stderr, "[ERROR] : undefined variable\n");
			break;
		}
		fprintf(stderr, "value = [%d]\n", (int)value);
		stack[(*pc)->dst] = (int)value;
		pc--;
		goto L_HEAD;
	case OPCALL:
		fprintf(stderr, "OPCALL\n");
		VirtualMachineCodeArray *func_vmcode = (VirtualMachineCodeArray *)fetch_from_virtual_memory((*pc)->name);
		if (func_vmcode == NULL) {
			fprintf(stderr, "[ERROR] : undefined function name %s\n", (*pc)->name);
			break;
		}
		fprintf(stderr, "func_vmcode = [%d]\n", (int)func_vmcode);
		int res = VirtualMachine_run(func_vmcode);
		stack[(*pc)->dst] = res;
		pc--;
		goto L_HEAD;
	case OPPUSH:
		fprintf(stderr, "OPPUSH\n");
		fprintf(stderr, "%d\n", (*pc)->dst);
		function_arg_stack[arg_stack_count] = (*pc)->dst;
		fprintf(stderr, "push value = [%d]\n", function_arg_stack[arg_stack_count]);
		arg_stack_count++;
		pc--;
		goto L_HEAD;
	case OPPOP:
		fprintf(stderr, "OPPOP\n");
		arg_stack_count--;
		(*pc)->src = function_arg_stack[arg_stack_count];
		stack[(*pc)->dst] = (*pc)->src;
		fprintf(stderr, "pop value = [%d]\n", function_arg_stack[arg_stack_count]);
		pc--;
		goto L_HEAD;
	case OPRET:
		fprintf(stderr, "OPRET\n");
		pc--;
		break;
	default:
		break;
	}
	stack_count = 0;
	/*
	int i = 0;
	while (i < vm_count) {
		free(vmcode[i]);
		vmcode[i] = NULL;
		i++;
	}
	*/
	local_func_code = NULL;
	return stack[0];
}

VirtualMachine *new_VirtualMachine(void)
{
	VirtualMachine *ret = (VirtualMachine *)gmalloc(sizeof(VirtualMachine));
	ret->run = VirtualMachine_run;
	return ret;
}
