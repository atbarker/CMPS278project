#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <sys/types.h>

#include "rollback.h"
#include "utils.h"

/*
 *argv[0] program name
 *argv[1] time desired
 *argv[2] DB file
 *argv[3] new db file 
 *argv[4] WAL file
 */
int main(int argc, char *argv[]){
    struct wal_file *log = read_wal("walbackup1");
    printf("Number of pages: %d\n", log->number_pages);
    rollback_linear(log);
    return 0;
}
