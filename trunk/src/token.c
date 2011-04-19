#include <glisp.h>

char** tokenizer(char *line)
{
  char **token = (char **)gmalloc(sizeof(char *) * MAX_LINE_SIZE);
  int index = 0;
  int n = 0;
  char buf[32] = {0};
  int buf_n = 0;

  while (line[index] != EOL) {
	switch (line[index]) {
	case ' ':
	  index++;
	  continue;
	case '-':
	  if (isdigit(line[index + 1])) {
		//simple exp
		if (index == 0) {
		  fprintf(stderr, "Syntax Error!!\n");
		  deleteToken(token);
		  return NULL;
		} else if (line[index - 1] == '(') {
		  fprintf(stderr, "Syntax Error!!\n");
		  deleteToken(token);
		  return NULL;
		}
		buf[0] = line[index];
		buf_n = 1;
		index++;
		while (isdigit(line[index])) {
		  buf[buf_n] = line[index];
		  index++;
		  buf_n++;
		}
		buf[buf_n] = EOL;
		token[n] = (char *)malloc(sizeof(char) * (buf_n + 1));
		memset(token[n], 0, sizeof(char) * (buf_n + 1));
		sprintf(token[n], "%s", buf);
		n++;
		continue;
	  } else {
		token[n] = (char *)malloc(sizeof("-"));
		memset(token[n], 0, sizeof("-"));
		sprintf(token[n], "%s", "-");
		n++;
	  }
	  break;
	case '>':
	  if (line[index + 1] != EOL) {
		if (line[index + 1] == '=') {
		  token[n] = (char *)malloc(sizeof(char) * 3);
		  sprintf(token[n], "%c%c", line[index], line[index + 1]);
		  n++;
		  index++;
		} else {
		  token[n] = (char *)malloc(sizeof(char) * 2);
		  sprintf(token[n], "%c", line[index]);
		  n++;
		}
	  } else {
		printf("Syntax error !!\n");
	  }
	  break;
	case '<':
	  if (line[index + 1] != EOL) {
		if (line[index + 1] == '=') {
		  token[n] = (char *)malloc(sizeof(char) * 3);
		  sprintf(token[n], "%c%c", line[index], line[index + 1]);
		  n++;
		  index++;
		} else {
		  token[n] = (char *)malloc(sizeof(char) * 2);
		  sprintf(token[n], "%c", line[index]);
		  n++;
		}
	  } else {
		printf("Syntax error !!\n");
	  }
	  break;	  
	case '0': case '1': case '2': case '3': case '4': case '5':
	case '6': case '7': case '8': case '9':
	  buf[0] = line[index];
	  buf_n = 1;
	  index++;
	  while (isdigit(line[index])) {
		buf[buf_n] = line[index];
		index++;
		buf_n++;
	  }
	  buf[buf_n] = EOL;
	  token[n] = (char *)malloc(sizeof(char) * (buf_n + 1));
	  memset(token[n], 0, sizeof(char) * (buf_n + 1));
	  sprintf(token[n], "%s", buf);
	  n++;
	  continue;
	default :
	  if (isalpha(line[index])) {
		buf_n = 0;
		do {
		  buf[buf_n] = line[index];
		  index++;
		  buf_n++;
		} while (isalpha(line[index]));
		buf[buf_n] = EOL;
		token[n] = (char *)malloc(sizeof(char) * (buf_n + 1));
		memset(token[n], 0, sizeof(char) * (buf_n + 1));
		sprintf(token[n], "%s", buf);
		n++;
		continue;
	  } else {
		//specific symbol
		token[n] = (char *)malloc(sizeof(char) * 2);
		memset(token[n], 0, sizeof(char) * 2);
		sprintf(token[n], "%c", line[index]);
		n++;
	  }
	  break;
	}
	index++;
  }
  return token;
}

void deleteToken(char **token)
{
  int i = 0;
  if (token != NULL) {
	while (token[i] != EOL) {
	  //printf("token[%d] = %s\n", i, token[i]);
	  free(token[i]);
	  i++;
	}
	free(token);
  }
}
