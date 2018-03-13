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
    char* temp_path = depth == 0 ? construct_path("", path) : path;
    printf("~~~~~~~~~~~~~~~~~~ Depth: %d Path: %s\n\n", depth, temp_path);
    int n = scandir(temp_path, &namelist, NULL, alphasort);
    printf("\nn: %d\n", n);
    if (n < 0) {
        perror("scandir");
    }
    else {
        for (int i = 0; i < n; i++) {
            if (strcmp(namelist[i]->d_name, ".") != 0 && strcmp(namelist[i]->d_name, "..") != 0) {
                temp_path = construct_path(path, namelist[i]->d_name);
                printf("PATH: %s\n", temp_path);
                if (check_directory(namelist[i]->d_name) == 1) {
                    printf("### %s\n", temp_path);
                    scan_dir(temp_path, depth + 1);
                }
                else if (check_directory(temp_path) != -1) {
                    printf("%s => %s\n", namelist[i]->d_name, calc_sha256(namelist[i]->d_name));
                }
                else {
                    // put_data(namelist[n]->d_name, calc_sha256(namelist[n]->d_name, output), 0);
                    printf("%s => FAILS!\n", temp_path);                    
                }
            }
        }
        free(namelist);
    }     
}

int check_undefined_file(char* path) {

    return 0;
}

void add_path(char* current, char* additional) {
    if (additional[0] != '\0') {
        strcat(current, "/");
    }
    strcat(current, additional);
}

char* construct_path(char* current_path, char* additional_path) {
    char cwd[1024];
    char* result = strstr(current_path, getcwd(cwd, sizeof(cwd)));
    char* temp_path = (char *) malloc(sizeof(char) * 128);
    strcpy(temp_path,"");
    if (result == NULL) {
        strcpy(temp_path, getcwd(cwd, sizeof(cwd)));
        add_path(temp_path, current_path);
    }
    add_path(temp_path, additional_path);
    return temp_path;
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