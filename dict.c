#include "dict.h"

Dict* dict_new(int size)
{
	Dict* dict  = (Dict*) malloc(sizeof(Dict));
	dict->items = (Dict_item**) malloc(sizeof(Dict_item) * size * 2);
	dict->size  = size;
	dict->len   = 0;
	return dict;
}

void dict_clean(Dict* dict)
{
	free(dict->items);
	free(dict);
}

void dict_print(Dict* dict)
{
	for (int i = 0; i < dict->len; i++)
	{
		Dict_item* item = dict->items[i];
		printf("{%p : %p} ", item->key, item->value);
	}
	printf("\n");
}

void dict_insert(Dict* dict, void* key, void* value, size_t k_sz, size_t v_sz) 
{
	if (dict->len >= dict->size)
	{
		fprintf(stderr, "[ERROR]: Dictionary is full cannot add more element\n");
		exit(1);
	}
	dict->items[dict->len] = (Dict_item*) malloc(sizeof(Dict_item));
	dict->items[dict->len]->key   = calloc(1, k_sz);
	memcpy(dict->items[dict->len]->key, key, k_sz);
	dict->items[dict->len]->value = calloc(1, v_sz);
	memcpy(dict->items[dict->len]->value, value, v_sz);
	dict->len++;
}

void dict_pop(Dict* dict, void* key, size_t k_sz)
{
	if (dict->len == 0)
	{
		fprintf(stderr, "[ERROR]: Cannot pop empty dictionary.\n");
		exit(1);
	}

	int prev_size = dict->len;

	int index = 0;
	for(int i = 0; i < prev_size; i++)
	{
		Dict_item* item = dict->items[i];
		if (memcmp(key, item->key, k_sz))
		{
			dict->items[index] = item;
			index++;
		}
		else
		{
			dict->len--;
		}
	}

	if (prev_size == dict->len)
	{
		fprintf(stderr, "[ERROR]: Didnt found the required key.\n");
		exit(1);
	}
}

void* dict_get(Dict* dict, void* key, size_t k_sz)
{
	for(int i = 0; i <  dict->len; i++)
	{
		Dict_item* item = dict->items[i];
		if (!memcmp(key, item->key, k_sz))
		{
			return item->value;
		}
	}
	fprintf(stderr, "[ERROR]: Couldnt find the key you were searching for.\n");
	exit(1);
	return NULL;
}

Dict_item* dict_get_item_idx(Dict* dict, int idx)
{
	if (idx >= dict->len)
	{
		fprintf(stderr, "[ERROR]: Index out of bounds.\n");
		exit(1);
	}

	Dict_item* item = dict->items[idx];
	return item;
}

bool dict_exists(Dict* dict, void* key, size_t k_sz)
{
	for (int i = 0; i < dict->len; i++)
	{
		Dict_item* item = dict->items[i];
		if (!memcmp(key, item->key, k_sz))
		{
			return true;
		}
	}
	return false;
}
