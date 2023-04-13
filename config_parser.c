#include "config_parser.h"

int _doubles_to_str(double* doubles, size_t cnt, char* buf){
    int i;
    int writed = 0;
    char locbuf[256];
    *buf = 0;
    for (i = 0; i < cnt; ++i){
        if (!i){
            writed += sprintf(locbuf, "%lf", *(doubles+i));
        }
        else{
            writed += sprintf(locbuf, ",%lf", *(doubles+i));
        }
        strcat(buf,locbuf);
    }
    return writed;
}

int _unsignends_to_str(unsigned* us, size_t cnt, char*buf){
    int i;
    int writed = 0;
    char locbuf[256];
    *buf = 0;
    for (i = 0; i < cnt; ++i){
        if (!i){
            writed += sprintf(locbuf, "%u", *(us+i));
        }
        else{
            writed += sprintf(locbuf, ",%u", *(us+i));
        }
        strcat(buf,locbuf);
    }
    return writed;
}

int _bool_to_str(int b, char* buf){
    if (b){
        return sprintf(buf, "YES");
    }
    else{
        return sprintf(buf, "NO");
    }
    
}

int _get_from_config(char* configname, char* field, char* type, void* dest){
    // dest = string, int, double, size_t, unsigned
    FILE* file;
    char buf1[1024];
    char buf2[1024];
    char formatstring[256];
    int i,j;
    int retcode = 0;
    // open file
    if ((file = fopen(configname, "r")) == NULL){
        fprintf(stderr, "can not open config file %s", configname);
        return -1; // file open fail
    }
    if(!strcmp(type,"string")){
        strcpy(formatstring, "%s %s");
    }
    else if(!strcmp(type,"int")){
        strcpy(formatstring, "%s %d");
    }
    else if(!strcmp(type,"double")){
        strcpy(formatstring, "%s %lf");
    }
    else if(!strcmp(type,"size_t")){
        strcpy(formatstring, "%s %zu");
    }
    else if(!strcmp(type,"unsigned")){
        strcpy(formatstring, "%s %u");
    }
    else{
        fprintf(stderr, "unknown format %s", type);
        return -2; // unknown format fail
    }
    while(fgets(buf1, sizeof(buf1)/sizeof(char), file)){
        for (i = 0; i < strlen(buf1);){
            if(buf1[i] == '\0' || buf1[i] == '#'){ break;}
            if(buf1[i] == '='){buf1[i] = ' '; }
            buf2[i] = buf1[i];
            i++;
        }
        buf2[i] = '\0';
        sscanf(buf2, "%s", buf1);
        if(!(strcmp(buf1, field))){
            retcode = sscanf(buf2, formatstring, buf1, dest) - 1;
            break;
        }
    }
    if(!retcode){
        fprintf(stderr, "field %s not found!\n", field);
        exit(EXIT_FAILURE);
    }
    fclose(file);
    return retcode;
}