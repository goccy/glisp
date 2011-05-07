#include <glisp.h>

#define MAX_VIRTUAL_MEMORY_SIZE 32
static GMap *virtual_memory[MAX_VIRTUAL_MEMORY_SIZE] = {NULL};
static int virtual_memory_address = 0;
GMap *new_GMap(const char *key, void *value, CellType type)
{
	GMap *ret = (GMap *)gmalloc(sizeof(GMap));
	ret->key = (const char *)gmalloc(strlen(key) + 1);
	strncpy((char *)ret->key, key, strlen(key) + 1);
	ret->value = value;
	ret->type = type;
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

void clear_virtual_memory(void)
{
	size_t i = 0;
	while (virtual_memory[i] != NULL) {
		GMap *map = virtual_memory[i];
		if (map->type == DEFUN) {
			VirtualMachineCode *code = (VirtualMachineCode *)map->value;
			free(--code);
		}
		i++;
	}
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

/*=============================<<<  VirtualMachineCode  Class >>> ================================*/

static void VirtualMachineCode_dump(VirtualMachineCode *code)
{
	switch (code->op) {
	case OPADD:
		DBG_PL("OPADD : ");
		break;
	case OPSUB:
		DBG_PL("OPSUB : ");
		break;
	case OPMUL:
		DBG_PL("OPMUL : ");
		break;
	case OPDIV:
		DBG_PL("OPDIV : ");
		break;
	case OPMOV:
		DBG_PL("OPMOV : ");
		break;
	case OPCMP:
		DBG_PL("OPCMP : ");
		break;
	case OPJL:
		DBG_PL("OPJL  : ");
		break;
	case OPJG:
		DBG_PL("OPJG  : ");
		break;
	case OPSTORE:
		DBG_PL("OPSTORE : ");
		DBG_PL("name = %s ", code->name);
		if (code->args != NULL) {
			int i = 0;
			while (code->args[i] != NULL) {
				DBG_PL("args[%d] = %s ", i, code->args[i]);
				i++;
			}
		}
		break;
	case OPLOAD:
		DBG_PL("OPLOAD : ");
		break;
	case OPPUSH:
		DBG_PL("OPPUSH : ");
		break;
	case OPPOP:
		DBG_PL("OPPOP : ");
		break;
	case OPCALL:
		DBG_PL("OPCALL : ");
		break;
	case OPRET:
		DBG_PL("OPRET : ");
		break;
	case OPFASTCALL:
		DBG_PL("OPFASTCALL : ");
		break;
	case OPPFASTCALL:
		DBG_PL("OPPFASTCALL : ");
		break;
	case OPiPFASTCALL:
		DBG_PL("OPiPFASTCALL : ");
		break;
	case OPiJLC:
		DBG_PL("OPiJLC : ");
		break;
	case OPiJGC:
		DBG_PL("OPiJGC : ");
		break;
	case OPiSUBC:
		DBG_PL("OPiSUBC : ");
		break;
	case OPiADDC:
		DBG_PL("OPiADDC : ");
		break;
	case OPiPUSHC:
		DBG_PL("OPiPUSHC : ");
		break;
	case OPCOPY:
		DBG_PL("OPCOPY : ");
		break;
	case OPTHCODE:
		DBG_PL("OPTHCODE : ");
		break;
	case OPNOP:
		DBG_PL("OPNOP : ");
		break;
	case OPABADD:
		DBG_PL("OPABADD : ");
		break;
	case OPACADD:
		DBG_PL("OPACADD : ");
		break;
	case OPADADD:
		DBG_PL("OPADADD : ");
		break;
	case OPBCADD:
		DBG_PL("OPBCADD : ");
		break;
	case OPBDADD:
		DBG_PL("OPBDADD : ");
		break;
	case OPCDADD:
		DBG_PL("OPCDADD : ");
		break;
	case OPAiADDC:
		DBG_PL("OPAiADDC : ");
		break;
	case OPBiADDC:
		DBG_PL("OPBiADDC : ");
		break;
	case OPCiADDC:
		DBG_PL("OPCiADDC : ");
		break;
	case OPDiADDC:
		DBG_PL("OPDiADDC : ");
		break;
	case OPAiSUBC:
		DBG_PL("OPAiSUBC : ");
		break;
	case OPBiSUBC:
		DBG_PL("OPBiSUBC : ");
		break;
	case OPCiSUBC:
		DBG_PL("OPCiSUBC : ");
		break;
	case OPDiSUBC:
		DBG_PL("OPDiSUBC : ");
		break;
	case OPAPOP:
		DBG_PL("OPAPOP : ");
		break;
	case OPBPOP:
		DBG_PL("OPBPOP : ");
		break;
	case OPCPOP:
		DBG_PL("OPCPOP : ");
		break;
	case OPDPOP:
		DBG_PL("OPDPOP : ");
		break;
	case OPAPUSH:
		DBG_PL("OPAPUSH : ");
		break;
	case OPBPUSH:
		DBG_PL("OPBPUSH : ");
		break;
	case OPCPUSH:
		DBG_PL("OPCPUSH : ");
		break;
	case OPDPUSH:
		DBG_PL("OPDPUSH : ");
		break;
	case OPAiPUSHC:
		DBG_PL("OPAiPUSHC : ");
		break;
	case OPBiPUSHC:
		DBG_PL("OPBiPUSHC : ");
		break;
	case OPCiPUSHC:
		DBG_PL("OPCiPUSHC : ");
		break;
	case OPDiPUSHC:
		DBG_PL("OPDiPUSHC : ");
		break;
	case OPACOPY:
		DBG_PL("OPACOPY : ");
		break;
	case OPBCOPY:
		DBG_PL("OPBCOPY : ");
		break;
	case OPCCOPY:
		DBG_PL("OPCCOPY : ");
		break;
	case OPDCOPY:
		DBG_PL("OPDCOPY : ");
		break;
	case OPAiJLC:
		DBG_PL("OPAiJLC : ");
		break;
	case OPBiJLC:
		DBG_PL("OPBiJLC : ");
		break;
	case OPCiJLC:
		DBG_PL("OPCiJLC : ");
		break;
	case OPDiJLC:
		DBG_PL("OPDiJLC : ");
		break;
	case OPAiJGC:
		DBG_PL("OPAiJGC : ");
		break;
	case OPBiJGC:
		DBG_PL("OPBiJGC : ");
		break;
	case OPCiJGC:
		DBG_PL("OPCiJGC : ");
		break;
	case OPDiJGC:
		DBG_PL("OPDiJGC : ");
		break;
	case OPAFASTCALL:
		DBG_PL("OPAFASTCALL : ");
		break;
	case OPBFASTCALL:
		DBG_PL("OPBFASTCALL : ");
		break;
	case OPCFASTCALL:
		DBG_PL("OPCFASTCALL : ");
		break;
	case OPDFASTCALL:
		DBG_PL("OPDFASTCALL : ");
		break;
	case OPAPFASTCALL:
		DBG_PL("OPAPFASTCALL : ");
		break;
	case OPBPFASTCALL:
		DBG_PL("OPBPFASTCALL : ");
		break;
	case OPCPFASTCALL:
		DBG_PL("OPCPFASTCALL : ");
		break;
	case OPDPFASTCALL:
		DBG_PL("OPDPFASTCALL : ");
		break;
	case OPARET:
		DBG_PL("OPARET : ");
		break;
	case OPBRET:
		DBG_PL("OPBRET : ");
		break;
	case OPCRET:
		DBG_PL("OPCRET : ");
		break;
	case OPDRET:
		DBG_PL("OPDRET : ");
		break;
	case OPAMOV:
		DBG_PL("OPAMOV : ");
		break;
	case OPBMOV:
		DBG_PL("OPBMOV : ");
		break;
	case OPCMOV:
		DBG_PL("OPCMOV : ");
		break;
	case OPDMOV:
		DBG_PL("OPDMOV : ");
		break;
	case GOTO_BLOCK:
		DBG_PL("GOTO_BLOCK : ");
		break;
	case RET_BLOCK:
		DBG_PL("RET_BLOCK : ");
		break;
	default:
		break;
	}
	DBG_P("dst = %d src = %d jmp = %d opnext = %p", code->dst, code->src, code->jmp, code->opnext);
}

static void VirtualMachineCode_delete(VirtualMachineCode *c)
{
	free((char *)c->name);
	int i = 0;
	if (c->args != NULL) {
		while (c->args[i] != NULL) {
			free((char *)c->args[i]);
			i++;
		}
		free(c->args);
	}
	free(c->api);
	free(c);
	c = NULL;
}

static VirtualMachineCodeAPI *new_VirtualMachineCodeAPI(void)
{
	VirtualMachineCodeAPI *ret = (VirtualMachineCodeAPI *)gmalloc(sizeof(VirtualMachineCodeAPI));
	ret->dump = VirtualMachineCode_dump;
	ret->free = VirtualMachineCode_delete;
	return ret;
}

//===========Bad Code===============//
static int false_jump_register = 0;
static int ret_call_count = 0;
//==================================//
static int stack_count = 0;
VirtualMachineCode *new_VirtualMachineCode(Conscell *c, int base_count)
{
	//asm("int3");
	VirtualMachineCode *ret = (VirtualMachineCode *)gmalloc(sizeof(VirtualMachineCode));
	ret->jmp = 0;
	ret->name = NULL;
	ret->args = NULL;
	ret->api = new_VirtualMachineCodeAPI();
	if (c == NULL) {
		ret->op = OPRET;
		ret->dst = 0;
		ret->src = stack_count;
		return ret;
	}
	switch (c->type) {
	case ADD:
		ret->op = OPADD;
		ret->dst = stack_count;
		ret->src = stack_count + base_count;
		false_jump_register++;
		break;
	case SUB:
		ret->op = OPSUB;
		ret->dst = stack_count;
		ret->src = stack_count + base_count;
		false_jump_register++;
		break;
	case MULTI:
		ret->op = OPMUL;
		ret->dst = stack_count;
		ret->src = stack_count + base_count;
		false_jump_register++;
		break;
	case DIV:
		ret->op = OPDIV;
		ret->dst = stack_count;
		ret->src = stack_count + base_count;
		false_jump_register++;
		break;
	case NUM:
		ret->op = OPMOV;
		ret->dst = stack_count + base_count;
		ret->src = c->num;
		stack_count++;
		false_jump_register++;
		break;
	case FUNC_ARGS:
		ret->op = OPPUSH;
		ret->dst = stack_count + base_count;
		ret->src = c->num;
		stack_count++;
		false_jump_register++;
		break;
	case IF:
		ret->op = OPCMP;
		ret->dst = false_jump_register + 2;
		ret->src = -1;
		false_jump_register++;
		break;
	case LESS:
		ret->op = OPJL;
		ret->dst = stack_count;
		ret->src = stack_count + base_count;
		false_jump_register++;
		break;
	case GRATER:
		ret->op = OPJG;
		ret->dst = stack_count;
		ret->src = stack_count + base_count;
		false_jump_register++;
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
		false_jump_register++;
		//through
	case SETQ: {
		ret->op = OPSTORE;
		ret->dst = stack_count;
		ret->src = stack_count + base_count;
		Conscell *variable_cell = c->cdr;
		char *variable_name = variable_cell->string;
		size_t variable_name_length = strlen(variable_name) + 1;
		ret->name = (char *)gmalloc(variable_name_length);
		strncpy((char *)ret->name, variable_name, variable_name_length);
		false_jump_register++;
		break;
	}
	case STRING: {
		ret->op = OPLOAD;
		ret->dst = stack_count;
		ret->src = -1; //unused parameter
		char *variable_name = c->string;
		size_t variable_name_length = strlen(variable_name) + 1;
		ret->name = (char *)gmalloc(variable_name_length);
		strncpy((char *)ret->name, variable_name, variable_name_length);
		stack_count++;
		false_jump_register++;
		break;
	}
	case FUNC: {
		ret->op = OPCALL;
		ret->dst = stack_count;
		ret->src = -1;
		char *variable_name = c->string;
		size_t variable_name_length = strlen(variable_name) + 1;
		ret->name = (char *)gmalloc(variable_name_length);
		strncpy((char *)ret->name, variable_name, variable_name_length);
		false_jump_register++;
		break;
	}
	default:
		break;
	}
	return ret;
}

/*=============================<<<  Compiler  Class >>> ================================*/

static VirtualMachineCodeArray *Compiler_compile(Compiler *compiler, Conscell *path);
static VirtualMachineCode *local_func_code = NULL;
static int isExecFlag = 1;
static int isConditionFlag = 0;
static int isSetFlag = 0;
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
				if (!isRecursive) {
					VirtualMachineCode *code = new_VirtualMachineCode(path, ret);
					code->api->dump(code);
					c->vmcodes->add(c->vmcodes, code);
				}
			} else {
				ret++;
				//fprintf(stderr, "ret = [%d]\n", ret);
				//fprintf(stderr, "Value Cell\n");
				if (!isRecursive) {
					VirtualMachineCode *code = new_VirtualMachineCode(path, ret);
					code->api->dump(code);
					c->vmcodes->add(c->vmcodes, code);
				}
			}
			tmp = tmp->cdr;
		}
	} else if (optype == SETQ || optype == DEFUN || optype == FUNC) {
		if (!isRecursive) {
			VirtualMachineCode *code = new_VirtualMachineCode(path, 0);
			code->api->dump(code);
			c->vmcodes->add(c->vmcodes, code);
			if (optype == SETQ ||optype == DEFUN) {
				isSetFlag = 1;
			}
			if (optype == DEFUN) {
				isExecFlag = 0;
				local_func_code = code; //set: ====> local_func_code
				c->isExecFlag = 0;
			} else if (optype == FUNC && !isExecFlag) {
				DBG_P("defun");
				//TODO :  must support some arguments
				//========== create push code ===========//
				code = new_VirtualMachineCode(path, 0);
				code->op = OPPUSH;
				code->dst = stack_count;
				code->src = -1;
				//=======================================//
				code->api->dump(code);
				c->vmcodes->add(c->vmcodes, code);
			}
		} else {
			if (optype == FUNC) {
				int n = Compiler_opCompile(c, path->cdr->car, 1);//recursive
				ret += n;
			}
		}
	} else if (optype == IF) {
		//asm("int3");
		Conscell *tmp = path->cdr;
		while (tmp->cdr != NULL) {
			tmp = tmp->cdr;
			if (tmp->type == NUM) {
				VirtualMachineCode *code = new_VirtualMachineCode(tmp, ret);
				code->api->dump(code);
				c->vmcodes->add(c->vmcodes, code);
			} else {
				Compiler_opCompile(c, tmp, 0);
			}
		}
		//c->vmcodes->dump(c->vmcodes);
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
	Conscell *tmp = NULL;
	while (path->car != NULL && path->type != FUNC) {
		path = path->car;
		if (path->cdr == NULL) break;
		//DBG_P("opcode = ");
		path->printTypeName(path);
		if (path->type == IF) {
			tmp = path->cdr;
			while (tmp->cdr != NULL) {
				if (tmp->type == SETQ || tmp->type == DEFUN) tmp = tmp->cdr;//skip string cell
				tmp = tmp->cdr;
				VirtualMachineCode *code = new_VirtualMachineCode(NULL, 0);//OPRET
				code->api->dump(code);
				compiler->vmcodes->add(compiler->vmcodes, code);
				//=====TODO : must fix this bad code becase cannot support if nested code.====
				ret_call_count++;
				if (ret_call_count == 2) {
					false_jump_register = 0;//stack_count;
				}
				//============================================================================
				if (tmp->type != LEFT_PARENTHESIS) {
					tmp->printTypeName(tmp);
					VirtualMachineCode *c = new_VirtualMachineCode(tmp, 0);
					c->api->dump(c);
					compiler->vmcodes->add(compiler->vmcodes, c);
					compiler->vmcodes->dump(compiler->vmcodes);
				} else {
					Compiler_compile(compiler, tmp);//recursive call
				}
			}
			//DBG_P("=== create calculate operation code ===");
			Compiler_opCompile(compiler, path, 0);
			//DBG_P("=======================================");
			path = path->cdr;
		} else {
			//DBG_P("=== create calculate operation code ===");
			Compiler_opCompile(compiler, path, 0);
			//DBG_P("=======================================");
			tmp = path;
			while (tmp->cdr != NULL) {
				if (tmp->type == SETQ || tmp->type == DEFUN) tmp = tmp->cdr;//skip string cell
				tmp = tmp->cdr;
				if (tmp->type != LEFT_PARENTHESIS) {
					tmp->printTypeName(tmp);
					VirtualMachineCode *c = new_VirtualMachineCode(tmp, 0);
					c->api->dump(c);
					compiler->vmcodes->add(compiler->vmcodes, c);
				} else {
					Compiler_compile(compiler, tmp);//recursive call
				}
			}
		}
	}
	return compiler->vmcodes;
}

