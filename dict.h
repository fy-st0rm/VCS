#ifndef _DICT_H_
#define _DICT_H_

#include "const.h"

typedef struct
{
	void* key;
	void* value;
} Dict_item;

typedef struct 
{
	Dict_item** items;
	int size;
	int len;
} Dict;


Dict* dict_new(int size);
void  dict_clean(Dict* dict);
void  dict_print(Dict* dict);
void  dict_insert(Dict* dict, void* key, void* value, size_t k_sz, size_t v_sz);
void  dict_pop(Dict* dict, void* key, size_t k_sz);
void* dict_get(Dict* dict, void* key, size_t k_sz);
Dict_item* dict_get_item_idx(Dict* dict, int idx);
bool  dict_exists(Dict* dict, void* key, size_t k_sz);

#endif
