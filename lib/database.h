#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

sqlite3* connect();
int callback(void *, int, char**, char**);
void create_table();
int show_data(void *, int, char**, char**);
int count_data(void *, int, char**, char**);
void select_data();
void insert_data(char*, char*);

#endif /* DATABASE_H */