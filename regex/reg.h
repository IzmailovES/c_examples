#ifndef A_REG_H
#define A_REG_H


typedef struct {
    char* name;
    uint32_t reg;
} a_register;

#define A_REG_COUNT 5

#define A_REGS_LIST { \
					 {"reg0", 0}, \
                     {"reg1", 1}, \
                     {"reg2", 2}, \
                     {"reg3", 3}, \
                     {"reg04", 4} \
                    }

int reg_search_by_name_regex(char *regex, uint32_t* result);
int reg_search_by_name(char* name, a_register* result);
int reg_serach_by_addr(uint32_t addr, a_register* result);

#endif // A_REG_H
