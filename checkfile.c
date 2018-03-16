#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <openssl/sha.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include "lib/checkfile.h"
#include "lib/database.h"

// Source: https://www.lemoda.net/c/recursive-directory/
void scan_dir(char* dir_name) {
    DIR* directory;
    directory = opendir (dir_name);

    if (!directory) {
        fprintf (stderr, "Cannot open directory '%s': %s\n",
                 dir_name, strerror (errno));
        exit (EXIT_FAILURE);
    }

    int end = false;
    while (!end) {
        struct dirent * entry;
        const char * d_name;

        entry = readdir (directory);
        if (! entry) {
            end = true;
        }
        else {
            d_name = entry->d_name;

            if (strcmp (d_name, "..") != 0 && strcmp (d_name, ".") != 0) {
                //Checking if directory
                if (entry->d_type & DT_DIR) {
                    int path_length;
                    char path[PATH_MAX];
    
                    path_length = snprintf (path, PATH_MAX, "%s/%s", dir_name, d_name);
                    if (path_length >= PATH_MAX) {
                        fprintf (stderr, "Path length has got too long.\n");
                        exit (EXIT_FAILURE);
                    }
                    scan_dir (path);
                }
                else {
                    char* path = (char*) malloc(sizeof(char) * PATH_MAX);
                    strcpy(path,"");
                    strcat(path, dir_name);
                    strcat(path, "/");
                    strcat(path, d_name);
                    printf("%s => %s\n", path, calc_sha256(path));
                }
            }
        }
    }

    if (closedir (directory)) {
        fprintf (stderr, "Could not close '%s': %s\n",
                 dir_name, strerror (errno));
        exit (EXIT_FAILURE);
    }
}

// Source: https://stackoverflow.com/questions/4553012/checking-if-a-file-is-a-directory-or-just-a-file
int check_directory(const char* path) {
   //printf("Checking: %s\n", path);
    struct stat path_stat;
    if (stat(path, &path_stat) == -1) {
      //perror("while calling stat()");
      return -1;
   } else {
      return S_ISDIR(path_stat.st_mode);
   }
}

// Source: https://stackoverflow.com/questions/7853156/calculate-and-print-sha256-hash-of-a-file-using-openssl
char* calc_sha256 (char* path) {
    char* output = (char *) malloc(sizeof(char) * 65);
    FILE* file = fopen(path, "rb");

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    const int bufSize = 65535;
    char* buffer = malloc(bufSize);
    int bytesRead = 0;
    fread(buffer, 1, bufSize, file);
    while((bytesRead = fread(buffer, bufSize, 1, file))) {
        SHA256_Update(&sha256, buffer, bytesRead);
    }
    SHA256_Final(hash, &sha256);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(output + (i * 2), "%02x", hash[i]);
    }

    fclose(file);
    free(buffer);
    return output;
}      