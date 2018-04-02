#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

sqlite3* connect();
void send_to_log(char*, char*);
int callback(void *, int, char**, char**);
void create_table(int);
int check_data_by_path(char*);
int check_file_change(char*, char*);
void select_checksum_by_path(char*, char*);
char* select_all_data();
int update_data(int, char*, char*);
void get_current_timestamp(char*);
int insert_data(char*, char*);
int put_data(char*, char*, int, int);

#endif /* DATABASE_H */