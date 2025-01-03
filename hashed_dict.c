#ifndef MY_OWN_HASHTAB_H
#define MY_OWN_HASHTAB_H

#include <malloc.h>
#include <string.h>

struct nlist;
struct hashtab;
typedef unsigned char simple_byte;

/* funcs for manage hashtab:
 * - create
 * - delete
 * - resize
 */
struct hashtab* create_hashtab(size_t size, size_t key_size, size_t value_size, int (*iseq)(void*, void*, size_t));
struct hashtab* resaize_hashtab(size_t new_size, struct hashtab* ht); // not implemented
void delete_hashtab(struct hashtab* ht);

/* funcs for manage hashtab elements
 * - lookup - search element  - returns pointer to nlist or NULL
 * - setitem - set new value in existing node or create new
 */
struct nlist* setitem(void* key, void* value, struct hashtab* ht);
struct nlist* lookup(void *s, struct hashtab* ht);

/* some iseq functions - for embedded types and strings
 */
int iseq_string(void* fst, void* snd, size_t size);
int iseq_fixed(void* fst, void* snd, size_t size);

/* common funcs for hastab - static only
 */
static unsigned hash(void *key, struct hashtab* ht);
static void* void_dup(void* value, size_t size_bytes); /* make dumlicate of mem */
static char *strdup(char *s); /* make a duplicate of s */

/* structs implementations
 * /
struct nlist { /* table entry: */
    struct nlist *next; /* next entry in chain */
    void *key;
    void *val; 
};

struct hashtab {
	struct nlist** hash_array;
	size_t array_size;
	size_t key_size;
	size_t value_size;
	int (*iseq)((void*, void*, size_t));
};

/* end header */
#endif //MY_OWN_HASHTAB_H

struct hashtab* create_hashtab(size_t size, size_t key_size, size_t value_size, int (*iseq)(void*, void*, size_t)){
	struct hashtab* new;
	new = (struct hashtab*)malloc(sizeof(struct hashtab));
	if (new == NULL || ((new->hash_array = (struct nlist**)malloc(sizeof(struct nlist*)*size)) == NULL)){
		return NULL;
	}
	memset(new->hash_array, 0, sizeof(struct nlist*)*size);
	new->array_size = size;
	new->key_size = key_size;
	new->value_size = value_size;
	new->iseq = iseq;
	return new;
}

void delete_hashtab(struct hashtab* ht){
	free(ht->hash_array);
	free(ht);
}

/* hash: form hash value for string s */
unsigned hash(void *key, struct hashtab* ht)
{
    register unsigned hashval = 0;
	size_t i = ht->key_size;
    for (; i; --i){
      hashval = *((simple_byte*)(key + i - 1u)) - + 31u * hashval;
	}
    return hashval % ht->array_size;
}

/* lookup: look for s in hashtab */
struct nlist *lookup(void *s, struct hashtab* ht)
{
    struct nlist *np;
    for (np = ht->hash_array[hash(s, ht)]; np != NULL; np = np->next)
        if (!ht->iseq(s, np->key, ht->key_size)){
          return np; /* found */
		}
    return NULL; /* not found */
}

static void* void_dup(void* value, size_t size_bytes){
	void *new;
	if ((new = malloc(size_bytes)) == NULL){
		return NULL;
	}
	return memcpy(new, value, size_bytes);
}

struct nlist* setitem(void* key, void* value, struct hashtab* ht){
	struct nlist *np;
	unsigned hashval;
	if ((np = lookup(key, ht)) == NULL){ // if not found
		np = (struct nlist*)malloc(sizeof(struct nlist));
		if (np == NULL || ((np->key = void_dup(key, ht->key_size)) == NULL)){
			return NULL;
		}
		hashval = hash(key,ht);
		np->next = ht->hash_array[hashval];
		ht->hash_array[hashval] = np;
	}else{ // if found
		free(np->val);
	}
	if ((np->val = void_dup(value, ht->value_size)) == NULL){
		return NULL;
	}
	return np;

}


/* group of is equal functions for fixed-width types, such as int, double, etc
 * and variable-width string(array of chars)
 * returns 0 in objects is equal, 1 otherwise
 */
int iseq_fixed(void* fst, void* snd, size_t size){
	for(; size; --size){
		if (*((simple_byte*)(fst + size - 1)) != *((simple_byte*)(snd + size - 1))){
			return 1;
		}	
	}
	return 0;
}

int iseq_string(void* fst, void* snd, size_t size){
	char* char_fst = *(char**)fst;
	char* char_snd = *(char**)snd;
	while (*char_fst or *char_snd){
		if (*char_fst != *char_snd){
			return 1;
		}
	}
	return 0;
}

static char *strdup(char *s) /* make a duplicate of s */
{
    char *p;
    p = (char *) malloc(strlen(s)+1); /* +1 for ’\0’ */
    if (p != NULL)
       strcpy(p, s);
    return p;
}
