#include <glisp.h>

static int brace_count = 0;

static void glisp_init(void)
{
	usingHistory();
	addHistory("(defun fib(n) (if (< n 3) 1 (+ (fib(- n 2)) (fib(- n 1)))))");
	//add_history("(defun tarai(x y z)(if (<= x y) y ( (tarai( (tarai((- x 1) y z)) (tarai((- y 1) z x)) (tarai((- z 1) x y)))))))");
	addHistory("(fib 36)");
	addHistory("(setq x 4)");
	addHistory("(defun func(n) (+ n 2))");
	brace_count = 0;
}

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

static void glisp_main(Tokenizer *t, Parser *p, char *line)
{
	char **token = t->split(line);
	if (token != NULL && token[0] != NULL && !strncmp(token[0], "(", sizeof("("))) {
		Conscell *root = p->parse(token);
		Compiler *c = new_Compiler();
		VirtualMachineCodeArray *vmcode = c->compile(c, root->cdr);
		VirtualMachine *vm = new_VirtualMachine();
		vmcode->dump(vmcode);
		c->compileToFastCode(vmcode);
		VirtualMachineCode *thcode = new_VirtualMachineCode(NULL, 0);
		thcode->op = OPTHCODE;
		vmcode->add(vmcode, thcode);
		c->fixRegNumber(vmcode);
		if (c->getMaxRegNumber(vmcode) < 4) {
			c->finalCompile(vmcode);
		}
		vmcode->reverse(vmcode);
		vmcode->dump(vmcode);
		VirtualMachineCode *code = vmcode->getPureCode(vmcode);
		vm->run(code);//direct threading compile time & not execute
		int vm_stack_top = vmcode->size - 2;
		code++;//Skip OPNOP
		if (c->isExecFlag) {
			int ret = 0;
			ret = vm->run(code);//execute
			vm->setVariable(code, vm_stack_top, ret);
			fprintf(stderr, "%d\n", ret);
			free(--code);
		} else {
			vm->setFunction(code, vm_stack_top);
		}
		DBG_P("===================");
		vm->clear();
		vm->free(vm);
		vmcode->free(vmcode);
		c->free(c);
		t->free(token);
		p->free(root);
	}
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
			line = greadline(">>> ");
		}
		if (!strncmp(line, "quit", sizeof("quit")) ||
			!strncmp(line, "exit", sizeof("exit"))) {
			exit(0);
		}
		addHistory(line);
		int check = glisp_check_brace(line);
		if (check == 0) {
			strcat(tmp, " ");
			strcat(tmp, line);
			glisp_main(t, p, line);
			tmp[0] = EOL;
			free(line);
			line = NULL;
		} else if (check > 0) {
			strcat(tmp, " ");
			strcat(tmp, line);
			free(line);
			line = greadline("...");
		} else {
			fprintf(stderr, "Syntax error!!\n");
			tmp[0] = EOL;
			free(line);
			line = NULL;
			brace_count = 0;//brace_count is global variable
		}
	}
	clear_virtual_memory();
	free(t);
	free(p);
}

void glisp_start_script(char *file_name)
{
	char line[128] = {0};
	char tmp[128] = {0};
	FILE *fp = fopen(file_name, "r");
	if (fp == NULL) {
		fprintf(stderr, "file open error!!\n");
		fclose(fp);
		exit(EXIT_FAILURE);
		return;
	}
	Tokenizer *t = new_Tokenizer();
	Parser *p = new_Parser();
	while (fgets(line, MAX_LINE_SIZE, fp) != NULL) {
		int check = glisp_check_brace(line);
		if (check == 0) {
			strcat(tmp, line);
			printf("src = %s\n", tmp);
			glisp_main(t, p, tmp);
			tmp[0] = EOL;
		} else if (check > 0) {
			strcat(tmp, line);
		} else {
			printf("Syntax error!!\n");
			exit(EXIT_FAILURE);
		}
	}
	fclose(fp);
	clear_virtual_memory();
	free(t);
	free(p);
}
