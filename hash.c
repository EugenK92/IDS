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
#include <sodium.h>

#include "lib/hash.h"
#include "lib/xml.h"

// Source: https://stackoverflow.com/a/4553076
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

int get_output_size() {
    int size = 0;
    char* algorithm = parseDoc("rules.xml", "hash", "algorithm", 0);
    memmove(algorithm, algorithm+1, strlen(algorithm));
    
    if (strcmp(algorithm, "libsodium") == 0) {
        size = 64 * 2 + 1;
    }
    else if (strcmp(algorithm, "SHA256") == 0) {
        size = SHA256_DIGEST_LENGTH * 2 + 1;
    }
    else if (strcmp(algorithm, "SHA512") == 0) {
        size = SHA512_DIGEST_LENGTH * 2 + 1;
    }    

    return size;
}

//Source: https://www.reddit.com/r/crypto/comments/7ooot2/using_libsodiums_generic_hash_to_hash_a_file/
char* hash_libsodium(char* path) {

    int hash_size = 64;
    char* hex = (char*) malloc(sizeof(char) * (hash_size * 2 + 1));
    strcpy(hex, "");
    if (sodium_init() < 0) {
        printf("Cannot use this. Because of reasons\n");
    }
    else {
        int buff_size = 255;
        crypto_generichash_state state;
        unsigned char buf[buff_size];
        unsigned char out[hash_size];
        size_t read;

        FILE* file = fopen(path, "rb");
        if (file) {
            crypto_generichash_init(&state, NULL, 0, hash_size);
            while(!feof(file)) {
                read = fread(buf, 1, buff_size, file);
                crypto_generichash_update(&state,buf,read);
            }
            crypto_generichash_final(&state, out, hash_size);
            for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
                sprintf(hex + (i * 2), "%02x", out[i]);
            }
            fclose(file);
        }

    }

    return hex;
}

// Source: https://stackoverflow.com/questions/7853156/calculate-and-print-sha256-hash-of-a-file-using-openssl
char* hash_sha256(char* path) {
    FILE* file = fopen(path, "rb");
    char* output = (char*) malloc(sizeof(char) * (SHA256_DIGEST_LENGTH * 2 + 1));
    strcpy(output, "");
    if (file) {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        const int bufSize = 65535;
        char* buffer = (char*) malloc(sizeof(char) * bufSize);
        int bytesRead = 0;
        if (check_directory(path) == 1) {
            while((bytesRead = fread(buffer, 1, bufSize, file))) {
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

char* hash_sha512(char* path) {
    FILE* file = fopen(path, "rb");
    char* output = (char*) malloc(sizeof(char) * (SHA512_DIGEST_LENGTH * 2 + 1));
    strcpy(output, "");
    if (file) {
        unsigned char hash[SHA512_DIGEST_LENGTH];
        SHA512_CTX sha512;
        SHA512_Init(&sha512);
        const int bufSize = 65535;
        char* buffer = (char*) malloc(sizeof(char) * bufSize);
        int bytesRead = 0;
        if (check_directory(path) == 1) {
            while((bytesRead = fread(buffer, 1, bufSize, file))) {
                SHA512_Update(&sha512, buffer, bytesRead);
            }
            SHA512_Final(hash, &sha512);

            for (int i = 0; i < SHA512_DIGEST_LENGTH; i++) {
                sprintf(output + (i * 2), "%02x", hash[i]);
            }
        }

        fclose(file);
        free(buffer);
    }

    return output;
}

char* calc_checksum (char* path) {

    char* algorithm = parseDoc("rules.xml", "hash", "algorithm", 0);
    memmove(algorithm, algorithm+1, strlen(algorithm));
    char* output = (char*) malloc(sizeof(char) * get_output_size());
    strcpy(output, "");
    
    if (strcmp(algorithm, "libsodium") == 0) {
        output = hash_libsodium(path);
    }
    else if (strcmp(algorithm, "SHA256") == 0) {
        output = hash_sha256(path);
    }
    else if (strcmp(algorithm, "SHA512") == 0) {
        output = hash_sha512(path);
    }
    else {
        printf("Algorithmus ist nicht bekannt. Nutze SHA256\n");
        output = hash_sha256(path);
    }
    return output;
}  

