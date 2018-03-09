#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <time.h> 

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

void select_all_data() {
    sqlite3 *db = connect();
    sqlite3_stmt *stmt;
    char *query = "SELECT * FROM filelist;";
    
    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("%d %s %s %s %s\n", sqlite3_column_int(stmt, 0), 
                                   sqlite3_column_text(stmt, 1), 
                                   sqlite3_column_text(stmt, 2),
                                   sqlite3_column_text(stmt, 3),
                                   sqlite3_column_text(stmt, 4)
        );
    }

    sqlite3_finalize(stmt); 
}

/**
 * returns 100 if data is available
*/
int check_data_by_path(char* path) {
    sqlite3 *db = connect();
    sqlite3_stmt *stmt;

    char *query = "SELECT * FROM filelist WHERE path = ?";
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, path, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt); 
    return rc;   
}

char* get_current_timestamp() {
    time_t timer;
    char* buffer = (char *) malloc(sizeof(char) * 21);
    struct tm* tm_info;

    time(&timer);
    tm_info = localtime(&timer);
    strftime(buffer, 21, "%Y-%m-%d %H:%M:%S", tm_info);
    return buffer;
}

int update_data(int id, char * path, char* checksum) {
    sqlite3 *db = connect();
    sqlite3_stmt *stmt;

    char* current_time = get_current_timestamp();
    char *query = "UPDATE filelist SET path = ?, checksum = ?, updated_at = ? WHERE id = ?";
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, path, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, checksum, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, current_time, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, id);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt); 
    return rc;
}

int insert_data(char* path, char* checksum) {
    sqlite3 *db = connect();
    sqlite3_stmt *stmt;

    char* current_time = get_current_timestamp();
    char *query = "INSERT INTO filelist (path, checksum, created_at, updated_at) VALUES (?, ?, ?, ?);";
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, path, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, checksum, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, current_time, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, current_time, -1, SQLITE_STATIC);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt); 
    return rc;
}

int put_data(char* path, char* checksum, int id) {
    int result = 0;
    
    if (check_data_by_path(path) == 101) {
        result = insert_data(path, checksum);
    }
    else {
        result = update_data(id, path, checksum);
    }

    return result;
}