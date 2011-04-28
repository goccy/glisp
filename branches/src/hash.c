#include <glisp.h>

HashTable* searchHashTable(char *key)
{
	HashTable *ret = search_htop;
	while (ret->next != NULL) {
		ret = ret->next;
		if (ret->key != NULL) {
			if (!strncmp(ret->key, key, strlen(key) + 1)) {
				return ret;
			}
		}
	}
	ret = NULL;
	return ret;
}

int getValFromHashTable(char *key)
{
  int value = 0;
  HashTable *search = searchHashTable(key);
  if (search != NULL) {
	value = search->value;
  } else {
	printf("Cannnot find!! %s\n", key);
  }
  return value;
}

void deleteKeyFromHashTable(char *key)
{
  HashTable *search = search_htop;
  HashTable *prev = NULL;
  HashTable *cur = NULL;
  HashTable *after = NULL;
  while(search->next != NULL) {
	prev = search;
	cur = search->next;
	search = search->next;
	if (cur->key != NULL) {
	  if (!strncmp(cur->key, key, strlen(key) + 1)) {
		if (cur->next != NULL) {
		  after = cur->next;
		  free(cur->key);
		  free(cur);
		  prev->next = after;
		  cur = after;
		} else {
		  prev->next = NULL;
		  free(cur->key);
		  free(cur);
		  hash_table = prev;
		}
	  }
	}
  }
}

void setToHashTable(char *key, int value)
{
  HashTable *h_table = NULL;
  HashTable *search_end = searchHashTable(key);
  if (search_end == NULL) {
	h_table = new_HashTable();
	h_table->key = (char *)gmalloc(strlen(key) + 1);
	sprintf(h_table->key, "%s", key);
	h_table->value = value;
	hash_table->next = h_table;
	hash_table = hash_table->next;
  } else {
	search_end->value = value;
  }
}

HashTable* new_HashTable(void)
{
  HashTable *h_table = (HashTable *)gmalloc(sizeof(HashTable));
  h_table->key = NULL;
  h_table->value = 0;
  h_table->next = NULL;
  return h_table;
}

void deleteHashTable(void)
{
  HashTable *del = search_htop;
  while(del->next != NULL) {
	HashTable *tmp = del->next;
	free(del);
	del = tmp;
	if (del->key != NULL) {
	  free(del->key);
	}
  }
  free(hash_table);
}

/*
int main(void)
{
  hash_table = new_HashTable();
  search_top = hash_table;
  setToHashTable("key", 2);
  setToHashTable("key2", 4);
  setToHashTable("key2", 6);
  int value = getValFromHashTable("key");
  printf("value = %d\n", value);
  value = getValFromHashTable("key2");
  printf("value = %d\n", value);
  value = getValFromHashTable("key3");
  printf("value = %d\n", value);

  return 0;
}
*/
