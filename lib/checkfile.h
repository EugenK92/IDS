#ifndef CHECKFILE_H
#define CHECKFILE_H

#include <stdbool.h>
#include <openssl/sha.h>

int check_if_allowed_path(char*);
int check_directory(const char *);
char* calc_sha256 (char*);
bool check_checksum(char s[], const unsigned char*);
void scan_dir(char*, int, int);

#endif /* CHECKFILE_H */