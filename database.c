#include <stdio.h>
#include <sqlite3.h> 

sqlite3* connect() {
    sqlite3 *db;

   int rc = sqlite3_open("sqlite/ids.db", &db);

   if (rc) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
   } 
   else {
      fprintf(stderr, "Connected to database.\n");
   }

    return db;
}

int callback(void *db, int argc, char **argv, char **azColName) {
    return 0;
}

void create_table() {
    sqlite3 *db = connect();

    char *query = "CREATE TABLE IF NOT EXISTS filelist (" \
                  " id INTEGER PRIMARY KEY AUTOINCREMENT," \
                  " path TEXT," \
                  " created_at TIMESTAMP," \
                  " updated_at TIMESTAMP" \
                  ");";

    char *errmsg = NULL;
    int rc = sqlite3_exec(db, query, callback, 0, &errmsg);
    if (rc == SQLITE_OK) {
        printf("Table created.\n");
    }
    else {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);     
    }

   sqlite3_close(db);
}

void insert_data(char* path) {
    sqlite3 *db = connect();

    char *query = "";
}