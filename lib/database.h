#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

sqlite3* connect();
int callback(void *db, int argc, char **argv, char **azColName);
void create_table();

#endif /* DATABASE_H */