#include <glisp.h>

static int arg_stack[MAX_ARGS][4096];
static int sfp[MAX_ARGS];
static int arg_n;//save num of args
static int ARGS_N;//all num of args 

static void evalArgs(Conscell *path);
static int evalCar(Conscell *path);
static int evalCdr(Conscell *path);
static Conscell* evalAdd(Conscell *path);
static Conscell* evalSub(Conscell *path);
static Conscell* evalMulti(Conscell *path);
static Conscell* evalDiv(Conscell *path);
static Conscell* evalGrater(Conscell *path);
static Conscell* evalLess(Conscell *path);
static Conscell* evalGraterEqual(Conscell *path);
static Conscell* evalLessEqual(Conscell *path);
static Conscell* evalEqual(Conscell *path);
static Conscell* evalIf(Conscell *path);
static Conscell* evalSetq(Conscell *path);
static Conscell* evalDefun(Conscell *path);
static Conscell* evalFunc(Conscell *path);


Conscell* eval(Conscell *path)
{
  Conscell *ret = NULL;
  if (path->car != NULL) {
	path = path->car;
  }
  switch (path->type) {
  case ADD:
	ret = evalAdd(path);
	break;
  case SUB:
	ret = evalSub(path);
	break;
  case MULTI:
	ret = evalMulti(path);
	break;
  case DIV:
	ret = evalDiv(path);
	break;
  case GRATER:
	ret = evalGrater(path);
	break;
  case LESS:
	ret = evalLess(path);
	break;
  case EQUAL:
	ret = evalEqual(path);
	break;
  case GRATER_EQUAL:
	ret = evalGraterEqual(path);
	break;
  case LESS_EQUAL:
	ret = evalLessEqual(path);
	break;
  case IF:
	ret = evalIf(path);
	break;
  case SETQ:
	ret = evalSetq(path);
	break;
  case DEFUN:
	ret = evalDefun(path);
	break;
  case FUNC:
	ret = evalFunc(path);
	break;
  default:
	printf("Syntax error !!\n");
	break;
  }
  return ret;
}

static void evalArgs(Conscell *path)
{
  int args[MAX_ARGS] = {0};
  int ret_n = 0;
  int i = 0;
  while (path->cdr != NULL) {
	path = path->cdr;
	if (path->type != LEFT_PARENTHESIS) {
	  ret_n = evalCdr(path);
	} else {
	  ret_n = evalCar(path);
	}
	arg_n = i;
	args[i] = ret_n;
	i++;
  }
  for (i = 0; i < ARGS_N; i++) {
	sfp[i]++;
	arg_stack[i][sfp[i]] = args[i];
  }
}

static int evalCar(Conscell *path)
{
  int ret_n = 0;
  Conscell *branch = NULL;
  if (path->car->type == FUNC_NAME) {
	evalArgs(path->car);
	branch = eval(path->car->car);
	ret_n = branch->result;
	/*=========FIRST TIME CALL==========*/
  } else if (path->car->type == FUNC && func_ptr != NULL) {
	FuncTable *f = getFuncTable(path->car->string);
	if (f != NULL) {
	  evalArgs(path->car);
	  path->car->type = FUNC_NAME;//FAST CALL FLAG
	  func_ptr = f;
	  path->car->car = f->root;
	  branch = eval(path->car->car);
	  ret_n = branch->result;
	}
	/*==================================*/
  } else {
	branch = eval(path);
	ret_n = branch->result;
  }
  return ret_n;
}

static int evalCdr(Conscell *path)
{
  int ret_n = 0;
  int value = 0;
  if (path->type == FUNC_ARGS) {
	arg_n = path->num;
	ret_n = arg_stack[arg_n][sfp[arg_n]];
  } else if (path->type == NUM) {
	ret_n = path->num;
  } else {
	/*=============FIRST TIME CALL===========*/
	if (func_ptr != NULL) {
	  int i = 0;
	  while (func_ptr->args[i] != EOL) {
		if (!strncmp(path->string, func_ptr->args[i], sizeof(func_ptr->args[i]))) {
		  path->num = i;
		  path->type = FUNC_ARGS;//FAST CALL FLAG
		  value = arg_stack[i][sfp[i]];
		  arg_n = i;
		  break;
		}
		i++;
	  }
	  /*=====================================*/
	} else {
	  value = getValFromHashTable(path->string);
	}
	ret_n = value;
  }
  return ret_n;
}

