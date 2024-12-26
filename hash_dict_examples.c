#include "hashed_dict.c" // ohohoh
						 //
int main(){
	int f = 1;
	while(f){
		struct hashtab* ht= create_hashtab(1024, sizeof(int), sizeof(float));

		int key = 42;
		float value = 123.99;
		setitem(&key, &value, ht);
		key++;
		setitem(&key, &value, ht);
		key--;
		printf("%f\n", *((float*)(lookup(&key, ht)->val)));
		key++;
		printf("%f\n", *((float*)(lookup(&key, ht)->val)));
		delete_hashtab(ht);
		f = 0;
	}
	return 0;
}
