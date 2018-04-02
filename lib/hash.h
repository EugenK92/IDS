#ifndef HASH_H
#define HASH_H

int get_file_size(FILE*);
int check_directory(const char *);
int get_output_size();
void hash_libsodium(char*, char*);
void hash_sha256(char*, char*); 
void hash_sha512(char*, char*); 
void calc_checksum (char*, char*);

#endif /* HASH_H */