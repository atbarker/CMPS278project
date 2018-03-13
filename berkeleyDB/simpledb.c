#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <sys/types.h>
#include "simpledb.h"

int insert(DB *dbp, DB_ENV *env, struct character *ch, struct db_context *context){
    int ret;
    DB_LSN *lsn = malloc(sizeof(DB_LSN));
    struct db_log_record *record = malloc(sizeof(struct db_log_record));;
    DBT log_data;
    DBT key, data;

    memset(&log_data, 0, sizeof(DBT));
    memset(&data, 0, sizeof(DBT));
    memset(&key, 0, sizeof(DBT));    

    key.data = &context->next_available_id;
    key.size = sizeof(int);
    data.data = ch;
    data.size = sizeof(struct character);
    
    memset(&log_data, 0, sizeof(DBT));

    if((ret = dbp->put(dbp, NULL, &key, &data, 0)) != 0){
       fprintf(stderr, "Record insert failed\n");
       dbp->err(dbp, ret, "DB->put");
       return -1;
    }

    record->time = time(NULL);
    record->XID = 0;
    record->type = 0;
    record->key = context->next_available_id;
    record->offset = 0;
    record->before = NULL;
    record->after = ch;
    
    log_data.data = record;
    log_data.size = sizeof(struct db_log_record);
    ret = env->log_put(env, lsn, &log_data, 0);
    if(ret){
        fprintf(stderr, "Insert log record failed\n");
        return -1;
    }

    context->next_available_id++;

    free(lsn);
    free(record);
    return 0;
}

int retrieve(DB *dbp, DB_ENV *env, int key, struct character *ch, struct db_context *context){
    int ret;
    DB_LSN *lsn = malloc(sizeof(DB_LSN));
    struct db_log_record *record = malloc(sizeof(struct db_log_record));;
    DBT log_data;

    DBT keyt, data;

    memset(&log_data, 0, sizeof(DBT));
    memset(&data, 0, sizeof(DBT));
    memset(&keyt, 0, sizeof(DBT));    

    keyt.data = &key;
    keyt.size = sizeof(int);
    
    memset(&log_data, 0, sizeof(DBT));

    if((ret = dbp->get(dbp, NULL, &keyt, &data, 0)) != 0){
        fprintf(stderr, "Record retrieve failed\n");
        dbp->err(dbp, ret, "DB->get");
    }

    record->time = time(NULL);
    record->XID = 0;
    record->type = 1;
    record->key = key;
    record->offset = 0;
    record->before = NULL;
    record->after = NULL;

    log_data.data = record;
    log_data.size = sizeof(struct db_log_record);
    ret = env->log_put(env, lsn, &log_data, 0);
    if(ret){
        fprintf(stderr, "Insert log record failed\n");
        return -1;
    }    
    free(lsn);
    free(record);
    return 0;
}

int delete(DB *dbp, DB_ENV *env, int key, struct db_context *context){
    int ret;
    DB_LSN *lsn = malloc(sizeof(DB_LSN));
    struct db_log_record *record = malloc(sizeof(struct db_log_record));;
    DBT log_data;

    DBT keyt;

    memset(&log_data, 0, sizeof(DBT));
    memset(&keyt, 0, sizeof(DBT));    

    keyt.data = &key;
    keyt.size = sizeof(int);

    if((ret = dbp->del(dbp, NULL, &keyt, 0)) != 0){
       fprintf(stderr, "Record delete failed\n");
       dbp->err(dbp, ret, "DB->del");
       return -1;
    }

    record->time = time(NULL);
    record->XID = 0;
    record->type = 2;
    record->key = key;
    record->offset = 0;
    record->before = NULL;
    record->after = NULL;

    log_data.data = record;
    log_data.size = sizeof(struct db_log_record);
    ret = env->log_put(env, lsn, &log_data, 0);
    if(ret){
        fprintf(stderr, "Insert log record failed\n");
        return -1;
    }    
    free(lsn);
    free(record);
    return 0;
}

int update(DB *dbp, DB_ENV *env, int key, struct character *ch, struct db_context *context){
    int ret;
    DB_LSN *lsn = malloc(sizeof(DB_LSN));
    struct db_log_record *record = malloc(sizeof(struct db_log_record));;
    DBT log_data;
    DBT keyt, data;

    memset(&log_data, 0, sizeof(DBT));
    memset(&data, 0, sizeof(DBT));
    memset(&keyt, 0, sizeof(DBT));    

    keyt.data = &key;
    keyt.size = sizeof(int);
    data.data = ch;
    data.size = sizeof(struct character);
    
    memset(&log_data, 0, sizeof(DBT));

    if((ret = dbp->put(dbp, NULL, &keyt, &data, 0)) != 0){
       fprintf(stderr, "Record insert failed\n");
       dbp->err(dbp, ret, "DB->put");
       return -1;
    }

    record->time = time(NULL);
    record->XID = 0;
    record->type = 0;
    record->key = key;
    record->offset = 0;
    record->before = NULL;
    record->after = ch;
    
    log_data.data = record;
    log_data.size = sizeof(struct db_log_record);
    ret = env->log_put(env, lsn, &log_data, 0);
    if(ret){
        fprintf(stderr, "Insert log record failed\n");
        return -1;
    }

    context->next_available_id++;

    free(lsn);
    free(record);
    return 0;
}
