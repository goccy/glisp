#include <glisp.h>

FuncTable* new_FuncTable(void)
{
  FuncTable *f_table = (FuncTable *)gmalloc(sizeof(FuncTable));
  f_table->func_name = NULL;
  f_table->args = NULL;
  f_table->root = NULL;
  f_table->next = NULL;
  return f_table;
}

FuncTable* searchFuncTable(char *func_name)
{
  FuncTable *ret = search_ftop;
  while (ret->next != NULL) {
	ret = ret->next;
	if (ret->func_name != NULL) {
	  if (!strncmp(ret->func_name, func_name, strlen(func_name) + 1)) {
		return ret;
	  }
	}
  }
  ret = NULL;
  return ret;
}

void setToFuncTable(char *func_name, char **args, Conscell *root)
{
  FuncTable *f_table = NULL;
  FuncTable *search_end = searchFuncTable(func_name);
  if (search_end == NULL) {
	f_table = new_FuncTable();
	f_table->func_name = (char *)gmalloc(strlen(func_name) + 1);
	sprintf(f_table->func_name, "%s", func_name);
	f_table->args = (char **)gmalloc(sizeof(char *) * MAX_ARGS);
	int i = 0;
	while (args[i] != EOL) {
	  f_table->args[i] = (char *)gmalloc(strlen(args[i]) + 1);
	  sprintf(f_table->args[i], "%s", args[i]);
	  i++;
	}
	f_table->root = root;
	func_table->next = f_table;
	func_table = func_table->next;
	printf("Func: %s\n", func_name);
  } else {
	printf("redefinition !!\n");
  }
}

FuncTable* getFuncTable(char *func_name)
{
  FuncTable *ret = searchFuncTable(func_name);
  if (ret == NULL) {
	printf("Cannnot find func_name !!\n");
	return NULL;
  } else {
	return ret;
  }
}

void deleteFuncTable(Parser *p)
{
	FuncTable *del = search_ftop;
	while (del->next != NULL) {
		FuncTable *tmp = del->next;
		free(del);
		del = tmp;
		if (del->args != NULL) {
			int i = 0;
			while (del->args[i] != EOL) {
				free(del->args[i]);
				i++;
			}
			free(del->args);
		}
		if (del->func_name != NULL) {
			free(del->func_name);
		}
		if (del->root != NULL) {
			p->delete(del->root);
		}
	}
	free(func_table);
}
