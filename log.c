#include <stdio.h>
#include <stdlib.h>

#include "lib/log.h"

void write_to_logfile(char* value, char* time) {
    FILE *file = fopen("log/log.txt", "a");
    if (file != NULL) {
        fprintf(file, "%s %s\n", value, time);    
    }
    else {
        FILE *file = fopen("log/log.txt", "w");
        fprintf(file, "%s %s\n", value, time);    
    }
    fclose(file);
}