#include <glisp.h>

static int brace_count = 0;
static void glisp_init_table(void)
{
	hash_table = new_HashTable();
	search_htop = hash_table;
	func_table = new_FuncTable();
	search_ftop = func_table;
	func_ptr = NULL;
}

static void glisp_init(void)
{
	using_history();
	add_history("(defun fib(n) (if (< n 3) 1 (+ (fib(- n 2)) (fib(- n 1)))))");
	//add_history("(defun tarai(x y z)(if (<= x y) y ( (tarai( (tarai((- x 1) y z)) (tarai((- y 1) z x)) (tarai((- z 1) x y)))))))");
	add_history("(fib 36)");
	add_history("(setq x 4)");
	add_history("(defun func(n) (+ n 2))");
	glisp_init_table();
	brace_count = 0;
}

/*
static void glisp_display_result(Conscell *ans)
{
	if (ans != NULL) {
		if (ans->type == BOOL || ans->type == STR_BOOL) {
			if (ans->result == 1) {
				printf("true\n");
			} else {
				printf("false\n");
			}
		} else if (ans->type != SETQ &&
				   ans->type != FUNC) {
			printf("%d\n", ans->result);
		} 
	}
}
*/
static int glisp_check_brace(char *line)
{
	//brace_count is global variable
	int i = 0;
	while (line[i] != EOL) {
		if (line[i] == '(') {
			brace_count++;
		} else if (line[i] == ')') {
			brace_count--;
		}
		i++;
	}
	return brace_count;
}

void glisp_start_shell(void)
{
	char *line = NULL;
	char tmp[128] = {0};
	glisp_init();
	Tokenizer *t = new_Tokenizer();
	Parser *p = new_Parser();
	while (true) {
		if (line == NULL) {
			line = readline(">>>");
		}
		if (!strncmp(line, "quit", sizeof("quit")) ||
			!strncmp(line, "exit", sizeof("exit"))) {
			deleteHashTable();
			deleteFuncTable(p);
			exit(0);
		}
		add_history(line);
		int check = glisp_check_brace(line);
		if (check == 0) {
			strcat(tmp, " ");
			strcat(tmp, line);
			char **token = t->split(tmp);
			if (token != NULL && token[0] != NULL && !strncmp(token[0], "(", sizeof("("))) {
				Conscell *root = p->parse(token);
				Compiler *c = new_Compiler();
				VirtualMachineCodeArray *vmcode = c->compile(c, root->cdr);
				VirtualMachine *vm = new_VirtualMachine();
				//int ret = vm->run(vmcode);
				DBG_P("===================");
				//fprintf(stderr, "ans = [%d]\n", ret);
				//Conscell *ret = eval(root->cdr);
				//displayResult(ret);
				t->delete(token);
				p->delete(root);
			}
			tmp[0] = EOL;
			free(line);
			line = NULL;
		} else if (check > 0) {
			strcat(tmp, " ");
			strcat(tmp, line);
			free(line);
			line = readline("...");
		} else {
			fprintf(stderr, "Syntax error!!\n");
			tmp[0] = EOL;
			free(line);
			line = NULL;
			brace_count = 0;//brace_count is global variable
		}
	}
}

void glisp_start_script(char *file_name)
{
	char line[128] = {0};
	char tmp[128] = {0};
	FILE *fp = NULL;
	fp = fopen(file_name, "r");
	if (fp == NULL) {
		fprintf(stderr, "file open error!!\n");
		fclose(fp);
		exit(EXIT_FAILURE);
		return;
	}
	glisp_init_table();
	Tokenizer *t = new_Tokenizer();
	Parser *p = new_Parser();
	while (fgets(line, MAX_LINE_SIZE, fp) != NULL) {
		int check = glisp_check_brace(line);
		if (check == 0) {
			strcat(tmp, line);
			printf("src = %s\n", tmp);
			char **token = t->split(tmp);
			if (token != NULL && token[0] != NULL && !strncmp(token[0], "(", sizeof("("))) {
				Conscell *root = p->parse(token);
				Compiler *c = new_Compiler();
				VirtualMachineCodeArray *vmcode = c->compile(c, root->cdr);
				//vmcode->dump(vmcode);
				VirtualMachine *vm = new_VirtualMachine();
				c->compileToFastCode(vmcode);
				VirtualMachineCode *thcode = new_VirtualMachineCode(NULL, 0);
				thcode->op = OPTHCODE;
				vmcode->add(vmcode, thcode);
				vmcode->reverse(vmcode);
				int vm_stack_top = vmcode->size - 1;
				VirtualMachineCode *code = vmcode->getPureCode(vmcode);
				vm->run(code);//direct threading compile time & not execute
				if (c->isExecFlag) {
					int ret = vm->run(code);//execute
					vm->setVariable(code, vm_stack_top, ret);
					fprintf(stderr, "ans = [%d]\n", ret);
				} else {
					vm->setFunction(code, vm_stack_top);
				}
				vm->clear();
				DBG_P("===================");
				//Conscell *ret = eval(root->cdr);
				t->delete(token);
				p->delete(root);
			}
			tmp[0] = EOL;
		} else if (check > 0) {
			strcat(tmp, line);
		} else {
			printf("Syntax error!!\n");
			exit(EXIT_FAILURE);
		}
	}
	fclose(fp);
	deleteHashTable();
	deleteFuncTable(p);
}
