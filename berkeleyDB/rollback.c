
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <assert.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include "rollback.h"
#include "simpledb.h"
#include "utils.h"

struct thread_args {
    void *mem;
    uint64_t memsize;  
};

//retrieve a batch of log records and crunch the numbers
void* rollback_worker(void *args){
    struct thread_args *td_args = args;
    	
    return NULL;
}

//shutdown a worker
void* shutdown(){

    return NULL;
}

void *printGarbage(){
    printf("thread yay\n");
}

//compile my rolled up log linearly
struct rollback_summary* rollback_linear(DB_LSN *lsn, DB *dbp, DB_ENV *env, struct db_context *context){

    DB_LOGC *cursor;
    DB_LSN *last_lsn = malloc(sizeof(DB_LSN));
    DBT *log_contents = malloc(sizeof(DBT) * 6);
    int i;

    //create the cursor
    if(env->log_cursor(env, &cursor, 0)){
        fprintf(stderr, "Error creating database cursor\n");
        return NULL;
    }

    //have to initialize the objects
    memset(last_lsn, 0, sizeof(DB_LSN));
    memset(log_contents, 0, sizeof(DBT));
    log_contents->data = NULL;
    log_contents->size = sizeof(struct db_log_record);
 
    //scan from the most recent log record to the correct timestamp or LSN
    printf("grabbing first log record\n");
    cursor->get(cursor, last_lsn, log_contents, DB_LAST);

    //grab each log record starting from that LSN
    for(i = 1; i < 6; i++){
        printf("Grabbing recent log %d\n", i);
        cursor->get(cursor, last_lsn, log_contents, DB_PREV);
    }

    //cleanup
    cursor->close(cursor, 0);
    free(log_contents);
    free(last_lsn);
    return NULL;
} 

//merge the partitions created by the workers
void* merge_partitions(){
    
    return NULL;
}

//a page is only valid if the salts match the header 
//checksums must match
//roll up my log in parallel chunks
struct rollback_summary* rollback_parallel(
    int number_records, 
    int time_quanta, 
    int number_partitions, 
    int rollback_lsn, 
    struct db_context *context)
{
    
    pthread_t *threads;
    int i;
    int quanta_divisions;
    int ret;
    
    //if a time quantum is specified then divide up the log records by time quantum 
    if(time_quanta != 0){
        quanta_divisions = number_records/time_quanta;
	threads = malloc(sizeof(pthread_t)*quanta_divisions);
        for(i=0; i < quanta_divisions; i++){
            if(pthread_create(&threads[i], NULL, printGarbage, NULL)){
                fprintf(stderr, "Couldn't create thread %d\n", i);
            }
            printf("Thread %d created\n", i);
        }
        for(i=0; i < quanta_divisions; i++){
            pthread_join(threads[i], NULL);
        }
    }
    //if not then divide them up by the number of partitions
    else{
        threads = malloc(sizeof(pthread_t)* number_partitions);
        for(i=0; i < number_partitions; i++){
            ret = pthread_create(&threads[i], NULL, printGarbage, NULL);
            if(ret){
                fprintf(stderr, "Could not create thread %d\n", i);
            }
            printf("Thread %d created\n", i);
        }
        for(i=0; i < number_partitions; i++){
            pthread_join(threads[i], NULL);
        }
    }

    free(threads);
    return NULL;
}