inline int gmatch(const char *i, const char *you)
{
	if (strlen(i) == strlen(you) &&
		!strncmp(i, you, strlen(i))) {
		return 1;
	} else {
		return 0;
	}
}

static int isSETQ = 0;
static int isDEFUN = 0;
static void Compiler_compileToFastCode(VirtualMachineCodeArray *vmcode)
{
	stack_count = 0;
	DBG_P("isSetFlag = [%d]", isSetFlag);//defun or setq
	int vm_stack_top = vmcode->size;
	VirtualMachineCode **pc = vmcode->a;// + vm_stack_top - 1;
	const char *set_name = NULL;
	if (isSetFlag) {
		set_name = vmcode->a[1]->name;//pc[0] ==> OPRET, pc[1] ==> OPSTORE
	}
	int i = 0;
	int jl_register = 0;
	int isFirstPOPFlag = 1;
	for (i = vm_stack_top - 1; i > 0; i--) {
		switch (vmcode->a[i]->op) {
		case OPCALL:
			if (set_name != NULL && gmatch(pc[i]->name, set_name)) {
				//recursive call == translate ==> OPFASTCALL
				DBG_P("RECURSIVE FUNC");
				if (pc[i + 1]->op == OPPUSH) {
					//non void function
					pc[i]->op = OPPFASTCALL;
					if (jl_register != -1) {
						pc[jl_register]->jmp--;
						jl_register--;
					}
					vmcode->remove(vmcode, i + 1);
				} else if (pc[i + 1]->op == OPiPUSHC) {
					//non void function
					pc[i]->op = OPiPFASTCALL;
					if (jl_register != -1) {
						pc[jl_register]->jmp--;
						jl_register--;
					}
					vmcode->remove(vmcode, i + 1);
				} else {
					//func(void)
					pc[i]->op = OPFASTCALL;
				}
			}
			break;
		case OPLOAD:
			if (local_func_code != NULL && search_func_args_from_vmcode(local_func_code, pc[i]->name)) {
				DBG_P("This variable is function's argument!!");
				if (isFirstPOPFlag) {
					DBG_P("convert virtual machine code from OPLOAD to OPPOP");
					pc[i]->op = OPPOP;
					isFirstPOPFlag = 0;
				} else if ((pc[i + 1]->op == OPFASTCALL) || (pc[i + 1]->op == OPPFASTCALL) ||
						   (pc[i + 1]->op == OPiPFASTCALL) || (pc[i + 2]->op == OPFASTCALL) ||
						   (pc[i + 2]->op == OPPFASTCALL) || (pc[i + 2]->op ==  OPiPFASTCALL)) {
					DBG_P("convert virtual machine code from OPLOAD to OPPOP");
					pc[i]->op = OPPOP;
				} else {
					DBG_P("convert virtual machine code from OPLOAD to OPCOPY");
					pc[i]->op = OPCOPY;
				}
			}
			break;
		case OPJL:
			if (pc[i + 1]->op == OPMOV ||
				pc[i + 1]->op == OPPOP) {
				if (pc[i + 2]->op == OPMOV) {
					DBG_P("CONST JMP");
					pc[i]->op = OPiJLC;
					pc[i]->src = pc[i + 2]->src;
					vmcode->remove(vmcode, i + 2);
				}
			}
			pc[i]->jmp = pc[i - 2]->dst;
			jl_register = i - 2;//-2 ==> OPCMP remove twice times
			break;
		case OPJG:
			if (pc[i + 1]->op == OPMOV ||
				pc[i + 1]->op == OPPOP) {
				//if (pc[i + 1]->op == OPMOV) {
				//pc[i]->src = pc[i + 1]->src;
					//pc[i + 2]->dst = pc[i]->dst;
					//vmcode->remove(vmcode, i + 1);
				//} else {
				if (pc[i + 2]->op == OPMOV) {
					DBG_P("CONST JMP");
					pc[i]->op = OPiJGC;
					pc[i]->src = pc[i + 2]->src;
					vmcode->remove(vmcode, i + 2);
				}
			}
			pc[i]->jmp = pc[i - 2]->dst;
			jl_register = i - 2;//-2 ==> OPCMP remove twice times
			break;
		case OPCMP:
			vmcode->remove(vmcode, i);
			break;
		case OPSUB:
			if (//pc[i + 1]->op == OPMOV ||
				pc[i + 2]->op == OPMOV) {
				DBG_P("CONST SUB");
				pc[i]->op = OPiSUBC;
				DBG_P("jl_register = [%d]", jl_register);
				if (jl_register != -1) {
					pc[jl_register]->jmp--;
					jl_register--;
				}
				//if (pc[i + 1]->op == OPMOV) {
				//pc[i]->src = pc[i + 1]->src;
				//vmcode->remove(vmcode, i + 1);
				//} else {
				if (pc[i + 2]->op == OPMOV) {
					pc[i]->src = pc[i + 2]->src;
					vmcode->remove(vmcode, i + 2);
				}
			}
			break;
		case OPADD:
			if (pc[i + 2]->op == OPMOV) {
				pc[i]->op = OPiADDC;
				if (jl_register != -1) {
					pc[jl_register]->jmp--;
					jl_register--;
				}
				pc[i]->src = pc[i + 2]->src;
				vmcode->remove(vmcode, i + 2);
			}
			break;
		case OPSTORE: {
			DBG_P("OPSTORE");
			if (pc[i]->args != NULL) {
				//function
				isDEFUN = 1;
			} else {
				//variable
				isSETQ = 1;
			}
			break;
		}
		case OPPUSH:
			DBG_P("OPPUSH");
			if (pc[i]->src != -1) {
				pc[i]->op = OPiPUSHC;
			}
			break;
		case OPRET:
			jl_register = -1;
			break;
		default:
			break;
		}
	}
}

