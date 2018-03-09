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
    // create_table();
    select_all_data();
    put_data("blub/blub/wdwd", "ABC1234", 0);
    printf("\n");
    select_all_data();
    
}

