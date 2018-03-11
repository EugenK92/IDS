#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <openssl/sha.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "lib/checkfile.h"
#include "lib/database.h"

void scan_dir(char* path, int depth) {
    struct dirent **namelist;
    int n = 0;
    char cwd[1024];
    //char output[65];
    printf("Depth: %d Path: %s\n\n", depth, path);
    n = scandir(path, &namelist, NULL, alphasort);
    if (n < 0) {
        perror("scandir");
    }
    else {
        while (n--) {
            //put_data(namelist[n]->d_name, calc_sha256(namelist[n]->d_name, output), 0);
            printf("%s => %d\n", namelist[n]->d_name, check_directory(namelist[n]->d_name));
            char* temp_path = (char *) malloc(sizeof(char) * 128);
            strcpy(temp_path,"");
            strcat(temp_path, getcwd(cwd, sizeof(cwd)));
            strcat(temp_path, "/");
            strcat(temp_path, namelist[n]->d_name);
            printf("%s\n", temp_path);
            if (strcmp(namelist[n]->d_name, ".") != 0 && 
                strcmp(namelist[n]->d_name, "..") != 0 && 
                check_directory(temp_path) == 1) {
               
                opendir(temp_path);
                scan_dir(temp_path, depth + 1);
            }

            free(namelist[n]);
        }
        free(namelist);
        for (int i = 0; i < depth; i++) {
            //opendir("..");
        }
    }     
}

// Source: https://stackoverflow.com/questions/4553012/checking-if-a-file-is-a-directory-or-just-a-file
int check_directory(const char *path) {
    printf("Checking: %s\n", path);
    struct stat path_stat;
    if (stat(path, &path_stat) == -1) {
      perror("while calling stat()");
      return -1;
   } else {
      return S_ISDIR(path_stat.st_mode);
   }
}

// Source: https://stackoverflow.com/questions/7853156/calculate-and-print-sha256-hash-of-a-file-using-openssl
char* calc_sha256 (char* path, char output[65]) {
    FILE* file = fopen(path, "rb");
    //if(!file) return -1;

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    const int bufSize = 32768;
    char* buffer = malloc(bufSize);
    int bytesRead = 0;
    //if(!buffer) return -1;
    while((bytesRead = fread(buffer, 1, bufSize, file)))
    {
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