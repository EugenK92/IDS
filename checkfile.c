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

// Helping Source: https://www.lemoda.net/c/recursive-directory/
void scan_dir(char* dir_name) {
    DIR* directory;
    directory = opendir(dir_name);

    if (!directory) {
        fprintf (stderr, "Cannot open directory '%s': %s\n",
                 dir_name, strerror (errno));
        //exit (EXIT_FAILURE);
    }

    int end = false;
    while (!end && directory) {
        struct dirent * entry;
        const char * d_name;
        entry = readdir(directory);
        // printf("HEEEE\n");
        if (!entry) {
            end = true;
        }
        else {
            d_name = entry->d_name;
            // printf("%s\n", d_name);
            if (strcmp(d_name, "..") != 0 && strcmp(d_name, ".") != 0) {
                //Checking if directory
                // printf("blub1\n");
                if (entry->d_type & DT_DIR) {
                    int path_length;
                    char path[PATH_MAX];
                    path_length = strcmp(d_name, "") == 0 ? 
                        snprintf (path, PATH_MAX, "%s", dir_name) :
                        snprintf (path, PATH_MAX, "%s/%s", dir_name, d_name);
                    if (path_length >= PATH_MAX) {
                        fprintf (stderr, "Path length has got too long.\n");
                        exit (EXIT_FAILURE);
                    }
                    // printf("%s\n", path);
                    scan_dir(path);
                }
                else if (entry->d_type & DT_REG) {
                    char* output = (char*) malloc(sizeof(char) * 65);
                    char* path = (char*) malloc(sizeof(char) * PATH_MAX);
                    strcpy(path,"");
                    strcat(path, dir_name);
                    strcat(path, "/");
                    strcat(path, d_name);
                    output = calc_sha256(path);
                    // printf("%s\n", output);
                    put_data(path, output);
                }      
            }
        }

    }

    if (directory && closedir(directory)) {
        fprintf(stderr, "Could not close '%s': %s\n",
                 dir_name, strerror (errno));
        exit(EXIT_FAILURE);
    }
}

// Source: https://stackoverflow.com/questions/4553012/checking-if-a-file-is-a-directory-or-just-a-file
int check_directory(const char* path) {
   //printf("Checking: %s\n", path);
    struct stat path_stat;
    if (stat(path, &path_stat) == -1) {
      //perror("while calling stat()");
      return -1;
   } 
   else {
       if (S_ISREG(path_stat.st_mode & S_IFMT)) {
           return 1;
       }
       else if (S_ISFIFO(path_stat.st_mode & S_IFMT) || 
                S_ISSOCK(path_stat.st_mode & S_IFMT)) {
           return 2;
       }
       return 0;
   }
}

// Source: https://stackoverflow.com/questions/7853156/calculate-and-print-sha256-hash-of-a-file-using-openssl
char* calc_sha256 (char* path) {
    FILE* file = fopen(path, "rb");
    char* output = (char*) malloc(sizeof(char) * 65);
    strcpy(output, "");
    if (file) {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        const int bufSize = 65535;
        char* buffer = (char*) malloc(sizeof(char) * bufSize);
        int bytesRead = 0;
        // printf("STAT: %d\n", check_directory(path));
        if (check_directory(path) == 1) {
            while((bytesRead = fread(buffer, 1, bufSize, file))) {
                // printf("sha here\n");
                SHA256_Update(&sha256, buffer, bytesRead);
            }

            SHA256_Final(hash, &sha256);

            for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
                sprintf(output + (i * 2), "%02x", hash[i]);
            }
        }

        fclose(file);
        free(buffer);
    }
    return output;
}      