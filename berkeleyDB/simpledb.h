#ifndef SIMPLEDB_H
#define SIMPLEDB_H

#include <db.h>

struct db_context{
    DB *dbp;
    DB_ENV *env;
    DB_LSN **lsn_array;
    int number_lsn;
    int current_id;
    int randint[1024];
    int rand_index;    
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
    uint32_t time;
    uint32_t XID;
    uint32_t type;
    uint32_t key;
    uint32_t offset;
    struct character before;
    struct character after;   
}__attribute__((packed));

int insert(DB *dbp, DB_ENV *env, DBT *key, DBT *data);
int retrieve(DB *dbp, DB_ENV *env, DBT *key, DBT *data);
int delete(DB *dbp, DB_ENV *env, DBT *key);
int update(DB *dbp, DB_ENV *env, DBT *key, DBT *data);

#endif