static int Compiler_getMaxRegNumber(VirtualMachineCodeArray *vmcode)
{
	DBG_P("********* getMaxRegNumber ***********");
	VirtualMachineCode **pc = vmcode->a;
	int max_reg_num = 0;
	size_t i = 0;
	for (i = 0; i < vmcode->size; i++) {
		if (max_reg_num < pc[i]->dst) {
			max_reg_num = pc[i]->dst;
		}
	}
	return max_reg_num;
}

static void Compiler_fixRegNumber(VirtualMachineCodeArray *vmcode)
{
	//DBG_P("********* fixRegNumber ***********");
	volatile int reg_count = 0;
	volatile int target_reg_num[32] = {0};
	VirtualMachineCode **pc = vmcode->a;
	volatile size_t i, j = 0;
	for (i = 0; i < vmcode->size; i++) {
		//DBG_P("reg_count = [%d]", pc[i]->dst);
		if (reg_count != pc[i]->dst) {
			if (reg_count + 1 < pc[i]->dst) {
				target_reg_num[j] = pc[i]->dst;
				j++;
				reg_count = pc[i]->dst;
			} else {
				reg_count = pc[i]->dst;
			}
		}
	}
	if (target_reg_num[0] == 0) return;
	for (i = 0; target_reg_num[i] != 0; i++) {
		//DBG_P("target_reg_num = [%d]", target_reg_num[i]);
		for (j = 0; j < vmcode->size; j++) {
			switch (pc[j]->op) {
			case OPPOP: case OPPUSH: case OPiJLC: case OPiPUSHC: case OPiSUBC: case OPiJGC:
			case OPPFASTCALL: case OPiPFASTCALL: case OPFASTCALL: case OPCALL: case OPCOPY: case OPMOV:
				if (pc[j]->dst == target_reg_num[i]) {
					pc[j]->dst = target_reg_num[i] - 1;
				}
				break;
			case OPADD: case OPSUB: case OPMUL: case OPDIV: case OPRET:
				if (pc[j]->dst == target_reg_num[i]) {
					pc[j]->dst = target_reg_num[i] - 1;
				} else if (pc[j]->src == target_reg_num[i]) {
					pc[j]->src = target_reg_num[i] - 1;
				}
				break;
			default:
				break;
			}
		}
	}
	Compiler_fixRegNumber(vmcode);
}

#define CONCAT(c0, c1, c2) OP##c0##c1##c2
#define CONCAT2(c0, c1) OP##c0##c1

#define DST(n) pc[i]->dst == n
#define SRC(n) pc[i]->src == n

#define OPCREATE1(c) {							\
		if (DST(0) && SRC(1)) {					\
			pc[i]->op = CONCAT(A, B, c);		\
		} else if (DST(0) && SRC(2)) {			\
			pc[i]->op = CONCAT(A, C, c);		\
		} else if (DST(0) && SRC(3)) {			\
			pc[i]->op = CONCAT(A, D, c);		\
		} else if (DST(1) && SRC(2)) {			\
			pc[i]->op = CONCAT(B, C, c);		\
		} else if (DST(1) && SRC(3)) {			\
			pc[i]->op = CONCAT(B, D, c);		\
		} else if (DST(2) && SRC(3)) {			\
			pc[i]->op = CONCAT(C, D, c);		\
		}										\
	}											

