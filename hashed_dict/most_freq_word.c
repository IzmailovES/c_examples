#include <stdio.h>
#include <stdlib.h>
#include "htd.h"

size_t string_hash(void* ps, size_t size){
	size_t hashval = 0;

	for(size_t i = 0; ((char*)ps)[i] ; ++i ){
		hashval = ((unsigned char*)ps)[i] + 31*hashval;
	}
	return hashval%size;
}

int is_str_eq(void* ps1, void* ps2){
	return !strcmp((char*)ps1 , (char*)ps2);
}

void* str_cpy(void* ps){
	char* s = (char*)ps;
	char* new = malloc(sizeof(char)*strlen(s)+1);
	strcpy(new,s);
	return new;
}

void str_destroy(void* ps){
	free(ps);
}

void* int_key_copy(void* src){
    int* new = malloc(sizeof(int));
    *new = *(int*)src;
    return (void*)new;
}

void int_key_destroy(void* key){
    free(key);
}

struct htd_functions ht_functions = {
	.hash = string_hash,
	.is_equal = is_str_eq,
	.key_copy = str_cpy,
	.key_destroy = str_destroy,
	.value_copy = int_key_copy,
	.value_destroy = int_key_destroy,
};



int main(){
	struct htd_hashtab* counter = htd_create_hashtab(1000, &ht_functions);
	int ret_max = 0;
	char ret_word[256];
	char buf[256];
	struct htd_nlist* np;
	int tmp;
	int def_val = 1;

	while(1){
		scanf("%s", buf);
		if (feof(stdin)){ break;}
		np = htd_lookup(counter, buf);
		if (np){
			(*(int*)np->value)++;
		}else{
			np = htd_update(counter, buf, &def_val);
		}

		if (*(int*)np->value > ret_max){
			ret_max = *(int*)np->value;
			strcpy(ret_word, buf);
		}else if(*(int*)np->value == ret_max && strcmp(buf, ret_word) < 0){
			strcpy(ret_word, buf);
		}
	}
	printf("-> %s\n", ret_word);

}
