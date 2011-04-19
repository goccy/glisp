#include <glisp.h>
#define MAX_STACK_SIZE 128
#define TYPE_NAME(type)

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
	case OPRET:
		fprintf(stderr, "OPRET : ");
		break;
	default:
		break;
	}
	fprintf(stderr, "dst = %d src = %d\n", code->dst, code->src);
}

static int stack_count = 0;
static int jump_flag = 0;
static int *jump_true = 0;
static int *jump_false = 0;
VirtualMachineCode *new_VirtualMachineCode(Conscell *c, int base_count)
{
	VirtualMachineCode *ret = (VirtualMachineCode *)malloc(sizeof(VirtualMachineCode));
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
		//ret->src = stack_count + base_count + 1;
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
		//OPMOV
		ret->op = OPMOV;
		ret->dst = stack_count + base_count;
		ret->src = c->num;//unused parameter
		stack_count++;
		break;
	case IF:
		ret->op = OPCMP;
		ret->dst = 0;
		ret->src = 0;
		break;
	case LESS:
		ret->op = OPJL;
		ret->dst = stack_count;
		ret->src = stack_count + base_count + 1;
		//ret->dst = *jump_true;
		//ret->src = *jump_false;
		jump_flag = 1;
		break;
	default:
		break;
	}
	return ret;
}

static int vm_count = 0;
static VirtualMachineCode *vmcodes[MAX_STACK_SIZE] = {NULL};
static int Compiler_opCompile(Conscell *path, int base_count, int isRecursive)
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
	if (optype == ADD || optype == SUB || optype == MULTI || optype == DIV) {
		int i, n = 0;
		Conscell *tmp = path;
		tmp = tmp->cdr;
		for (i = 0; i < create_inst_num; i++) {
			if (tmp->type == LEFT_PARENTHESIS) {
				//fprintf(stderr, "Recursive Call\n");
				n = Compiler_opCompile(tmp->car, i, 1);//recursive
				ret += n;
				//fprintf(stderr, "n = [%d], i = [%d] ret = [%d]\n", n, i, ret);
				VirtualMachineCode *code = NULL;
				if (!isRecursive) {
					code = new_VirtualMachineCode(path, ret);
					code->dump(code);
					vmcodes[vm_count] = code;
					vm_count++;
				}
			} else {
				ret++;
				//fprintf(stderr, "ret = [%d]\n", ret);
				//fprintf(stderr, "Value Cell\n");
				VirtualMachineCode *code = NULL;
				if (!isRecursive) {
					code = new_VirtualMachineCode(path, ret);
					code->dump(code);
					vmcodes[vm_count] = code;
					vm_count++;
				}
			}
			tmp = tmp->cdr;
		}
	} else {
		while (path->cdr != NULL) {
			path = path->cdr;
			ret++;
		}
	}
	return ret;
}

static VirtualMachineCode **Compiler_compile(Conscell *path)
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
		Compiler_opCompile(path, 0, 0);
		puts("=======================================");
		tmp = path;
		while (tmp->cdr != NULL) {
			tmp = tmp->cdr;
			if (tmp->type != LEFT_PARENTHESIS) {
				printTypeName(tmp);
				VirtualMachineCode *c = new_VirtualMachineCode(tmp, 0);
				c->dump(c);
				vmcodes[vm_count] = c;
				vm_count++;
			} else {
				Compiler_compile(tmp);//recursive call
			}
		}
	}
	if (jump_flag) {
		//jump true
	}
	fprintf(stderr, "---end---\n");
	return vmcodes;
}

Compiler *new_Compiler(void)
{
	Compiler *ret = (Compiler *)malloc(sizeof(Compiler));
	ret->compile = Compiler_compile;
	VirtualMachineCode *code = new_VirtualMachineCode(NULL, 0);
	vmcodes[vm_count] = code;
	vm_count++;
	return ret;
}

static void fixedVirtualMachineCode(VirtualMachineCode **vmcode)
{
	int i = 0;
	while (vmcode[i] != NULL) {
		if ((vmcode[i]->op == OPADD || vmcode[i]->op == OPJL) &&
			vmcode[i + 1]->op != OPMOV) {
			vmcode[i]->src++;
		}
		i++;
	}
}

static void dumpAllVirtualMachineCode(VirtualMachineCode **vmcode)
{
	int i = 0;
	while (vmcode[i] != NULL) {
		fprintf(stderr, "L%d : ", i);
		vmcode[i]->dump(vmcode[i]);
		i++;
	}
}

static void VirtualMachine_run(VirtualMachineCode **vmcode)
{
	//asm("int3");
	//fixedVirtualMachineCode(vmcode);
	dumpAllVirtualMachineCode(vmcode);
	int stack[MAX_STACK_SIZE] = {0};
	int vm_stack_top = vm_count;
	//fprintf(stderr, "stack_top = [%d]\n", vm_stack_top);
	VirtualMachineCode **pc = vmcode + vm_stack_top - 1;
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
	case OPJL:
		fprintf(stderr, "OPJL\n");
		if (stack[(*pc)->dst] < stack[(*pc)->src]) {
			puts("true");
			stack[0] = stack[(*pc)->dst + 2];
		} else {
			puts("false");
			stack[0] = stack[(*pc)->src + 2];
		}
		pc--;
		goto L_HEAD;
	case OPRET:
		fprintf(stderr, "OPRET\n");
		pc--;
		break;
	default:
		break;
	}
	fprintf(stderr, "ans = [%d]\n", stack[0]);
	stack_count = 0;
	int i = 0;
	while (i < vm_count) {
		free(vmcode[i]);
		vmcode[i] = NULL;
		i++;
	}
	vm_count = 0;
}

VirtualMachine *new_VirtualMachine(void)
{
	VirtualMachine *ret = (VirtualMachine *)malloc(sizeof(VirtualMachine));
	ret->run = VirtualMachine_run;
	return ret;
}
