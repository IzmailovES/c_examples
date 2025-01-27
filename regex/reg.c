#include <stdio.h>
#include <regex.h>
#include <stdint.h>
#include <inttypes.h>
#include <regex.h>
#include <stdlib.h>

#include "reg.h"

a_register regs[A_REG_COUNT] = A_REGS_LIST;
char* pattern = "^reg.*$";
//int reg_flags = REG_NOSUB; // 0 if you want to process start and ent of match
int reg_flags = 0; // 0 if you want to process start and ent of match

int main(){
	int i;
	int regcomp_ret;
	for(i= 0; i!= A_REG_COUNT; ++i){
		printf("reg: %" PRIu32 ", name: %s\n", regs[i].reg, regs[i].name);
	}
	printf("next find\n");
	regex_t regex;
	regmatch_t match;
	regcomp_ret = regcomp(&regex, pattern, reg_flags);
	printf("reg compile status: %d\n", regcomp_ret);
	if (regcomp_ret){
		printf("regexp compilation fail\nexit...\n");
		exit(1);
	}
	for(i = 0; i!= A_REG_COUNT; ++i){
		if (regexec(&regex, regs[i].name, 1, &match, 0) == 0){
			if(reg_flags & REG_NOSUB != 0){
				printf("cached string: %.*s \n", match.rm_eo-match.rm_so, regs[i].name + match.rm_so);
			}
			printf("reg: %" PRIu32 ", name: %s\n", regs[i].reg, regs[i].name); }
	}
	regfree(&regex);
}


