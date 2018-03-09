//creates and populates a database for DND characters using berkeleyDB
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <sys/types.h>
#include <db.h>

#include "rollback.h"
#include "utils.h"

//define the database names and the log file name
#define DATABASE "characters.db"
#define LOG "characters.db-log"

static int current_id;
static int random[1024];
static random_index;

struct character{
    int id;
    char name[20];
    char class[20];
    int hp;
    int alive;
    int lvl;
}__attribute__((packed));

int next_available_id(){
    int new_id = current_id;
    current_id++;
    return new_id;
}

int generate_random(){
    return 0;
}

int select_random(){
    if(random_index < 1023){
        random_index++;
        return random[random_index];
    }else if(random_index >= 1024){
        random_index = 0;
        generate_random();
        return random[random_index];
    }
}

struct character* create_character(char* name, char* class, int hp, int alive, int lvl){
    struct character *ch = malloc(sizeof(struct character));
    ch->id = next_available_id();
    ch->name = name;
    ch->class = class;
    ch->hp = hp;
    ch->alive = alive;
    ch->lvl = lvl;
    return ch;
}

struct character create_random_character(){

}

//populate the database with a certain number of random transactions
int populate_db(int trans, DB *dbp){
    int i;
    DBT key, data;

    memset(&key, 0, sizeof(DBT));
    memset(&key, 0, sizeof(DBT));

    for(i=0; i<trans; i++){
        key.data = next_available_id;
        key.size = sizeof(int);
        data.data = create_random_character();
        data.size = sizeof(character);
        if(dbp->put(dbp, NULL, &key, &data, 0)  == 0){
            fprintf(stderr, "Character stored");
        }else{
            fprintf(stderr, "character populate failed\n");
        }
    }
    return 0;
}

int rollback_to_timestamp(char* new_db_name){
    return 0;
}


int main(int argc, char *argv[]){

    static DB *dbp = NULL;
    DBT key, data;
    DBC *cursor = NULL;
    int fill = 0;
    random_index = 0;
    current_id = 0;
    int transactions = 1024;

    if(dbp->open(dbp, NULL, DATABASE, NULL, DB_BTREE, DB_CREATE, 0) != 0 ){
	fprintf(stderr, "Database not found, or could not be opened, creating new one\n");
	if(db_create(dbp, NULL, 0)){
            fill = 1;
            ret = dbp->open(dbp, NULL, DATABASE, NULL, DB_BTREE, DB_CREATE, 0);
        }
    };

    if(dbp->cursor(dpb, NULL, &cursor, 0) != 0 ){
        fprintf(stderr, "Cursor could not be set\n");
	return -1;
    }

    memset(&key, 0, sizeof(DBT));
    memset(&data, 0, sizeof(DBT));

    if(fill == 1){
        populate_db(transactions, dbp);
    }

    while(true){

    }
    return 0;
}
