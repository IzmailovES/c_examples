#include "hashed_dict.c" // ohohoh
						 //
#include <stdlib.h>
int main(){
	int f = 1;
	while(f){
		int i;
		size_t hashtab_size = 10000000;
		int val;
		struct hashtab* ht= create_hashtab(hashtab_size, sizeof(int), sizeof(int));
		printf("hashtab size of %zu createdi\n", hashtab_size);
		for (i = 0; i != hashtab_size; ++i){	
			val = 1 + i;
			setitem(&i, &val, ht);
		}
		for (i = 0; i != hashtab_size; ++i){
			if (*((int*)(lookup(&i, ht)->val)) != (i + 1)){
				printf("wrong answer\n");
			}
			//printf("%f\n", *((float*)(lookup(&i, ht)->val)));
		}

		int key = 42;
		float value = 123.99;
		struct nlist* somepointer;
		setitem(&key, &value, ht);
		key++;
		value += 5;
		setitem(&key, &value, ht);
		key--;
		printf("%f\n", *((float*)(lookup(&key, ht)->val)));
		key++;
		printf("%f\n", *((float*)(lookup(&key, ht)->val)));
		key++;
		if ((somepointer = lookup(&key, ht)) == NULL ){
			printf("Not found in hastab\n");
		}else{
			printf("%f\n", *((float*)(somepointer->val)));
		}
		delete_hashtab(ht);
		f = 0;
	}
	return 0;
}
