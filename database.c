#include <stdio.h>
#include <sqlite3.h> 

sqlite3* connect() {
    sqlite3 *db;

   int rc = sqlite3_open("sqlite/ids.db", &db);

   if (rc) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
   } 
   else {
      //fprintf(stderr, "Connected to database.\n");
   }

    return db;
}

int callback(void *db, int argc, char **argv, char **azColName) {
    return 0;
}

int show_data(void *db, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {

        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    return argc;
}

int count_data(void *db, int argc, char **argv, char **azColName) {
    return argc;
}

void create_table() {
    sqlite3 *db = connect();

    char *query = "CREATE TABLE IF NOT EXISTS filelist (" \
                  " id INTEGER PRIMARY KEY AUTOINCREMENT," \
                  " path TEXT," \
                  " checksum TEXT," \
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

void select_data() {
    sqlite3 *db = connect();

    char *query = "SELECT * FROM filelist;";
    
    char *errmsg = NULL;
    int rc = sqlite3_exec(db, query, show_data, 0, &errmsg);
    printf("%d\n", rc);    
}

void insert_data(char* path, char* checksum) {
    sqlite3 *db = connect();

    // char *errmsg = NULL;
    sqlite3_stmt *stmt;

    char *query = "SELECT * FROM filelist WHERE checksum = ?";
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);

    if (rc == SQLITE_OK) { 
        sqlite3_bind_text(stmt, 1, checksum, sizeof(checksum), SQLITE_STATIC);
    } 
    else {    
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    
    rc = sqlite3_step(stmt);
    
    if (rc == SQLITE_ROW) {
        printf("%s\n", sqlite3_column_text(stmt, 0));
    }

    sqlite3_finalize(stmt);
}