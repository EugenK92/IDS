#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/log.h"

void write_to_logfile(char* value, char* time, char* name) {
    char* filename = (char*) malloc(sizeof(char*) * 50);
    strcpy(filename, "");
    strcat(filename, "log/");
    strcat(filename, name);
    FILE *file = fopen(filename, "a");
    if (file != NULL) {
        fprintf(file, "%s %s\n", value, time);    
    }
    else {
        FILE *file = fopen("log/log.txt", "w");
        fprintf(file, "%s %s\n", value, time);    
    }
    fclose(file);
    free(filename);
}