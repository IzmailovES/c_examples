#include "htd.h"

size_t int_hash(void* key, size_t sz){
	return (size_t)(*(int*)key%sz);
}

int int_int_is_equal(void* fst, void* snd){
	return *(int*)fst == *(int*)snd;
}

void* int_key_copy(void* src){
	int* new = malloc(sizeof(int));
	*new = *(int*)src;
	return (void*)new;
}

void int_key_destroy(void* key){
	free(key);
}


int main(){
	struct htd_functions ht_int_int_funcs = {
		.hash = int_hash,
		.is_equal = int_int_is_equal,
		.key_copy = int_key_copy,
		.key_destroy = int_key_destroy,
		.value_copy = int_key_copy, // key and value is the same type
		.value_destroy = int_key_destroy // key and value is the same type
	};

	struct htd_hashtab* ht = htd_create_hashtab(1000000ul, &ht_int_int_funcs);
	if (!ht){
		printf("ne rabotaet\n");
	}else{
		printf("ht spzdan %zu\n", ht->array_size);
	}
	int key = 42;
	int value = 443;
	struct htd_nlist* np;
	np = htd_update(ht, &key, &value);
	printf("update_done %d\n", *(int*)np->value);
	printf("%d\n", *(int*)htd_lookup(ht, &key)->value);

}
