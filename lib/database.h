#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

sqlite3* connect();
int callback(void *, int, char**, char**);
void create_table(int);
int count_data(void *, int, char**, char**);
int check_data_by_path(char*);
char* filter_value(char*, char*);
char* select_checksum_by_path(char*);
char* select_all_data();
int update_data(int, char*, char*);
char* get_current_timestamp();
int insert_data(char*, char*);
int put_data(char*, char*, int, int);

#endif /* DATABASE_H */