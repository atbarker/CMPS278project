
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <assert.h>
#include <pthread.h>
#include "rollback.h"

void* rollback_worker(void *args){
    struct thread_args *td_args = args;
    	
    return NULL;
}

void* shutdown(){

    return NULL;
} 

int rollback(void *args, int numthreads,int dbfile, int walfile){
    
    return 0;
}
