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
#include "lib/xml.h"

/* sudo apt-get install libsqlite3-dev */
/* sudo apt-get install libxml2 libxml2-dev */

int check_parameter(int len, char* argv[], char* param);
void print_manual();
int check_package(char* package);

int main (int argc, char* argv[]) {
    if (check_package("dpkg -s libsqlite3-dev") == 1 && check_package("dpkg -s libxml2") == 1) {
        if (argc < 2) {
            print_manual();
        } 
        else {
            //Source: https://stackoverflow.com/a/4159919
            if (geteuid() != 0) {
               printf("Sie führen das Programm ohne root Rechte aus. Einige Dateien können möglicherweise nicht geöffnet werden.\n");
            }

            int modus = 0;
            if (check_parameter(argc, argv, "--show_new") != 0 || check_parameter(argc, argv, "-v") != 0) {
                modus = 3;
            }
            if (check_parameter(argc, argv, "--show_changed") != 0 || check_parameter(argc, argv, "-v") != 0) {
                modus = 2;
            }
            if (check_parameter(argc, argv, "--verbose") != 0 || check_parameter(argc, argv, "-v") != 0) {
                modus = 1;
            }
            if (check_parameter(argc, argv, "--init") != 0 || check_parameter(argc, argv, "-i") != 0) {
                create_table(modus); 
            }
            else if (check_parameter(argc, argv, "--check") != 0 || check_parameter(argc, argv, "-c") != 0) {
                int pos = check_parameter(argc, argv, "--check") != 0 ? check_parameter(argc, argv, "--check") : check_parameter(argc, argv, "-c"); 
                int update = 0;
                if (check_parameter(argc, argv, "--update") != 0 || check_parameter(argc, argv, "-u") != 0) {
                    update = 1;
                }
                if (argc > pos + 1) {
                    scan_dir(argv[pos + 1], modus, update);
                }
                else {
                    scan_dir("/", modus, update);                    
                }
            }
            else if (check_parameter(argc, argv, "--help") != 0 || check_parameter(argc, argv, "-h") != 0) {
                print_manual();
            }
            else if (check_parameter(argc, argv, "--show_exclude") != 0) {
                parseDoc("rules.xml", "exclude_path", "path", 1);
            }
            else {
                print_manual();
            }
        }   
    }
    
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
    printf("\t--show_changed\t\tZeigt die geänderten Dateien an. Wenn die --verbose Option verwendet wird, wird diese Option ignoriert.\n");
    printf("\t--show_exclude\t\tZeigt die zu ignorierenden Pfade an. Pfade sind in der rules.xml Datei definiert.\n");
    printf("\t--show_new\t\tZeigt die neuen Dateien an. Wenn die --verbose oder --show_changed Option verwendet wird, wird diese Option ignoriert.\n");
    printf("\t-u, --update\t\tAktualisiert die Checksumme für geänderte Dateien. Diese Option wird zusammen mit --check verwendet.\n");
    printf("\t-v, --verbose\t\tZeigt ausführliche Informationen an.\n");
    printf("\nVerfügbare Hashalgorithmen:\n");
    printf("\tSHA256\n");
    printf("\tlibsodium Benutzt BLAKE2b\n");
    printf("\nLICENSE\tGNU General Public License v3.0\n");
}

// Source: https://stackoverflow.com/questions/43116/how-can-i-run-an-external-program-from-c-and-parse-its-output
int check_package(char* package) {
    char *cmd = package;    

    char buf[128];
    FILE *fp;

    if ((fp = popen(cmd, "r")) == NULL) {
        printf("Error opening pipe!\n");
        return -1;
    }

    int found = 0;
    while (fgets(buf, 128, fp) != NULL) {
        if (strstr(buf, "Status: install ok installed") != NULL) {
            found = 1;
        }
    }

    if(pclose(fp))  {
        printf("Command not found or exited with error status\n");
        return -1;
    }

    return found;
}








