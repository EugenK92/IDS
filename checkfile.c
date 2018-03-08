#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <openssl/sha.h>

#include "lib/checkfile.h"

void scan_dir() {
    struct dirent **namelist;
    int n = 0;
    char output[65];

    n = scandir(".", &namelist, NULL, alphasort);
    if (n < 0) {
        perror("scandir");
    }
    else {
        while (n--) {
            printf("%s ", namelist[n]->d_name);
            printf("%s\n", calc_sha256(namelist[n]->d_name, output));
            free(namelist[n]);
        }
        free(namelist);
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