#define OPCREATE2(c) {						    \
		if (DST(0)) {							\
			pc[i]->op = CONCAT2(A, c);			\
		} else if (DST(1)) {					\
			pc[i]->op = CONCAT2(B, c);			\
		} else if (DST(2)) {					\
			pc[i]->op = CONCAT2(C, c);			\
		} else if (DST(3)) {					\
			pc[i]->op = CONCAT2(D, c);			\
		}										\
	}

#define OPCREATE3(c) {						    \
		if (SRC(0)) {							\
			pc[i]->op = CONCAT2(A, c);			\
		} else if (SRC(1)) {					\
			pc[i]->op = CONCAT2(B, c);			\
		} else if (SRC(2)) {					\
			pc[i]->op = CONCAT2(C, c);			\
		} else if (SRC(3)) {					\
			pc[i]->op = CONCAT2(D, c);			\
		}										\
	}


static void Compiler_finalCompile(VirtualMachineCodeArray *vmcode)
{
	DBG_P("********* finalCompile ***********");
	VirtualMachineCode **pc = vmcode->a;
	size_t i = 0;
	for (i = 0; i < vmcode->size; i++) {
		switch (pc[i]->op) {
		case OPADD:
			OPCREATE1(ADD);
			break;
		case OPiSUBC:
			OPCREATE2(iSUBC);
			break;
		case OPiJLC:
			OPCREATE2(iJLC);
			break;
		case OPiJGC:
			OPCREATE2(iJGC);
			break;
		case OPFASTCALL:
			OPCREATE2(FASTCALL);
			break;
		case OPPFASTCALL:
			OPCREATE2(PFASTCALL);
			break;
		case OPiPUSHC:
			OPCREATE2(iPUSHC);
			break;
		case OPCOPY:
			OPCREATE2(COPY);
			break;
		case OPPOP:
			OPCREATE2(POP);
			break;
		case OPPUSH:
			OPCREATE2(PUSH);
			break;
		case OPMOV:
			OPCREATE2(MOV);
			break;
		case OPRET:
			OPCREATE3(RET);
			break;
		default:
			break;
		}
	}
}

static void Compiler_delete(Compiler *c)
{
	free(c);
	c = NULL;
}

Compiler *new_Compiler(void)
{
	Compiler *ret = (Compiler *)malloc(sizeof(Compiler));
	ret->vmcodes = new_VirtualMachineCodeArray();
	ret->isExecFlag = 1;
	ret->compile = Compiler_compile;
	ret->compileToFastCode = Compiler_compileToFastCode;
	ret->fixRegNumber = Compiler_fixRegNumber;
	ret->getMaxRegNumber = Compiler_getMaxRegNumber;
	ret->finalCompile = Compiler_finalCompile;
	VirtualMachineCode *code = new_VirtualMachineCode(NULL, 0);//OPRET
	ret->vmcodes->add(ret->vmcodes, code);
	ret->free = Compiler_delete;
	return ret;
}

/*=============================<<<  VirtualMachine  Class >>> ================================*/

inline void VirtualMachine_createDirectThreadingCode(VirtualMachineCode *vmcode, void **jmp_table)
{
	vmcode->op = OPNOP;
	VirtualMachineCode *pc = vmcode;
	for (; pc->api != NULL; pc++) {
		pc->opnext = jmp_table[pc->op];
	}
}

#include <sys/mman.h>
#include <unistd.h>
typedef void (*Function)(void);
typedef struct _InstBlock {
	void *start;
	void *end;
} InstBlock;

static void *gxmalloc(void)
{
	int pagesize = sysconf(_SC_PAGE_SIZE);
	DBG_P("pagesize = [%d]", pagesize);
	if (pagesize < 0) perror("sysconf");
	void *_codeptr = (void *)gmalloc(pagesize * 10L);
	void *codeptr = (void *)((long)_codeptr & ~(pagesize - 1L));
	memset(codeptr, 0xc3, pagesize * 10L);
	if (mprotect(codeptr, pagesize * 10L, PROT_WRITE|PROT_READ|PROT_EXEC) < 0) perror("mprotect");
	return codeptr;
}

static void VirtualMachine_createSelectiveInliningCode(VirtualMachineCode *vmcode, void **jmp_table, InstBlock *block_table)
{
	//void *codeptr = gxmalloc();
	int pos = 0;
	VirtualMachineCode *pc = vmcode;
	VirtualMachineCode *tmp = vmcode;
	pc++;//skip THCODE
	tmp++;
	while (1) {
		if (pc->op == OPSTORE) break;
		pc->code = gxmalloc();
		pos = 0;
		for (; tmp->api != NULL; tmp++) {
			switch (tmp->op) {
			case OPJL: case OPJG: case OPiJLC: case OPiJGC:
			case OPDiJLC: case OPRET:
			case OPARET: case OPBRET: case OPCRET: case OPDRET: {
				InstBlock block = block_table[tmp->op];
				int len = block.end - block.start;
				memcpy(pc->code + pos, block.start, len);
				pos += len;
				tmp->api->dump(tmp);
				DBG_P("len = [%d]", len);
				goto BREAK;
			}
			case OPLOAD: case OPCALL: case OPCPFASTCALL: case OPBPFASTCALL: {
				goto BREAK;
			}
			default: {
				InstBlock block = block_table[tmp->op];
				int len = block.end - block.start;
				memcpy(pc->code + pos, block.start, len);
				pos += len;
				tmp->api->dump(tmp);
				DBG_P("len = [%d]", len);
				break;
			}
			}
		}
	BREAK:;
		DBG_P("=====================================");
		if (tmp->op == OPLOAD || tmp->op == OPCALL || tmp->op == OPCPFASTCALL || tmp->op == OPBPFASTCALL) {
		} else if (tmp->op == OPRET || tmp->op == OPARET || tmp->op == OPBRET ||
				   tmp->op == OPCRET || tmp->op == OPDRET) {
			pc->op = RET_BLOCK;
			pc->opnext = jmp_table[pc->op];
		} else {
			pc->op = GOTO_BLOCK;
			pc->opnext = jmp_table[pc->op];
		}
		if ((tmp + 1)->api != NULL) {
			tmp++;
		} else {
			break;
		}
		pc = tmp;
	}
}

#define L(op) L_##op
#define CASE(op) L(op) :
#define DISPATCH_START() {						\
		r->ret_label_stack = &&L_RETURN;		\
		goto *jmp_table[pc->op];				\
	}
#define DISPATCH_END() L_RETURN:
#define NEXTOP *(pc->opnext)
#define MAX_REG_SIZE 32
#define regsize sizeof(int) * 4
#define START(op) L_##op##_START:
#define END(op) L_##op##_END:

typedef struct recursive_stack {
	int reg_stack[MAX_REG_SIZE];
	int function_arg_stack;
	void *ret_label_stack;
	VirtualMachineCode *pc_stack;
} RecursiveStack;

#ifdef _X86_
#define PUSH(reg, arg, label, pc) {				\
		asm volatile("push reg");				\
	}
#else
#define PUSH(reg, arg, label, pc) {				\
		r++;									\
		r->function_arg_stack = arg;			\
		r->ret_label_stack = label;				\
		r->pc_stack = pc;						\
		memcpy(r->reg_stack, reg, regsize);		\
	}											
#endif
#define POP() {									\
		pc = r->pc_stack;						\
		memcpy(reg, r->reg_stack, regsize);		\
		r--;									\
	}

#define RETURN() {								\
		goto *r->ret_label_stack;				\
	}

