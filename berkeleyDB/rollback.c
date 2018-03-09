
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <assert.h>
#include <pthread.h>
#include "rollback.h"
#include "utils.h"

void* rollback_worker(void *args){
    struct thread_args *td_args = args;
    	
    return NULL;
}

void* shutdown(){

    return NULL;
}

//compile my rolled up log linearly
void* rollback_linear(){
    return NULL;
} 

void* merge_partitions(){
    
    return NULL;
}

//a page is only valid if the salts match the header 
//checksums must match
//roll up my log in parallel chunks
void* rollback_parallel(int time_quanta, int number_partitions){
    
    //if you are doing shit
    if(time_quanta == 0){
	//partition it into sections based on quantum

	//start thread for each section 

    }else{
        //partition it into number partition sections

    }

    //merge the partitions
    return NULL;
}

int rollback(void *args, int numthreads,int dbfile, int walfile){
    
    return 0;
}
