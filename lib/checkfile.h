#ifndef CHECKFILE_H
#define CHECKFILE_H

#include <stdbool.h>
#include <openssl/sha.h>

int check_directory(const char *);
void calc_sha256 (char*, char o[65]);
bool check_checksum(char s[], const unsigned char*);
void scan_dir(char*);

#endif /* CHECKFILE_H */