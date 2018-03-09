//include external libraries
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sqlite3.h>

//include internal libraries
#include "lib/checkfile.h"
#include "lib/database.h"

/* sudo apt-get install libsqlite3-dev */

int main (int argc, char** argv)
{
    // scan_dir();
    //create_table();
   // put_data("README.md", "5fde7f704806740b791a386ca361a20cf8108398b93e375336e688037e9b8050", 0);
    // printf("\n");
    printf("%s\n", select_all_data());
    
}