static Conscell* evalAdd(Conscell *path)
{
  int ret_n = 0;
  do {
	path = path->cdr;
	if (path->type != LEFT_PARENTHESIS) {
	  ret_n += evalCdr(path);
	} else {
	  ret_n += evalCar(path);
	}
  } while (path->cdr != NULL);
  path->result = ret_n;
  return path;
}

static Conscell* firstTerm(Conscell *path)
{
  int ret_n = 0;
  path = path->cdr;
  if (path->type != LEFT_PARENTHESIS) {
	ret_n = evalCdr(path);
  } else {
	ret_n = evalCar(path);
  }
  path->result = ret_n;
  return path;
}

static Conscell* evalSub(Conscell *path)
{
  int ret_n = 0;
  Conscell *branch = NULL;
  branch = firstTerm(path);
  ret_n = branch->result;
  path = path->cdr;
  do {
	path = path->cdr;
	if (path->type != LEFT_PARENTHESIS) {
	  ret_n -= evalCdr(path);
	} else {
	  ret_n -= evalCar(path);
	}
  } while (path->cdr != NULL);
  path->result = ret_n;
  return path;
}

static Conscell* evalMulti(Conscell *path)
{
  int ret_n = 0;
  Conscell *branch = NULL;
  branch = firstTerm(path);
  ret_n += branch->result;
  path = path->cdr;
  do {
	path = path->cdr;
	if (path->type != LEFT_PARENTHESIS) {
	  ret_n *= evalCdr(path);
	} else {
	  ret_n *= evalCar(path);
	}
  } while (path->cdr != NULL);
  path->result = ret_n;
  return path;
}

static Conscell* evalDiv(Conscell *path)
{
  int ret_n = 0;
  Conscell *branch = NULL;
  branch = firstTerm(path);
  ret_n += branch->result;
  path = path->cdr;
  do {
	path = path->cdr;
	if (path->type != LEFT_PARENTHESIS) {
	  ret_n /= evalCdr(path);
	} else {
	  ret_n /= evalCar(path);
	}
  } while (path->cdr != NULL);
  path->result = ret_n;
  return path;
}

static Conscell* evalGrater(Conscell *path)
{
  int max_n = 0;
  int value = 0;
  Conscell *branch = firstTerm(path);
  max_n = branch->result;
  path = path->cdr;
  do {
	path = path->cdr;
	if (path->type != LEFT_PARENTHESIS) {
	  value = evalCdr(path);
	} else {
	  value = evalCar(path);
	}
	if (value >= max_n) {
	  //path->type = BOOL;
	  path->result = 0;
	  return path;
	} else {
	  max_n = value;
	}
  } while (path->cdr != NULL);
  //path->result = BOOL;
  path->result = 1;
  return path;
}

static Conscell* evalGraterEqual(Conscell *path)
{
  int max_n = 0;
  int value = 0;
  Conscell *branch = firstTerm(path);
  max_n = branch->result;
  path = path->cdr;
  do {
	path = path->cdr;
	if (path->type != LEFT_PARENTHESIS) {
	  value = evalCdr(path);
	} else {
	  value = evalCar(path);
	}
	if (value > max_n) {
	  //path->type = BOOL;
	  path->result = 0;
	  return path;
	} else {
	  max_n = value;
	}
  } while (path->cdr != NULL);
  //path->type = BOOL;
  path->result = 1;
  return path;
}

static Conscell* evalLess(Conscell *path)
{
  int max_n = 0;
  int value = 0;
  Conscell *branch = firstTerm(path);
  max_n += branch->result;
  path = path->cdr;
  do {
	path = path->cdr;
	if (path->type != LEFT_PARENTHESIS) {
	  value = evalCdr(path);
	} else {
	  value = evalCar(path);
	}
	if (value <= max_n) {
	  //path->type = BOOL;
	  path->result = 0;
	  return path;
	} else {
	  max_n = value;
	}
  } while (path->cdr != NULL);
  //path->type = BOOL;
  path->result = 1;
  return path;
}

static Conscell* evalLessEqual(Conscell *path)
{
  int max_n = 0;
  int value = 0;
  Conscell *branch = firstTerm(path);
  max_n = branch->result;
  path = path->cdr;
  do {
	path = path->cdr;
	if (path->type != LEFT_PARENTHESIS) {
	  value = evalCdr(path);
	} else {
	  value = evalCar(path);
	}
	if (value < max_n) {
	  //path->type = BOOL;
	  path->result = 0;
	  return path;
	} else {
	  max_n = value;
	}
  } while (path->cdr != NULL);
  //path->type = BOOL;
  path->result = 1;
  return path;
}

