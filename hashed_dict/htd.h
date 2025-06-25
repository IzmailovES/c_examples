#ifndef MY_OWN_HASHTAB_H
#define MY_OWN_HASHTAB_H

#include <malloc.h>
#include <string.h>
/* main functions:
 * create hashtab with determine types of keys and values
 * must to determine:
 *	- key hash function
 *	- key is_equal function
 *	- key copy and destroy functions
 *	- value copy and destroy functions
 *
 * feaches:
 * - create hashtab
 * - update element
 * - delete element
 * - lookup for element in hashtab
 * - resize hashtab
 * - delete hashtab
 *
 *	further feaches:
 *   - auto size control by usage level
 *
 * want to use:
 *	- create struct with functions to use hashtab elements
 *	- create hashtab whith: size_hashtab, struct with functions
 *	- delete hashtab will remove all elements and free memory for keys and values, then free hastab memory
 *	- resize hashtab will create new tab with new size and copy all elements with recalculate for new size
 *	- lookup for element function gets pointer for reference value and seek it in tab. Returns pointer for struct nlist or NULL
 *	- update element function gets pointer for key and value and create new item in hashtab with copy of key and value or updating exists value, return pointer for nlist
 *	- delete element function gets pointer for key and delete this entery in hashtab. return 0 if element was present, 1 otherwise 
 *
 */

struct htd_nlist;
struct htd_hashtab;
struct htd_functions;
typedef unsigned char htd_byte;

/* funcs for manage hashtab: */
struct htd_hashtab*		htd_create_hashtab(size_t size, struct htd_functions* funcs); //vrode_gotovo
struct htd_hashtab*		htd_resize_hashtab(struct htd_hashtab* ht, size_t new_size); //not implemented
size_t					htd_delete_hashtab(struct htd_hashtab** ht); // vrode cotovo
size_t					htd_clean_hashtab(struct htd_hashtab* ht); //vrode gotovo

/* funcs for manage hashtab elements */
struct htd_nlist*	htd_update(struct htd_hashtab* ht, void* key, void* value); //vrode gotovo
struct htd_nlist*	htd_lookup(struct htd_hashtab* ht, void* key); //vrode_gotovo
int					htd_delete(struct htd_hashtab* ht, void* key); // vrode work
struct htd_nlist    htd_change(struct htd_hashtab* ht, void* key, void* (*func)(void*), void* default_value); //TODO

/* some iseq functions - for embedded types and strings */
int iseq_string(void* fst, void* snd, size_t size); //TODO
int iseq_fixed(void* fst, void* snd, size_t size); //TODO

/* common funcs for help you */
static void* void_dup(void* value, size_t size_bytes); /* make duplicate of mem */ //TODO
//static char *strdup(char *s); /* make a duplicate of s */ //TODO
size_t hash(void *key, size_t ht_size); //TODO

/* structs implementations
 */
struct htd_nlist { /* table entry: */
    struct htd_nlist *next; /* next entry in chain */
    void *key;
    void *value; 
};

struct htd_hashtab {
	struct htd_nlist** hash_array;
	size_t array_size;
	struct htd_functions* functions;
	int _auto_functions;
};

struct htd_functions{
	size_t (*hash)(void*, size_t);
	int (*is_equal)(void*, void*);
	void* (*key_copy)(void*);
	void (*key_destroy)(void*);
	void* (*value_copy)(void*);
	void (*value_destroy)(void*);
};

/* some macros for good work
 * nopointer usage
 */
#define htd_lookup_nop(ht, key) ({ \
		typeof(key) _key = (key); \
		htd_lookup((ht), &(_key));})

#define htd_update_nop(ht, key, value) ({ \
		typeof(key) _key = (key); \
		typeof(value) _value = (value); \
		htd_update((ht), &_key, &_value); })

// funcs for auto-configure hashtab with buid-in types

#define htdfs_is_equal(obj_t) ({ \
	!memcpy()
})

#define htd_create_builtins(size, key_t, value_t, htd) ({ \
		struct htd_functions* ht_f = (struct htd_functions*)malloc(sizeof(struct htd_functoins)); \
		if (ht_f){	\
			ht_f->hash = htdfs_hash##key_t ; \
			ht_f->is_equal = htdfs_is_equal_##key_t ; \
			ht_f->key_copy = htdfs_copy##key_t ; \
			ht_f->key_destroy = htdfs_destroy##key_t ; \
			ht_f->value_copy = htdfs_copy##key_t ; \
			ht_f->value_destroy = htdfs_destroy##key_t ; \
			htd = htd_create_hashtab(size, ht_f); \
			if (htd){ htd->_auto_functions = 1;}	\
		}else{ \
			htd = NULL; \
		} \
})




/* end header */
#endif //MY_OWN_HASHTAB_H

