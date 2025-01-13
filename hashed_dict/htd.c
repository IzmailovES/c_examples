#include "htd.h"

struct htd_hashtab* htd_create_hashtab(size_t size, struct htd_functions* funcs){
	struct htd_hashtab* ht;
	ht = (struct htd_hashtab*)malloc(sizeof(struct htd_hashtab));
	if (ht == NULL || ((ht->hash_array = (struct htd_nlist**)malloc(sizeof(struct htd_nlist*)*size)) == NULL)){
		return NULL;
	}
	memset(ht->hash_array, 0, sizeof(struct htd_nlist*)*size);
	ht->array_size = size;
	ht->functions = funcs;
	return ht;
}

/* lookup: look for s in hashtab */
struct htd_nlist* htd_lookup(struct htd_hashtab* ht, void* key)
{
    struct htd_nlist* np; //node pointer
    for (np = ht->hash_array[ht->functions->hash(key, ht->array_size)]; np != NULL; np = np->next)
        if (ht->functions->is_equal(key, np->key)){
          return np; /* found */
		}
    return NULL; /* not found */
}

struct htd_nlist* htd_update(struct htd_hashtab* ht, void* key, void* value){
	struct htd_nlist *np; // node pointer
	size_t hashval;
	if ((np = htd_lookup(ht, key)) == NULL){ // if not found
		np = (struct htd_nlist*)malloc(sizeof(struct htd_nlist));
		if (np == NULL || ((np->key = ht->functions->key_copy(key)) == NULL)){
			return NULL;
		}
		hashval = ht->functions->hash(key, ht->array_size); //maybe double hash calculate
		np->next = ht->hash_array[hashval];
		ht->hash_array[hashval] = np;
	}else{ // if found
		ht->functions->value_destroy(np->value);
	}
	if ((np->value = ht->functions->value_copy(value)) == NULL){
		return NULL;
	}
	return np;
}

int htd_delete(struct htd_hashtab* ht, void* key){
	struct htd_nlist* np, *npp;
	size_t hashval;
	if ((np = htd_lookup(ht,key)) == NULL){ // if not exists
		return 0;
	}

	ht->functions->value_destroy(np->value);
	ht->functions->key_destroy(np->key);

	hashval = ht->functions->hash(key, ht->array_size);
	//search for pointer to np
	if ((npp = ht->hash_array[hashval]) == np){ //if our np is the first item
		ht->hash_array[hashval] = ht->hash_array[hashval]->next;
		free(np);
	}else{
		for (;npp->next != np; npp = npp->next);
		npp->next = np->next;
		free(np);
	}
	return 1;
}

size_t htd_clean_hashtab(struct htd_hashtab* ht){
	size_t deleted = 0;
	size_t i;
	struct htd_nlist* np;

	for (i=0; i != ht->array_size; ++i){
		while (np = ht->hash_array[i]){
			ht->hash_array[i] = np->next;
			ht->functions->value_destroy(np->value);
			ht->functions->key_destroy(np->key);
			free(np);
			++deleted;
		}
	}
	return deleted;
}

size_t htd_delete_hashtab(struct htd_hashtab** ht){
	size_t deleted = htd_clean_hashtab(*ht);
	free((*ht)->hash_array);
	free(*ht);
	*ht = NULL;
	return deleted;
}

#if 0
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
#endif
