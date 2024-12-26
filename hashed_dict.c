#include <malloc.h>
#include <string.h>

struct nlist;
struct hashtab;
typedef unsigned char simple_byte;

unsigned hash(void *key, struct hashtab* ht);
void* void_dup(void* value, size_t size_bytes);

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
//	char key_type[32];
//	char value_type[32];
};

struct hashtab* create_hashtab(size_t size, size_t key_size, size_t value_size){
	struct hashtab* new;
	new = (struct hashtab*)malloc(sizeof(struct hashtab));
	if (new == NULL || ((new->hash_array = (struct nlist**)malloc(sizeof(struct nlist*)*size)) == NULL)){
		return NULL;
	}
	memset(new->hash_array, 0, sizeof(struct nlist*)*size);
	new->array_size = size;
	new->key_size = key_size;
	new->value_size = value_size;
	return new;
}

void delete_hashtab(struct hashtab* ht){
	free(ht->hash_array);
	free(ht);
}

/* hash: form hash value for string s */
unsigned hash(void *key, struct hashtab* ht)
{
    unsigned hashval = 0;
	register size_t i = ht->key_size;
    for (; i; --i){
      hashval = *((simple_byte*)(key + ht->key_size - 1u)) - + 31u * hashval;
	}
    return hashval % ht->array_size;
}

int key_cmp(void* key1, void* key2, size_t keysize){
	for (; keysize; --keysize){
		if (*((simple_byte*)(key1 + keysize - 1)) != *((simple_byte*)(key2 + keysize -1))){
			return 1;
		}
	}
	return 0;
}


/* lookup: look for s in hashtab */
struct nlist *lookup(void *s, struct hashtab* ht)
{
    struct nlist *np;
    for (np = ht->hash_array[hash(s, ht)]; np != NULL; np = np->next)
        if (!key_cmp(s, np->key, ht->key_size)){
          return np; /* found */
		}
    return NULL; /* not found */
}

void* void_dup(void* value, size_t size_bytes){
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



#if 0
char *strdup(char *);
/* install: put (name, defn) in hashtab */
struct nlist *install(char *name, char *defn)
{
    struct nlist *np;
    unsigned hashval;
    if ((np = lookup(name)) == NULL) { /* not found */
        np = (struct nlist *) malloc(sizeof(*np));
        if (np == NULL || (np->name = strdup(name)) == NULL)
          return NULL;
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    } else /* already there */
        free((void *) np->defn); /*free previous defn */
    if ((np->defn = strdup(defn)) == NULL)
       return NULL;
    return np;
}

char *strdup(char *s) /* make a duplicate of s */
{
    char *p;
    p = (char *) malloc(strlen(s)+1); /* +1 for ’\0’ */
    if (p != NULL)
       strcpy(p, s);
    return p;
}
#endif
