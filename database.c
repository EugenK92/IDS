#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sqlite3.h>
#include <time.h> 

#include "lib/hash.h"
#include "lib/log.h"

sqlite3* connect() {
    sqlite3 *db;
    int rc = sqlite3_open_v2("sqlite/ids.db", &db, SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, NULL);
    
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

void create_table(int modus) {
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
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg); 
    }
    if (modus == 1) {
       printf("Table created\n");
    }

   sqlite3_close_v2(db);
}

char* select_all_data() {
    sqlite3 *db = connect();
    sqlite3_stmt *stmt;
    char *query = "SELECT * FROM filelist;";
    
    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    int length = 0;
    char* result = (char *) malloc(sizeof(char) * 65535);
    /* remove garbage */
    strcpy(result,"");
    char* temp = (char *) malloc(sizeof(char) * 512);
    char len[20];
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        snprintf(temp, 256, "id=%d;path=%s;checksum=%s;created_at=%s;updated_at=%s;\n", 
                                sqlite3_column_int(stmt, 0), 
                                sqlite3_column_text(stmt, 1), 
                                sqlite3_column_text(stmt, 2),
                                sqlite3_column_text(stmt, 3),
                                sqlite3_column_text(stmt, 4)
        );
        length++;
        strcat(result, temp);
    }
    sprintf(len, "length=%d", length);
    strcat(result, len);

    // printf("%s\n", result);
    sqlite3_finalize(stmt);
    sqlite3_close_v2(db);
    return result; 
}

char* select_checksum_by_path(char* path) {
    sqlite3 *db = connect();
    sqlite3_stmt *stmt;
    char *query = "SELECT checksum FROM filelist WHERE path = ?;";
    
    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, path, -1, SQLITE_STATIC);

    char* result = (char *) malloc(sizeof(char) * 65535);
    /* remove garbage */
    strcpy(result,"");
    char* temp = (char *) malloc(sizeof(char) * 512);
    int size = get_output_size();
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        snprintf(temp, size, "%s", sqlite3_column_text(stmt, 0));
        strcat(result, temp);
    }
    sqlite3_finalize(stmt);
    sqlite3_close_v2(db);
    return result; 
}

/**
 * returns 0 if data is not available
*/
int check_data_by_path(char* path) {
    sqlite3 *db = connect();
    sqlite3_stmt *stmt;

    char *query = "SELECT * FROM filelist WHERE path = ?";
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, path, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        rc = sqlite3_column_int(stmt, 0);
    }
    else {
        rc = 0;
    }
    sqlite3_finalize(stmt); 
    sqlite3_close_v2(db);
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
    sqlite3_close_v2(db); 
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
    sqlite3_close_v2(db);
    return rc;
}

int check_file_change(char* path, char* current_checksum) {
    char* data = select_checksum_by_path(path);
    return strcmp(current_checksum, data);
}

int put_data(char* path, char* checksum, int modus, int update) {
    int result = 0;
    int data = check_data_by_path(path);
    if (data == 0) {
        result = insert_data(path, checksum);
        char* log_string = (char*) malloc (sizeof(char*) * 128);
        strcpy(log_string, "New file was found: ");
        strcat(log_string, path);
        if (modus == 1 || modus == 3) {
            printf("New file was found: %s\n", path);
            write_to_logfile(log_string, get_current_timestamp(), "log_newfile.txt");
        }
        write_to_logfile(log_string, get_current_timestamp(), "log.txt");
    }
    else {
        result = check_file_change(path, checksum);
        if (result != 0) {
            char* log_string = (char*) malloc (sizeof(char*) * 128);
            strcpy(log_string, "File: ");
            strcat(log_string, path);
            strcat(log_string, " was changed");
            if (modus == 1 || modus == 2) {
                printf("File: %s was changed\n", path);
                write_to_logfile(log_string, get_current_timestamp(), "log_changedfile.txt");            
            }           
            write_to_logfile(log_string, get_current_timestamp(), "log.txt");            
        }
        if (update == 1) {
            update_data(data, path, checksum);
            printf("Entry: %s %s updated\n", path, checksum);
        }
    }

    return result;
}