#ifndef SIMPLEDB_H
#define SIMPLEDB_H

#include <db.h>
#include <time.h>
#include "utils.h"

#define RECORD_SIZE 40;

struct db_context{
    DB *dbp;
    DB_ENV *env;
    DB_LSN recent_lsn;
    int number_lsn;
    int current_id;
    int next_available_id;
    int number_keys;
    int randint[1024];
    int rand_index;
    int bitmap_size;
    unsigned char *id_bitmap;    
};

struct character{
    int id;
    char *name;
    char *class;
    int hp;
    int alive;
    int lvl;
}__attribute__((packed));

struct db_log_record{
    uint64_t time;
    uint32_t XID;
    uint32_t type;
    uint32_t key;
    //uint32_t offset;
    uint32_t data_length;
    unsigned char data[40];   
}__attribute__((packed));

int insert(DB *dbp, DB_ENV *env, struct character *ch, struct db_context *context);
int retrieve(DB *dbp, DB_ENV *env, int key, struct character *ch, struct db_context *context);
int delete(DB *dbp, DB_ENV *env, int key, struct db_context *context);
int update(DB *dbp, DB_ENV *env, int key, struct character *ch, struct db_context *context);

#endif
