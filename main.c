//include external libraries
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <dirent.h>
#include <unistd.h>

//include internal libraries
#include "lib/checkfile.h"
#include "lib/database.h"

/* sudo apt-get install libsqlite3-dev */

int main (int argc, char** argv)
{
    
    // create_table();
   // chdir("/etc");
    scan_dir(argv[1]);
    // printf("\n");
    // printf("%s\n", select_all_data());
    
}

