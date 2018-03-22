//include external libraries
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <dirent.h>
#include <unistd.h>

//include internal libraries
#include "lib/checkfile.h"
#include "lib/database.h"

/* sudo apt-get install libsqlite3-dev */

int check_parameter(int len, char* argv[], char* param);
void print_manual();

int main (int argc, char* argv[]) {

    if (argc < 2) {
        print_manual();
    } 
    else {
        if (check_parameter(argc, argv, "--init") != 0 || check_parameter(argc, argv, "-i") != 0) {
            printf("Start init\n");
        }
        else if (check_parameter(argc, argv, "--check") != 0 || check_parameter(argc, argv, "-c") != 0) {
            printf("Start check\n");
        }
        else if (check_parameter(argc, argv, "--help") != 0 || check_parameter(argc, argv, "-h") != 0) {
            print_manual();
        }
        else {
            print_manual();
        }
    }   
//     create_table();
//    // chdir("/etc");
//    if (argc < 2) {
//        printf("usage: ./IDS <starting path>\n");
//    }
//    else {
//        scan_dir(argv[1]);
//    }
    // printf("\n");
    // printf("%s\n", select_all_data());
    
}

int check_parameter(int len, char* argv[], char* param) {
    int end = false;
    int index = 0;
    int result = 0;
    while (!end && index < len - 1) {
        index++;
        if (strcmp(argv[index], param) == 0) {
            result = index;
            end = true;
        }
    }
    return result;
}

void print_manual() {
    printf("Aufruf: ./IDS [OPTION]... [PFAD]...\n");
    printf("Scannen des angegebenen PFADes. Standard ist das root '/' Verzeichnis.\n");
    printf("Erforderliche Argumente für lange Optionen sind auch für kurze erforderlich.\n");
    printf("\t-c, --check\t\tFühre das Programm für ein Verzeichnis aus. Standard ist das root '/' Verzeichnis.\n");
    printf("\t-h, --help\t\tGibt diese Anleitung aus.\n");
    printf("\t-i, --init\t\tFührt die Initialanweisungen für den ersten Start des Programms aus.\n");
    printf("\nLICENSE\tGNU General Public License v3.0\n");
}










