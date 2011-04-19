#include <glisp.h>

void* gmalloc(size_t size)
{
  void *ret = malloc(size);
  if (ret == NULL) {
	fprintf(stderr, "malloc error!!\n");
	exit(EXIT_FAILURE);
  }
  memset(ret, 0, size);
  return ret;
}
