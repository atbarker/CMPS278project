//createZZs and populates a database for DND characters using berkeleyDB
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <sys/types.h>
#include <db.h>

#include "rollback.h"
#include "utils.h"
#include "simpledb.h"

//define the database names and the log file name
#define DATABASE "characters.db"
#define LOG "characters.db-log"

static int current_id;
static int randint[1024];
static int random_index;

int next_available_id(){
    int new_id = current_id;
    current_id++;
    return new_id;
}

int generate_random(int d){
    int i;
    for(i = 0; i < 1024; i++){
        randint[i] = rand() % d + 1;
    }
    return 0;
}

int select_random(){
    if(random_index < 1023){
        random_index++;
        return randint[random_index];
    }else if(random_index >= 1024){
        random_index = 0;
        generate_random(100);
        return randint[random_index];
    }
}

struct character create_character(char* name, char* class, int hp, int alive, int lvl){
    struct character ch;
    ch.id = next_available_id();
    ch.name = name;
    ch.class = class;
    ch.hp = hp;
    ch.alive = alive;
    ch.lvl = lvl;
    return ch;
}

struct character create_random_character(){
    struct character ch;
    int class = rand() % 12 + 1;
    ch.alive = 1;
    ch.lvl = 1;
    ch.hp = rand() % 15 + 1;
    switch(class){
        case 1: ch.class = "fighter"; break;
        case 2: ch.class = "barbarian"; break;
        case 3: ch.class = "bard"; break;
        case 4: ch.class = "cleric"; break;
        case 5: ch.class = "druid"; break;
        case 6: ch.class = "paladin"; break;
        case 7: ch.class = "ranger"; break;
        case 8: ch.class = "wizard"; break;
        case 10: ch.class = "warlock"; break;
        case 11: ch.class = "sorcerer"; break;
        case 12: ch.class = "monk"; break;
        default:
            ch.class = "thief";
            break;
    }
    int name = rand() % 12 + 1;
    switch(name){
        case 1: ch.name = "brian"; break;
        case 2: ch.name = "dan"; break;
        case 3: ch.name = "bob"; break;
        case 4: ch.name = "stanely"; break;
        case 5: ch.name = "aidan"; break;
        case 6: ch.name = "feanor"; break;
        case 7: ch.name = "fingolfin"; break;
        case 8: ch.name = "fingon"; break;
        case 9: ch.name = "finwe"; break;
        case 10: ch.name = "elendil"; break;
        case 11: ch.name = "isildur"; break;
        case 12: ch.name = "ancalagon"; break;
        default:
            ch.name = " ";
            break;
    }

    return ch;

}

//populate the database with a certain number of random transactions
//since retrieves have no bearing on the log, for the purposes of troubleshooting
//they may be omitted.
int populate_db(int trans, DB *dbp, DB_ENV *env, struct db_context *context){

    int i;
    int key = 0;

    for(i=0; i<trans; i++){
        int txn_type = rand() % 3 + 1;
        //struct character ch = create_random_character();
        struct character ch = create_random_character();
        key = bitmap_get_rand(context->id_bitmap, context->bitmap_size);
        switch(txn_type){
            case 1:
                insert(dbp, env, &ch, context);
                break;
            //case 2: 
            //    delete(dbp, env, key, context);
            //    break;
            case 3: 
                update(dbp, env, key, &ch, context);
                break;
            default:
                break;
        }
    }
    return 0;
}

DB* rollback_to_timestamp(struct db_context *context, DB_ENV *env, DB *dbp, char* new_db_name, int parallel, int records){
    //if we want to run a test in parallel, otherwise run it single threaded
    //in a linear manner.
    int time = 0;
    int partitions = 4;
    int rollback_lsn = records;
    struct rollback_summary *sum = NULL;

    if(parallel){
        sum = rollback_parallel(0, time, partitions, rollback_lsn, dbp, env, context);
    }else{
        sum = rollback_linear(NULL, dbp, env, context);
    }
   
    //rollback_destruct(sum);
    return NULL;
}


int main(int argc, char *argv[]){

    struct db_context *context = malloc(sizeof(struct db_context));
    DB *dbp = NULL;
    DBC *cursor = NULL;
    DB_ENV *env = NULL;
    int fill = 1;
    int transactions = 1024;
    random_index = 0;

    context->current_id = 0;
    context->next_available_id = 0;
    context->number_keys = 0;
    context->bitmap_size = transactions;
    context->id_bitmap = malloc(context->bitmap_size);
    memset(context->id_bitmap, 0, context->bitmap_size);

    int ret;
     
    if(ret = db_env_create(&env, 0)){
        fprintf(stderr, "Couldn't create db environment\n");
        return -1;
    }
 
    if(env->open(env, NULL, DB_INIT_LOG | DB_INIT_MPOOL | DB_CREATE | DB_THREAD | DB_INIT_LOCK, 0644)){
        fprintf(stderr, "Couldn't open db environement\n");
        return -1;
    }
    
    if(db_create(&dbp, env, 0)){
        fprintf(stderr, "Couldn't create database\n");
        return -1;
    }

    if(dbp->open(dbp, NULL, DATABASE, NULL, DB_BTREE, DB_CREATE, 0644) != 0 ){
	fprintf(stderr, "Database not found, or could not be opened, creating new one\n");
	return -1;
    };

    if(dbp->cursor(dbp, NULL, &cursor, 0) != 0 ){
        fprintf(stderr, "Cursor could not be set\n");
	return -1;
    }


    if(fill == 1){
        fprintf(stdout, "populating DB\n");
        populate_db(transactions, dbp, env, context);
        fprintf(stdout, "done\n");
    }

    //struct character ch = create_random_character();

    clock_t begin = clock();

    rollback_to_timestamp(context, env, dbp, "bort", 1, 200);

    clock_t end = clock();
    
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    //while(TRUE){
        dbp->close(dbp, 0);
        env->close(env, 0);
    //}
    free(context->id_bitmap);
    free(context);
    return 0;
}
