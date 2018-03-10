//creates and populates a database for DND characters using berkeleyDB
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
static int randint[1024];
static int random_index;

struct character{
    int id;
    char *name;
    char *class;
    int hp;
    int alive;
    int lvl;
}__attribute__((packed));

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

struct character* create_random_character(){
    struct character *ch = malloc(sizeof(struct character));
    int class = rand() % 12 + 1;
    ch->alive = 1;
    ch->lvl = 1;
    ch->hp = rand() % 15 + 1;
    switch(class){
        case 1: ch->class = "fighter"; break;
        case 2: ch->class = "barbarian"; break;
        case 3: ch->class = "bard"; break;
        case 4: ch->class = "cleric"; break;
        case 5: ch->class = "druid"; break;
        case 6: ch->class = "paladin"; break;
        case 7: ch->class = "ranger"; break;
        case 8: ch->class = "wizard"; break;
        case 10: ch->class = "warlock"; break;
        case 11: ch->class = "sorcerer"; break;
        case 12: ch->class = "monk"; break;
        default:
            ch->class = "thief";
            break;
    }
    int name = rand() % 12 + 1;
    switch(name){
        case 1: ch->name = "brian"; break;
        case 2: ch->name = "dan"; break;
        case 3: ch->name = "bob"; break;
        case 4: ch->name = "stanely"; break;
        case 5: ch->name = "aidan"; break;
        case 6: ch->name = "feanor"; break;
        case 7: ch->name = "fingolfin"; break;
        case 8: ch->name = "fingon"; break;
        case 9: ch->name = "finwe"; break;
        case 10: ch->name = "elendil"; break;
        case 11: ch->name = "isildur"; break;
        case 12: ch->name = "ancalagon"; break;
        default:
            ch->name = " ";
            break;
    }

    return ch;

}

//populate the database with a certain number of random transactions
int populate_db(int trans, DB *dbp){
    int i;
    DBT key, data;
    int ret;

    memset(&key, 0, sizeof(DBT));
    memset(&data, 0, sizeof(DBT));

    for(i=0; i<trans; i++){
        key.data = next_available_id;
        key.size = sizeof(int);
        data.data = create_random_character();
        data.size = sizeof(struct character);
        if((ret = dbp->put(dbp, NULL, &key, &data, 0))  == 0){
            fprintf(stderr, "Character stored\n");
        }else{
            fprintf(stderr, "character populate failed\n");
            dbp->err(dbp, ret, "DB->put");
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
    int fill = 1;
    random_index = 0;
    current_id = 0;
    int transactions = 1024;
    int ret;
    
    db_create(&dbp, NULL, 0);

    if(dbp->open(dbp, NULL, DATABASE, NULL, DB_BTREE, DB_CREATE, 0777) != 0 ){
	fprintf(stderr, "Database not found, or could not be opened, creating new one\n");
	//if(db_create(&dbp, NULL, 0)){
        //    fill = 1;
        //    ret = dbp->open(dbp, NULL, DATABASE, NULL, DB_BTREE, DB_CREATE, 0);
        //}
    };

    if(dbp->cursor(dbp, NULL, &cursor, 0) != 0 ){
        fprintf(stderr, "Cursor could not be set\n");
	return -1;
    }

    memset(&key, 0, sizeof(DBT));
    memset(&data, 0, sizeof(DBT));

    if(fill == 1){
        fprintf(stdout, "populating DB\n");
        populate_db(transactions, dbp);
        fprintf(stdout, "done\n");
    }

    while(TRUE){

    }
    return 0;
}