static int VirtualMachine_run(VirtualMachineCode *vmcode)
{
	DBG_P("=============<<< run >>>===================");
	RecursiveStack _r[MAX_STACK_SIZE];
	RecursiveStack *r = _r;
	VirtualMachineCode *pc = vmcode;
	int reg[MAX_REG_SIZE] = {0};
	VirtualMachineCode *local_cache_func_vmcode = NULL;
	int cur_arg = -1; //this value is flag that copys pop num to all function argument
	
	static void *jmp_table[] = {
		&&L(OPMOV), &&L(OPADD), &&L(OPSUB), &&L(OPMUL), &&L(OPDIV),
		&&L(OPCALL), &&L(OPJMP), &&L(OPCMP), &&L(OPPOP), &&L(OPPUSH),
		&&L(OPRET), &&L(OPJL), &&L(OPJG), &&L(OPSTORE), &&L(OPLOAD),
		&&L(OPiADDC), &&L(OPiSUBC), &&L(OPiJLC), &&L(OPiJGC), &&L(OPFASTCALL),
		&&L(OPPFASTCALL), &&L(OPiPFASTCALL),
		&&L(OPiPUSHC), &&L(OPCOPY), &&L(OPTHCODE), &&L(OPNOP),
		/*FINAL INST*/
		&&L(OPABADD), &&L(OPACADD), &&L(OPADADD), &&L(OPBCADD),	&&L(OPBDADD), &&L(OPCDADD), 
		&&L(OPAiADDC), &&L(OPBiADDC), &&L(OPCiADDC), &&L(OPDiADDC),
		&&L(OPAiSUBC), &&L(OPBiSUBC), &&L(OPCiSUBC), &&L(OPDiSUBC), 
		&&L(OPAPOP), &&L(OPBPOP), &&L(OPCPOP), &&L(OPDPOP),
		&&L(OPAPUSH), &&L(OPBPUSH),	&&L(OPCPUSH), &&L(OPDPUSH),
		&&L(OPAiPUSHC),	&&L(OPBiPUSHC),	&&L(OPCiPUSHC),	&&L(OPDiPUSHC),
		&&L(OPACOPY), &&L(OPBCOPY), &&L(OPCCOPY), &&L(OPDCOPY),
		&&L(OPAiJLC), &&L(OPBiJLC), &&L(OPCiJLC), &&L(OPDiJLC),
		&&L(OPAiJGC), &&L(OPBiJGC),	&&L(OPCiJGC), &&L(OPDiJGC),
		&&L(OPAFASTCALL), &&L(OPBFASTCALL), &&L(OPCFASTCALL), &&L(OPDFASTCALL),
		&&L(OPAPFASTCALL), &&L(OPBPFASTCALL), &&L(OPCPFASTCALL), &&L(OPDPFASTCALL),
		&&L(OPARET), &&L(OPBRET), &&L(OPCRET), &&L(OPDRET),
		&&L(OPAMOV), &&L(OPBMOV), &&L(OPCMOV), &&L(OPDMOV),
		&&L(GOTO_BLOCK), &&L(RET_BLOCK),
	};
	static InstBlock block_table[] = {
		{&&L(OPMOV_START), &&L(OPMOV_END)}, {&&L(OPADD_START), &&L(OPADD_END)}, {&&L(OPSUB_START), &&L(OPSUB_END)}, {&&L(OPMUL_START), &&L(OPMUL_END)}, {&&L(OPDIV_START), &&L(OPDIV_END)},
		{&&L(OPCALL_START), &&L(OPCALL_END)}, {&&L(OPJMP_START), &&L(OPJMP_END)}, {&&L(OPCMP_START), &&L(OPCMP_END)}, {&&L(OPPOP_START), &&L(OPPOP_END)}, {&&L(OPPUSH_START), &&L(OPPUSH_END)},
		{&&L(OPRET_START), &&L(OPRET_END)}, {&&L(OPJL_START), &&L(OPJL_END)}, {&&L(OPJG_START), &&L(OPJG_END)}, {&&L(OPSTORE_START), &&L(OPSTORE_END)}, {&&L(OPLOAD_START), &&L(OPLOAD_END)},
		{&&L(OPiADDC_START), &&L(OPiADDC_END)}, {&&L(OPiSUBC_START), &&L(OPiSUBC_END)}, {&&L(OPiJLC_START), &&L(OPiJLC_END)}, {&&L(OPiJGC_START), &&L(OPiJGC_END)}, {&&L(OPFASTCALL_START), &&L(OPFASTCALL_END)},
		{&&L(OPPFASTCALL_START), &&L(OPPFASTCALL_END)}, {&&L(OPiPFASTCALL_START), &&L(OPiPFASTCALL_END)},
		{&&L(OPiPUSHC_START), &&L(OPiPUSHC_END)}, {&&L(OPCOPY_START), &&L(OPCOPY_END)}, {NULL, NULL}, {NULL, NULL},
		/*FINAL INST*/
		{&&L(OPABADD_START), &&L(OPABADD_END)}, {&&L(OPACADD_START), &&L(OPACADD_END)}, {&&L(OPADADD_START), &&L(OPADADD_END)}, {&&L(OPBCADD_START), &&L(OPBCADD_END)},	{&&L(OPBDADD_START), &&L(OPBDADD_END)}, {&&L(OPCDADD_START), &&L(OPCDADD_END)},
		{&&L(OPAiADDC_START), &&L(OPAiADDC_END)}, {&&L(OPBiADDC_START), &&L(OPBiADDC_END)}, {&&L(OPCiADDC_START), &&L(OPCiADDC_END)}, {&&L(OPDiADDC_START), &&L(OPDiADDC_END)},
		{&&L(OPAiSUBC_START), &&L(OPAiSUBC_END)}, {&&L(OPBiSUBC_START), &&L(OPBiSUBC_END)}, {&&L(OPCiSUBC_START), &&L(OPCiSUBC_END)}, {&&L(OPDiSUBC_START), &&L(OPDiSUBC_END)},
		{&&L(OPAPOP_START), &&L(OPAPOP_END)}, {&&L(OPBPOP_START), &&L(OPBPOP_END)}, {&&L(OPCPOP_START), &&L(OPCPOP_END)}, {&&L(OPDPOP_START), &&L(OPDPOP_END)},
		{&&L(OPAPUSH_START), &&L(OPAPUSH_END)}, {&&L(OPBPUSH_START), &&L(OPBPUSH_END)},	{&&L(OPCPUSH_START), &&L(OPCPUSH_END)}, {&&L(OPDPUSH_START), &&L(OPDPUSH_END)},
		{&&L(OPAiPUSHC_START), &&L(OPAiPUSHC_END)},	{&&L(OPBiPUSHC_START), &&L(OPBiPUSHC_END)},	{&&L(OPCiPUSHC_START), &&L(OPCiPUSHC_END)},	{&&L(OPDiPUSHC_START), &&L(OPDiPUSHC_END)},
		{&&L(OPACOPY_START), &&L(OPACOPY_END)}, {&&L(OPBCOPY_START), &&L(OPBCOPY_END)}, {&&L(OPCCOPY_START), &&L(OPCCOPY_END)}, {&&L(OPDCOPY_START), &&L(OPDCOPY_END)},
		{&&L(OPAiJLC_START), &&L(OPAiJLC_END)}, {&&L(OPBiJLC_START), &&L(OPBiJLC_END)}, {&&L(OPCiJLC_START), &&L(OPCiJLC_END)}, {&&L(OPDiJLC_START), &&L(OPDiJLC_END)},
		{&&L(OPAiJGC_START), &&L(OPAiJGC_END)}, {&&L(OPBiJGC_START), &&L(OPBiJGC_END)},	{&&L(OPCiJGC_START), &&L(OPCiJGC_END)}, {&&L(OPDiJGC_START), &&L(OPDiJGC_END)},
		{&&L(OPAFASTCALL_START), &&L(OPAFASTCALL_END)}, {&&L(OPBFASTCALL_START), &&L(OPBFASTCALL_END)}, {&&L(OPCFASTCALL_START), &&L(OPCFASTCALL_END)}, {&&L(OPDFASTCALL_START), &&L(OPDFASTCALL_END)},
		{&&L(OPAPFASTCALL_START), &&L(OPAPFASTCALL_END)}, {&&L(OPBPFASTCALL_START), &&L(OPBPFASTCALL_END)}, {&&L(OPCPFASTCALL_START), &&L(OPCPFASTCALL_END)}, {&&L(OPDPFASTCALL_START), &&L(OPDPFASTCALL_END)},
		{&&L(OPARET_START), &&L(OPARET_END)}, {&&L(OPBRET_START), &&L(OPBRET_END)}, {&&L(OPCRET_START), &&L(OPCRET_END)}, {&&L(OPDRET_START), &&L(OPDRET_END)},
		{&&L(OPAMOV_START), &&L(OPAMOV_END)}, {&&L(OPBMOV_START), &&L(OPBMOV_END)}, {&&L(OPCMOV_START), &&L(OPCMOV_END)}, {&&L(OPDMOV_START), &&L(OPDMOV_END)},
	};
	DISPATCH_START();
	
	CASE(OPMOV) {
		DBG_P("OPMOV");
		START(OPMOV);
		reg[pc->dst] = pc->src;
		pc++;
		END(OPMOV);
		goto NEXTOP;
	}
	CASE(OPADD) {
		DBG_P("OPADD");
		START(OPADD);
		reg[pc->dst] += reg[pc->src];
		pc++;
		END(OPADD);
		goto NEXTOP;
	}
	CASE(OPiADDC) {
		DBG_P("OPiADDC");
		START(OPiADDC);
		reg[pc->dst] += pc->src;
		pc++;
		END(OPiADDC);
		goto NEXTOP;
	}
	CASE(OPSUB) {
		DBG_P("OPSUB");
		START(OPSUB);
		reg[pc->dst] -= reg[pc->src];
		pc++;
		END(OPSUB);
		goto NEXTOP;
	}
	CASE(OPiSUBC) {
		DBG_P("OPiSUBC");
		START(OPiSUBC);
		reg[pc->dst] -= pc->src;
		pc++;
		END(OPiSUBC);
		goto NEXTOP;
	}
	CASE(OPMUL) {
		DBG_P("OPMUL");
		START(OPMUL);
		reg[pc->dst] *= reg[pc->src];
		pc++;
		END(OPMUL);
		goto NEXTOP;
	}
	CASE(OPDIV) {
		DBG_P("OPDIV");
		START(OPDIV);
		reg[pc->dst] /= reg[pc->src];
		pc++;
		END(OPDIV);
		goto NEXTOP;
	}
	CASE(OPCMP) {
		DBG_P("OPCMP");
		START(OPCMP);
		pc += pc->dst;
		END(OPCMP);
		goto NEXTOP;
	}
	CASE(OPJL) {
		DBG_P("OPJL");
		START(OPJL);
		pc += (reg[pc->dst] < reg[pc->src]) ? pc->jmp : 1;
		END(OPJL);
		goto NEXTOP;
	}
	CASE(OPiJLC) {
		DBG_P("OPiJLC");
		START(OPiJLC);
		pc += (reg[pc->dst] < pc->src) ? pc->jmp : 1;
		END(OPiJLC);
		goto NEXTOP;
	}
	CASE(OPJG) {
		DBG_P("OPJG");
		START(OPJG);
		pc += (reg[pc->dst] > reg[pc->src]) ? pc->jmp : 1;
		END(OPJG);
		goto NEXTOP;
	}
	CASE(OPiJGC) {
		DBG_P("OPiJGC");
		START(OPiJGC);
		pc += (reg[pc->dst] > pc->src) ? pc->jmp : 1;
		END(OPiJGC);
		goto NEXTOP;
	}
	CASE(OPLOAD) {
		DBG_P("OPLOAD");
		START(OPLOAD);
		//load map from virtual memory
		void *value = fetch_from_virtual_memory(pc->name);
		if (value == NULL) {
			fprintf(stderr, "[ERROR] : undefined variable\n");
			return 0;
		}
		reg[pc->dst] = (intptr_t)value;
		pc++;
		END(OPLOAD);
		goto NEXTOP;
	}
	CASE(OPSTORE) {
		START(OPSTORE);
		pc++;
		END(OPSTORE);
		goto NEXTOP;
	}
	CASE(OPCALL) {
		DBG_P("OPCALL");
		START(OPCALL);
		VirtualMachineCode *func_vmcode = (VirtualMachineCode *)fetch_from_virtual_memory(pc->name);
		if (func_vmcode == NULL) {
			fprintf(stderr, "[ERROR] : undefined function name [%s]\n", pc->name);
			return 0;
		}
		local_cache_func_vmcode = func_vmcode;
		r->ret_label_stack = &&L_CALLAFTER;
		r->pc_stack = pc;
		memcpy(r->reg_stack, reg, MAX_REG_SIZE);
		pc = local_cache_func_vmcode;
		goto NEXTOP;
	L_CALLAFTER:
		DBG_P("L_CALLAFTER");
		pc = r->pc_stack;
		int res = reg[0];
		DBG_P("res = [%d]", res);
		memcpy(reg, r->reg_stack, MAX_REG_SIZE);
		r--;
		reg[pc->dst] = res;
		pc++;
		END(OPCALL);
		goto NEXTOP;
	}
	CASE(OPJMP) {
		START(OPJMP);
		pc++;
		END(OPJMP);
		goto NEXTOP;
	}
	CASE(OPFASTCALL) {
		DBG_P("OPFASTCALL");
		START(OPFASTCALL);
		r->ret_label_stack = &&L_FASTCALLAFTER;
		r->pc_stack = pc;
		memcpy(r->reg_stack, reg, MAX_REG_SIZE);
		pc = local_cache_func_vmcode;
		goto NEXTOP;
	L_FASTCALLAFTER:
		DBG_P("L_FASTCALLAFTER");
		pc = r->pc_stack;
		int res = reg[0];
		memcpy(reg, r->reg_stack, MAX_REG_SIZE);
		r--;
		reg[pc->dst] = res;
		pc++;
		END(OPFASTCALL);
		goto NEXTOP;
	}
	CASE(OPPFASTCALL) {
		DBG_P("OPPFASTCALL");
		START(OPPFASTCALL);
		//======PUSH SECTION========//
		r++;
		r->function_arg_stack = reg[pc->dst];
		r->ret_label_stack = &&L_PFASTCALLAFTER;
		r->pc_stack = pc;
		memcpy(r->reg_stack, reg, MAX_REG_SIZE);
		//======FASTCALL SECTION======//
		pc = local_cache_func_vmcode;
		goto NEXTOP;
	L_PFASTCALLAFTER:
		DBG_P("L_PFASTCALLAFTER");
		pc = r->pc_stack;
		int res = reg[0];
		memcpy(reg, r->reg_stack, MAX_REG_SIZE);
		r--;
		reg[pc->dst] = res;
		//============================//
		pc++;
		END(OPPFASTCALL);
		goto NEXTOP;
	}
	CASE(OPiPFASTCALL) {
		DBG_P("OPiPFASTCALL");
		START(OPiPFASTCALL);
		//======iPUSHC SECTION========//
		r++;
		r->function_arg_stack = pc->src;
		pc++;
		r->ret_label_stack = &&L_iPFASTCALLAFTER;
		r->pc_stack = pc;
		memcpy(r->reg_stack, reg, MAX_REG_SIZE);
		//======FASTCALL SECTION======//
		pc = local_cache_func_vmcode;
		goto NEXTOP;
	L_iPFASTCALLAFTER:
		DBG_P("L_FASTCALLAFTER");
		pc = r->pc_stack;
		int res = reg[0];
		memcpy(reg, r->reg_stack, MAX_REG_SIZE);
		r--;
		reg[pc->dst] = res;
		//============================//
		pc++;
		END(OPiPFASTCALL);
		goto NEXTOP;
	}
	CASE(OPPUSH) {
		DBG_P("OPPUSH");
		START(OPPUSH);
		r++;
		r->function_arg_stack = reg[pc->dst];
		pc++;
		END(OPPUSH);
		goto NEXTOP;
	}
	CASE(OPiPUSHC) {
		DBG_P("OPiPUSHC");
		START(OPiPUSHC);
		r++;
		r->function_arg_stack = pc->src;
		pc++;
		END(OPiPUSHC);
		goto NEXTOP;
	}
	CASE(OPPOP) {
		DBG_P("OPPOP");
		START(OPPOP);
		cur_arg = r->function_arg_stack;
		reg[pc->dst] = cur_arg;
		pc++;
		END(OPPOP);
		goto NEXTOP;
	}
	CASE(OPCOPY) {
		DBG_P("OPCOPY");
		START(OPCOPY);
		reg[pc->dst] = cur_arg;
		pc++;
		END(OPCOPY);
		goto NEXTOP;
	}
	CASE(OPTHCODE) {
		DBG_P("OPTHCODE");
		VirtualMachine_createDirectThreadingCode(vmcode, jmp_table);
#ifdef USING_SELECTIVE_INLINING
		VirtualMachine_createSelectiveInliningCode(vmcode, jmp_table, block_table);
#endif
		return 0;
	}
	CASE(OPNOP) {
		DBG_P("OPNOP");
		pc++; goto NEXTOP;
	}
	CASE(OPRET) {
		DBG_P("OPRET");
		START(OPRET);
		reg[0] = reg[pc->src];
		END(OPRET);
		RETURN();
	}
	//================================================================//
	CASE(OPABADD) {
		DBG_P("OPABADD : ");
		START(OPABADD);
		reg[0] += reg[1];
		pc++;
		END(OPABADD);
		goto NEXTOP;
	}
	CASE(OPACADD) {
		DBG_P("OPACADD : ");
		START(OPACADD);
		reg[0] += reg[2];
		pc++;
		END(OPACADD);
		goto NEXTOP;
	}
	CASE(OPADADD) {
		DBG_P("OPADADD : ");
		START(OPADADD);
		reg[0] += reg[3];
		pc++;
		END(OPADADD);
		goto NEXTOP;
	}
	CASE(OPBCADD) {
		DBG_P("OPBCADD : ");
		START(OPBCADD);
		reg[1] += reg[2];
		//asm volatile("hoge:");
		//asm volatile("add %2, %0" : "=r"(reg[1]): "0"(reg[1]), "r"(reg[2]));
		//asm volatile("foo:");
		pc++;
		END(OPBCADD);
		goto NEXTOP;
	}
	CASE(OPBDADD) {
		DBG_P("OPBDADD : ");
		START(OPBDADD);
		reg[1] += reg[3];
		pc++;
		END(OPBDADD);
		goto NEXTOP;
	}
	CASE(OPCDADD) {
		DBG_P("OPCDADD : ");
		START(OPCDADD);
		reg[2] += reg[3];
		pc++;
		END(OPCDADD);
		goto NEXTOP;
	}
	CASE(OPAiADDC) {
		DBG_P("OPAiADDC : ");
		START(OPAiADDC);
		reg[0] += pc->src;
		pc++;
		END(OPAiADDC);
		goto NEXTOP;
	}
	CASE(OPBiADDC) {
		DBG_P("OPBiADDC : ");
		START(OPBiADDC);
		reg[1] += pc->src;
		pc++;
		END(OPBiADDC);
		goto NEXTOP;
	}
	CASE(OPCiADDC) {
		DBG_P("OPCiADDC : ");
		START(OPCiADDC);
		reg[2] += pc->src;
		pc++;
		END(OPCiADDC);
		goto NEXTOP;
	}
	CASE(OPDiADDC) {
		DBG_P("OPDiADDC : ");
		START(OPDiADDC);
		reg[3] += pc->src;
		pc++;
		END(OPDiADDC);
		goto NEXTOP;
	}
	CASE(OPAiSUBC) {
		DBG_P("OPAiSUBC : ");
		START(OPAiSUBC);
		reg[0] -= pc->src;
		pc++;
		END(OPAiSUBC);
		goto NEXTOP;
	}
	CASE(OPBiSUBC) {
		DBG_P("OPBiSUBC : ");
		START(OPBiSUBC);
		reg[1] -= pc->src;
		pc++;
		END(OPBiSUBC);
		goto NEXTOP;
	}
	CASE(OPCiSUBC) {
		DBG_P("OPCiSUBC : ");
		START(OPCiSUBC);
		reg[2] -= pc->src;
		pc++;
		END(OPCiSUBC);
		goto NEXTOP;
	}
	CASE(OPDiSUBC) {
		DBG_P("OPDiSUBC : ");
		START(OPDiSUBC);
		reg[3] -= pc->src;
		pc++;
		END(OPDiSUBC);
		goto NEXTOP;
	}
	CASE(OPAPOP) {
		DBG_P("OPAPOP : ");
		START(OPAPOP);
		cur_arg = r->function_arg_stack;
		reg[0] = cur_arg;
		pc++;
		END(OPAPOP);
		goto NEXTOP;
	}
	CASE(OPBPOP) {
		DBG_P("OPBPOP : ");
		START(OPBPOP);
		cur_arg = r->function_arg_stack;
		reg[1] = cur_arg;
		pc++;
		END(OPBPOP);
		goto NEXTOP;
	}
	CASE(OPCPOP) {
		DBG_P("OPCPOP : ");
		START(OPCPOP);
		cur_arg = r->function_arg_stack;
		reg[2] = cur_arg;
		pc++;
		END(OPCPOP);
		goto NEXTOP;
	}
	CASE(OPDPOP) {
		DBG_P("OPDPOP : ");
		START(OPDPOP);
		cur_arg = r->function_arg_stack;
		reg[3] = cur_arg;
		pc++;
		END(OPDPOP);
		goto NEXTOP;
	}
	CASE(OPAPUSH) {
		DBG_P("OPAPUSH : ");
		START(OPAPUSH);
		r++;
		r->function_arg_stack = reg[0];
		pc++;
		END(OPAPUSH);
		goto NEXTOP;
	}
	CASE(OPBPUSH) {
		DBG_P("OPBPUSH : ");
		START(OPBPUSH);
		r++;
		r->function_arg_stack = reg[1];
		pc++;
		END(OPBPUSH);
		goto NEXTOP;
	}
	CASE(OPCPUSH) {
		DBG_P("OPCPUSH : ");
		START(OPCPUSH);
		r++;
		r->function_arg_stack = reg[2];
		pc++;
		END(OPCPUSH);
		goto NEXTOP;
	}
	CASE(OPDPUSH) {
		DBG_P("OPDPUSH : ");
		START(OPDPUSH);
		r++;
		r->function_arg_stack = reg[3];
		pc++;
		END(OPDPUSH);
		goto NEXTOP;
	}
	CASE(OPAiPUSHC) {
		DBG_P("OPAiPUSHC : ");
		START(OPAiPUSHC);
		r++;
		r->function_arg_stack = pc->src;
		pc++;
		END(OPAiPUSHC);
		goto NEXTOP;
	}
	CASE(OPBiPUSHC) {
		DBG_P("OPBiPUSHC : ");
		START(OPBiPUSHC);
		r++;
		r->function_arg_stack = pc->src;
		pc++;
		END(OPBiPUSHC);
		goto NEXTOP;
	}
	CASE(OPCiPUSHC) {
		DBG_P("OPCiPUSHC : ");
		START(OPCiPUSHC);
		r++;
		r->function_arg_stack = pc->src;
		pc++;
		END(OPCiPUSHC);
		goto NEXTOP;
	}
	CASE(OPDiPUSHC) {
		DBG_P("OPDiPUSHC : ");
		START(OPDiPUSHC);
		r++;
		r->function_arg_stack = pc->src;
		pc++;
		END(OPDiPUSHC);
		goto NEXTOP;
	}
	CASE(OPACOPY) {
		DBG_P("OPACOPY : ");
		START(OPACOPY);
		reg[0] = cur_arg;
		pc++;
		END(OPACOPY);
		goto NEXTOP;
	}
	CASE(OPBCOPY) {
		DBG_P("OPBCOPY : ");
		START(OPBCOPY);
		reg[1] = cur_arg;
		pc++;
		END(OPBCOPY);
		goto NEXTOP;
	}
	CASE(OPCCOPY) {
		DBG_P("OPCCOPY : ");
		START(OPCCOPY);
		reg[2] = cur_arg;
		pc++;
		END(OPCCOPY);
		goto NEXTOP;
	}
	CASE(OPDCOPY) {
		DBG_P("OPDCOPY : ");
		START(OPDCOPY);
		reg[3] = cur_arg;
		pc++;
		END(OPDCOPY);
		goto NEXTOP;
	}
	CASE(OPAiJLC) {
		DBG_P("OPAiJLC : ");
		START(OPAiJLC);
		pc += (reg[0] < pc->src) ? pc->jmp : 1;
		END(OPAiJLC);
		goto NEXTOP;
	}
	CASE(OPBiJLC) {
		DBG_P("OPBiJLC : ");
		START(OPBiJLC);
		pc += (reg[1] < pc->src) ? pc->jmp : 1;
		END(OPBiJLC);
		goto NEXTOP;
	}
	CASE(OPCiJLC) {
		DBG_P("OPCiJLC : ");
		START(OPCiJLC);
		pc += (reg[2] < pc->src) ? pc->jmp : 1;
		END(OPCiJLC);
		goto NEXTOP;
	}
	CASE(OPDiJLC) {
		DBG_P("OPDiJLC : ");
		START(OPDiJLC);
		pc += (reg[3] < pc->src) ? pc->jmp : 1;
		END(OPDiJLC);
		goto NEXTOP;
	}
	CASE(OPAiJGC) {
		DBG_P("OPAiJGC : ");
		START(OPAiJGC);
		pc += (reg[0] > pc->src) ? pc->jmp : 1;
		END(OPAiJGC);
		goto NEXTOP;
	}
	CASE(OPBiJGC) {
		DBG_P("OPBiJGC : ");
		START(OPBiJGC);
		pc += (reg[1] > pc->src) ? pc->jmp : 1;
		END(OPBiJGC);
		goto NEXTOP;
	}
	CASE(OPCiJGC) {
		DBG_P("OPCiJGC : ");
		START(OPCiJGC);
		pc += (reg[2] > pc->src) ? pc->jmp : 1;
		END(OPCiJGC);
		goto NEXTOP;
	}
	CASE(OPDiJGC) {
		DBG_P("OPDiJGC : ");
		START(OPDiJGC);
		pc += (reg[3] > pc->src) ? pc->jmp : 1;
		END(OPDiJGC);
		goto NEXTOP;
	}
	CASE(OPAFASTCALL) {
		DBG_P("OPAFASTCALL : ");
		START(OPAFASTCALL);
		r->ret_label_stack = &&L_AFASTCALLAFTER;
		r->pc_stack = pc;
		memcpy(r->reg_stack, reg, regsize);
		pc = local_cache_func_vmcode;
		goto NEXTOP;
	L_AFASTCALLAFTER:
		//DBG_P("L_AFASTCALLAFTER");
		pc = r->pc_stack;
		memcpy(reg, r->reg_stack, regsize);
		r--;
		pc++;
		END(OPAFASTCALL);
		goto NEXTOP;
	}
	CASE(OPBFASTCALL) {
		DBG_P("OPBFASTCALL : ");
		START(OPBFASTCALL);
		r->ret_label_stack = &&L_BFASTCALLAFTER;
		r->pc_stack = pc;
		memcpy(r->reg_stack, reg, regsize);
		pc = local_cache_func_vmcode;
		goto NEXTOP;
	L_BFASTCALLAFTER:
		//DBG_P("L_BFASTCALLAFTER");
		pc = r->pc_stack;
		int res = reg[0];
		memcpy(reg, r->reg_stack, regsize);
		r--;
		reg[1] = res;
		pc++;
		END(OPBFASTCALL);
		goto NEXTOP;
	}
	CASE(OPCFASTCALL) {
		DBG_P("OPCFASTCALL : ");
		START(OPCFASTCALL);
		r->ret_label_stack = &&L_CFASTCALLAFTER;
		r->pc_stack = pc;
		memcpy(r->reg_stack, reg, regsize);
		pc = local_cache_func_vmcode;
		goto NEXTOP;
	L_CFASTCALLAFTER:
		//DBG_P("L_CFASTCALLAFTER");
		pc = r->pc_stack;
		int res = reg[0];
		memcpy(reg, r->reg_stack, regsize);
		r--;
		reg[2] = res;
		pc++;
		END(OPCFASTCALL);
		goto NEXTOP;
	}
	CASE(OPDFASTCALL) {
		DBG_P("OPDFASTCALL : ");
		START(OPDFASTCALL);
		r->ret_label_stack = &&L_DFASTCALLAFTER;
		r->pc_stack = pc;
		memcpy(r->reg_stack, reg, regsize);
		pc = local_cache_func_vmcode;
		goto NEXTOP;
	L_DFASTCALLAFTER:
		//DBG_P("L_DFASTCALLAFTER");
		pc = r->pc_stack;
		int res = reg[0];
		memcpy(reg, r->reg_stack, regsize);
		r--;
		reg[3] = res;
		pc++;
		END(OPDFASTCALL);
		goto NEXTOP;
	}
	CASE(OPARET) {
		DBG_P("OPARET : ");
		START(OPARET);
		END(OPARET);
		RETURN();
	}
	CASE(OPBRET) {
		DBG_P("OPBRET : ");
		START(OPBRET);
		reg[0] = reg[1];
		END(OPBRET);
		RETURN();
	}
	CASE(OPCRET) {
		DBG_P("OPCRET : ");
		START(OPCRET);
		reg[0] = reg[2];
		END(OPCRET);
		RETURN();
	}
	CASE(OPDRET) {
		DBG_P("OPDRET : ");
		START(OPDRET);
		reg[0] = reg[3];
		END(OPDRET);
		RETURN();
	}
	CASE(OPAMOV) {
		DBG_P("OPAMOV : ");
		START(OPAMOV);
		reg[0] = pc->src;
		pc++;
		END(OPAMOV);
		goto NEXTOP;
	}
	CASE(OPBMOV) {
		DBG_P("OPBMOV : ");
		START(OPBMOV);
		reg[1] = pc->src;
		pc++;
		END(OPBMOV);
		goto NEXTOP;
	}
	CASE(OPCMOV) {
		DBG_P("OPCMOV : ");
		START(OPCMOV);
		reg[2] = pc->src;
		pc++;
		END(OPCMOV);
		goto NEXTOP;
	}
	CASE(OPDMOV) {
		DBG_P("OPDMOV : ");
		START(OPDMOV);
		reg[3] = pc->src;
		pc++;
		END(OPDMOV);
		goto NEXTOP;
	}
	CASE(OPAPFASTCALL) {
		DBG_P("OPAPFASTCALL : ");
		START(OPAPFASTCALL);
		PUSH(reg, reg[0], &&L_APFASTCALLAFTER, pc);
		pc = local_cache_func_vmcode;
		goto NEXTOP;
	L_APFASTCALLAFTER:;
		//DBG_P("L_APFASTCALLAFTER");
		POP();
		pc++;
		END(OPAPFASTCALL);
		goto NEXTOP;
	}
	CASE(OPBPFASTCALL) {
		DBG_P("OPBPFASTCALL : ");
		START(OPBPFASTCALL);
		PUSH(reg, reg[1], &&L_BPFASTCALLAFTER, pc);
		pc = local_cache_func_vmcode;
		goto NEXTOP;
	L_BPFASTCALLAFTER:;
		//DBG_P("L_BPFASTCALLAFTER");
		int res = reg[0];
		POP();
		reg[1] = res;
		pc++;
		END(OPBPFASTCALL);
		goto NEXTOP;
	}
	CASE(OPCPFASTCALL) {
		DBG_P("OPCPFASTCALL : ");
		START(OPCPFASTCALL);
		PUSH(reg, reg[2], &&L_CPFASTCALLAFTER, pc);
		pc = local_cache_func_vmcode;
		goto NEXTOP;
	L_CPFASTCALLAFTER:;
		//DBG_P("L_CPFASTCALLAFTER");
		int res = reg[0];
		POP();
		reg[2] = res;
		pc++;
		END(OPCPFASTCALL);
		goto NEXTOP;
	}
	CASE(OPDPFASTCALL) {
		DBG_P("OPDPFASTCALL : ");
		START(OPDPFASTCALL);
		PUSH(reg, reg[3], &&L_DPFASTCALLAFTER, pc);
		pc = local_cache_func_vmcode;
		goto NEXTOP;
	L_DPFASTCALLAFTER:;
		//DBG_P("L_DPFASTCALLAFTER");
		int res = reg[0];
		POP();
		reg[3] = res;
		pc++;
		END(OPDPFASTCALL);
		goto NEXTOP;
	}
	//================= Selective Inlining Code ===================//
	CASE(GOTO_BLOCK) {
		DBG_P("GOTO_BLOCK : ");
		pc->code();
		goto NEXTOP;
	}
	CASE(RET_BLOCK) {
		DBG_P("RET_BLOCK : ");
		pc->code();
		RETURN();
	}
	//==============================================================//
	DISPATCH_END();
	return reg[0];
}

