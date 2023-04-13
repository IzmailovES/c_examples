#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

int _doubles_to_str(double* doubles, size_t cnt, char* buf);
int _unsignends_to_str(unsigned* us, size_t cnt, char* buf);
int _bool_to_str(int b, char* buf);
int _get_from_config(char* configname, char* field, char* type, void* dest);



#endif //CONFIG_PARSER_H