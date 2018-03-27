#ifndef HASH_H
#define HASH_H

int check_directory(const char *);
int get_output_size();
char* hash_libsodium(char*);
char* hash_sha256(char*); 
char* hash_sha512(char*); 
char* calc_checksum (char*);

#endif /* HASH_H */