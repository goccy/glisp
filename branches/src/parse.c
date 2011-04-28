#include <glisp.h>

static void Conscell_printTypeName(Conscell *cell)
{
	switch (cell->type) {
	case ADD:
		DBG_P("ADD");
		break;
	case SUB:
		DBG_P("SUB");
		break;
	case MULTI:
		DBG_P("MULTI");
		break;
	case DIV:
		DBG_P("DIV");
		break;
	case GRATER:
		DBG_P("GRATER");
		break;
	case LESS:
		DBG_P("LESS");
		break;
	case EQUAL:
		DBG_P("EQUAL");
		break;
	case IF:
		DBG_P("IF");
		break;
	case DEFUN:
		DBG_P("DEFUN");
		break;
	case SETQ:
		DBG_P("SETQ");
		break;
	case STRING:
		DBG_P("STRING : %s", cell->string);
		break;
	case NUM:
		DBG_P("NUM : %d", cell->num);
		break;
	case LEFT_PARENTHESIS:
		DBG_P("LEFT_PARENTHESIS");
		break;
	case RIGHT_PARENTHESIS:
		DBG_P("RIGHT_PARENTHESIS");
		break;
	case FUNC:
		DBG_P("FUNC");
		break;
	case FUNC_NAME:
		DBG_P("FUNC_NAME");
		break;
	case FUNC_ARGS:
		DBG_P("FUNC_ARGS");
		break;
	default:
		break;
	}
}

static Conscell* new_Conscell(void)
{
	Conscell *ret = (Conscell *)gmalloc(sizeof(Conscell));
	ret->car = NULL;
	ret->string = NULL;
	ret->num = 0;
	ret->cdr = NULL;
	ret->type = 0;
	ret->result = 0;//deprecated
	ret->printTypeName = Conscell_printTypeName;
	return ret;
}

static Conscell* Parser_parse(char **token)
{
	int i = 0;
	int isFunctionCall = 0;//call flag
	int isFunctionDefinition = 0;//defun flag
	Conscell *root = new_Conscell();
	Conscell *path = root;
	Conscell *branch[MAX_BRANCH_SIZE] = {NULL};
	int branch_num = 0;

	while (token[i] != EOL) {
		if (!strncmp(token[i], "(", sizeof("("))) {
			path->cdr = new_Conscell();
			path = path->cdr;
			path->type = LEFT_PARENTHESIS;
			branch[branch_num] = path;
			branch_num++;
		} else if (!strncmp(token[i], ")", sizeof(")"))) {
			path->cdr = NULL;
			path = branch[branch_num - 1];
			branch_num--;
		} else if (!strncmp(token[i], "+", sizeof("+"))) {
			path->car = new_Conscell();
			path = path->car;
			path->type = ADD;
		} else if (!strncmp(token[i], "-", sizeof("-"))) {
			path->car = new_Conscell();
			path = path->car;
			path->type = SUB;
		} else if (!strncmp(token[i], "*", sizeof("*"))) {
			path->car = new_Conscell();
			path = path->car;
			path->type = MULTI;
		} else if (!strncmp(token[i], "/", sizeof("/"))) {
			path->car = new_Conscell();
			path = path->car;
			path->type = DIV;
		} else if (!strncmp(token[i], "<", sizeof("<"))) {
			path->car = new_Conscell();
			path = path->car;
			path->type = LESS;
		} else if (!strncmp(token[i], ">", sizeof(">"))) {
			path->car = new_Conscell();
			path = path->car;
			path->type = GRATER;
		} else if (!strncmp(token[i], "<=", sizeof("<="))) {
			path->car = new_Conscell();
			path = path->car;
			path->type = LESS_EQUAL;
		} else if (!strncmp(token[i], ">=", sizeof(">="))) {
			path->car = new_Conscell();
			path = path->car;
			path->type = GRATER_EQUAL;
		} else if (!strncmp(token[i], "=", sizeof("="))) {
			path->car = new_Conscell();
			path = path->car;
			path->type = EQUAL;
		} else if (!strncmp(token[i], "if", sizeof("if"))) {
			path->car = new_Conscell();
			path = path->car;
			path->type = IF;
		} else if (!strncmp(token[i], "setq", sizeof("setq"))) {
			path->car = new_Conscell();
			path = path->car;
			path->type = SETQ;
		} else if (!strncmp(token[i], "defun", sizeof("defun"))) {
			path->car = new_Conscell();
			path = path->car;
			path->type = DEFUN;
			isFunctionDefinition = 1;
		} else if (token[i][0] == '-') {
			//sub number
			int num = atoi(token[i]);
			path->cdr = new_Conscell();
			path = path->cdr;
			if (isFunctionCall && !isFunctionDefinition) {
				path->type = FUNC_ARGS;
			} else {
				path->type = NUM;
			}
			path->num = num;
		} else if (isdigit(token[i][0])) {
			//number
			int num = atoi(token[i]);
			path->cdr = new_Conscell();
			path = path->cdr;
			if (isFunctionCall && !isFunctionDefinition) {
				path->type = FUNC_ARGS;
			} else {
				path->type = NUM;
			}
			path->num = num;
		} else if (isalpha(token[i][0])) {
			//string
			if (!strncmp(token[i - 1], "(", sizeof("("))) {
				//func name
				path->car = new_Conscell();
				path = path->car;
				path->type = FUNC;
				isFunctionCall = 1;
			} else {
				path->cdr = new_Conscell();
				path = path->cdr;
				path->type = STRING;
			}
			path->string = (char *)gmalloc(strlen(token[i]) + 1);
			sprintf(path->string, "%s", token[i]);
		}
		i++;
	}
	return root;
}

static void Parser_delete(Conscell *path)
{
	Conscell *tmp = NULL;
	if (path->type == STRING || path->type == FUNC) {
		if (path->cdr != NULL) {
			free(path->car);
			tmp = path->cdr;
			free(path);
			path = tmp;
			Parser_delete(path);
		} else {
			free(path->car);
			free(path);
		}
	} else if (path->type == FUNC_NAME || path->type == FUNC_ARGS) {
		if (path->cdr != NULL) {
			tmp = path->cdr;
			free(path);
			path = tmp;
			Parser_delete(path);
		} else {
			free(path);
		}
	} else if (path->type == NUM || path->type == BOOL ||
			   path->type == STR_BOOL || path->type == SETQ) {
		if (path->cdr != NULL) {
			tmp = path->cdr;
			free(path);
			path = tmp;
			Parser_delete(path);
		} else {
			free(path);
		}
	} else if (path->car != NULL && path->cdr != NULL) {
		tmp = path->car;
		Parser_delete(tmp);
		tmp = path->cdr;
		free(path);
		path = tmp;
		Parser_delete(path);
	} else if (path->car != NULL && path->cdr == NULL) {
		tmp = path->car;
		free(path);
		path = tmp;
		Parser_delete(path);
	} else if (path->car == NULL && path->cdr != NULL) {
		tmp = path->cdr;
		free(path);
		path = tmp;
		Parser_delete(path);
	}
}

Parser *new_Parser(void)
{
	Parser *p = (Parser *)gmalloc(sizeof(Parser));
	p->parse = Parser_parse;
	p->delete = Parser_delete;
	return p;
}