static Conscell* evalEqual(Conscell *path)
{
  int max_n = 0;
  int value = 0;
  Conscell *branch = firstTerm(path);
  max_n = branch->result;
  path = path->cdr;
  do {
	path = path->cdr;
	if (path->type != LEFT_PARENTHESIS) {
	  value = evalCdr(path);
	} else {
	  value = evalCar(path);
	}
	if (value != max_n) {
	  //path->type = BOOL;
	  path->result = 0;
	  return path;
	} else {
	  max_n = value;
	}
  } while (path->cdr != NULL);
  //path->type = BOOL;
  path->result = 1;
  return path;
}

static Conscell* evalIf(Conscell *path)
{
  int ret_n = 0;
  path = path->cdr;
  Conscell *condition = eval(path);
  if (condition->result) {
	//true
	path = path->cdr;
	if (path->type != LEFT_PARENTHESIS) {
	  ret_n = evalCdr(path);
	} else {
	  ret_n = evalCar(path);
	}
  } else {
	//false
	path = path->cdr->cdr;
	if (path->type != LEFT_PARENTHESIS) {
	  ret_n = evalCdr(path);
	} else {
	  ret_n = evalCar(path);
	}
  }
  path->result = ret_n;
  int i = 0;
  for (i = 0; i < ARGS_N; i++) {
	sfp[i]--;
  }
  return path;
}

static Conscell* evalSetq(Conscell *path)
{
  path = path->cdr;
  char *key = path->string;
  path = path->cdr;
  int value = 0;
  if (path->type != LEFT_PARENTHESIS) {
	value = evalCdr(path);
  } else {
	value = evalCar(path);
  }
  setToHashTable(key, value);
  printf("{%s: %d}\n", key, value);
  path->type = SETQ;
  return path;
}

static Conscell* evalDefun(Conscell *path)
{
  char *args[MAX_ARGS] = {NULL};
  path = path->cdr;
  char *func_name = path->string;
  path = path->cdr;
  Conscell *branch = path;
  if (path->car != NULL) {//For void func
	path = path->car;
	args[0] = path->string;
	int i = 1;
	while (path->cdr != NULL) {
	  path = path->cdr;
	  args[i] = path->string;
	  i++;
	}
  }
  Conscell *tmp = branch;
  branch = branch->cdr;
  setToFuncTable(func_name, args, branch);
  tmp->cdr = NULL;
  path->type = FUNC;
  return path;
}

static Conscell* evalFunc(Conscell *path)
{
  char *func_name = path->string;
  FuncTable *f = getFuncTable(func_name);
  if (f == NULL) {
	return path;
  }
#ifdef DEBUG
  printf("func_name = %s\n", f->func_name);
  int debug = 0;
  for (debug = 0; f->args[debug] != EOL; debug++) {
	printf("args[%d] = %s\n", debug, f->args[debug]);
  }
  printf("root_ptr = %p\n", f->root);
#endif
  /*================regist args to Hash Table================*/
  int value[MAX_ARGS] = {0};
  char *del_keys[MAX_ARGS] = {NULL};
  char *mod_keys[MAX_ARGS] = {NULL};
  int i = 0, j = 0, k = 0;
  for (i = 0; f->args[i] != EOL; i++) {
	path = path->cdr;
	if (searchHashTable(f->args[i]) == NULL) {
	  setToHashTable(f->args[i], path->num);//temporary register
	  del_keys[j] = f->args[i];//delete flag
	  j++;
	} else {
	  value[k] = getValFromHashTable(f->args[i]);//save value
	  mod_keys[k] = f->args[i];//modify flag
	  k++;
	}
	sfp[i] = 0;
	arg_stack[i][sfp[i]] = path->num;
  }
  ARGS_N = i;
  /*=======================================================*/
  func_ptr = f;
  Conscell *root = f->root;
  arg_n = 0;
  path = eval(root);//jump func's root
  /*==============remove args from Hash Table===============*/
  for (i = 0; f->args[i] != EOL; i++) {
	if (mod_keys[i] != EOL) {
	  setToHashTable(mod_keys[i], value[i]);
	}
	if (del_keys[i] != EOL) {
	  deleteKeyFromHashTable(del_keys[i]);
	}
  }
  /*=======================================================*/
  func_ptr = NULL;
  return path;
}
