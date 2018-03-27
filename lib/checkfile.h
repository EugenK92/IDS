#ifndef CHECKFILE_H
#define CHECKFILE_H

#include <stdbool.h>
#include <openssl/sha.h>

int check_directory(const char *);
char* calc_sha256 (char*);
bool check_checksum(char s[], const unsigned char*);
void scan_dir(char*, int);

#endif /* CHECKFILE_H */