static void VirtualMachine_setVariable(VirtualMachineCode *vmcode, int size, int var)
{
	if (isSetFlag && isSETQ) {
		DBG_P("execute set variable");
		VirtualMachineCode *store = NULL;
		if ((vmcode + size)->op == OPSTORE) {
			store = vmcode + size;
		} else if ((vmcode + size - 1)->op == OPSTORE) {
			store = vmcode + size - 1;
		} else {
			DBG_P("OPSTORE is cannnot find because isSETQ flag is true");
		}
		DBG_P("store->name = [%s]", store->name);
		DBG_P("var = [%d]", var);
		intptr_t v = var;
		GMap *map = new_GMap(store->name, (void *)v, SETQ);
		store_to_virtual_memory(map);
	}
}

static void VirtualMachine_setFunction(VirtualMachineCode *vmcode, int size)
{
	if (isSetFlag && isDEFUN) {
		DBG_P("store vmcode to virtual memory");
		VirtualMachineCode *store = NULL;
		if ((vmcode + size)->op == OPSTORE) {
			store = vmcode + size;
		} else if ((vmcode + size - 1)->op == OPSTORE) {
			store = vmcode + size - 1;
		} else {
			DBG_P("OPSTORE is cannnot find because isDEFUN flag is true");
		}
		GMap *map = new_GMap(store->name, (void *)vmcode, DEFUN);
		store_to_virtual_memory(map);
	}
}

static void VirtualMachine_clear(void)
{
	stack_count = 0;
	local_func_code = NULL;
	isExecFlag = 1;
	isConditionFlag = 0;
	isSetFlag = 0;
	isSETQ = 0;
	isDEFUN = 0;
	ret_call_count = 0;
	false_jump_register = 0;
}

static void VirtualMachine_delete(VirtualMachine *vm)
{
	free(vm);
	vm = NULL;
}

VirtualMachine *new_VirtualMachine(void)
{
	VirtualMachine *ret = (VirtualMachine *)gmalloc(sizeof(VirtualMachine));
	ret->run = VirtualMachine_run;
	ret->setVariable = VirtualMachine_setVariable;
	ret->setFunction = VirtualMachine_setFunction;
	ret->clear = VirtualMachine_clear;
	ret->free = VirtualMachine_delete;
	return ret;
}
