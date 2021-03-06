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

int get_file_size(FILE* file) {
    size_t len;
    fseek(file, 0, SEEK_END);
    len = ftell(file);
    rewind(file);

    return len;
}

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
    char* algorithm = (char*) malloc(sizeof(char) * 1024);
    strcpy(algorithm, "");  
    parseDoc("rules.xml", "hash", "algorithm", algorithm, 0);
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

    free(algorithm);
    return size;
}

//Source: https://www.reddit.com/r/crypto/comments/7ooot2/using_libsodiums_generic_hash_to_hash_a_file/
void hash_libsodium(char* path, char* hex) {

    if (sodium_init() < 0) {
        printf("Cannot use this. Because of reasons\n");
    }
    else {
        int hash_size = 64;        
        int buff_size = 255;
        crypto_generichash_state state;
        unsigned char buf[buff_size];
        unsigned char out[hash_size];
        size_t read;

        FILE* file = fopen(path, "rb");
        if (file) {
            size_t len = get_file_size(file);
            crypto_generichash_init(&state, NULL, 0, hash_size);
            int fail = false;
            if (len != -1) {
                while(!feof(file) && !fail) { 
                    read = fread(buf, 1, buff_size, file);
                    if (read * sizeof(buf) > 0) {
                        crypto_generichash_update(&state,buf,read);
                    }
                    else {
                        fail = true;
                    }
                }
                crypto_generichash_final(&state, out, hash_size);
                for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
                    sprintf(hex + (i * 2), "%02x", out[i]);
                }
            }

            fclose(file);
        }

    }
}

// Source: https://stackoverflow.com/questions/7853156/calculate-and-print-sha256-hash-of-a-file-using-openssl
void hash_sha256(char* path, char* output) {
    FILE* file = fopen(path, "rb");
    if (file) {
        const int bufSize = 65535;
        char* buffer = (char*) malloc(sizeof(char) * bufSize);
        size_t len = get_file_size(file);
        if (len != -1) {
            unsigned char hash[SHA256_DIGEST_LENGTH];
            SHA256_CTX sha256;
            SHA256_Init(&sha256);
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
        }

        fclose(file);
        free(buffer);
    }
}

void hash_sha512(char* path, char* output) {
    FILE* file = fopen(path, "rb");
    if (file) {
        const int bufSize = 65535;
        char* buffer = (char*) malloc(sizeof(char) * bufSize);
        size_t len = get_file_size(file);
        if (len != -1) {
            unsigned char hash[SHA512_DIGEST_LENGTH];
            SHA512_CTX sha512;
            SHA512_Init(&sha512);
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
        }

        fclose(file);
        free(buffer);
    }
}

void calc_checksum(char* path, char* output) {

    char* algorithm = (char*) malloc(sizeof(char) * 1024);
    strcpy(algorithm, "");  
    parseDoc("rules.xml", "hash", "algorithm", algorithm, 0);
    memmove(algorithm, algorithm+1, strlen(algorithm));
    if (strcmp(algorithm, "libsodium") == 0) {
        hash_libsodium(path, output);
    }
    else if (strcmp(algorithm, "SHA256") == 0) {
        hash_sha256(path, output);
    }
    else if (strcmp(algorithm, "SHA512") == 0) {
        hash_sha512(path, output);
    }
    else {
        printf("Algorithmus ist nicht bekannt. Nutze SHA256\n");
        hash_sha256(path, output);
    }
    free(algorithm);
}  

