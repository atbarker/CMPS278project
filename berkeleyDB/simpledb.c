#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <sys/types.h>
#include "simpledb.h"

int insert(DB *dbp, DB_ENV *env, DBT *key, DBT *data){
    int ret;
    DB_LSN *lsn = malloc(sizeof(DB_LSN));
    struct db_log_record *record = malloc(sizeof(struct db_log_record));;
    DBT log_data;

    memset(&data, 0, sizeof(DBT));

    if((ret = dbp->put(dbp, NULL, key, data, DB_NOOVERWRITE)) != 0){
       fprintf(stderr, "Record insert failed\n");
       dbp->err(dbp, ret, "DB->put");
       return -1;
    }
    
    log_data.data = record;
    log_data.size = sizeof(struct db_log_record);
    ret = env->log_put(env, lsn, &log_data, 0);
    if(ret){
        fprintf(stderr, "Insert log record failed\n");
        return -1;
    }    
    return 0;
}

int retrieve(DB *dbp, DB_ENV *env, DBT *key, DBT *data){
    int ret;
    DB_LSN *lsn = malloc(sizeof(DB_LSN));
    struct db_log_record *record = malloc(sizeof(struct db_log_record));;
    DBT log_data;

    if((ret = dbp->get(dbp, NULL, key, data, 0)) != 0){
        fprintf(stderr, "Record retrieve failed\n");
        dbp->err(dbp, ret, "DB->get");
    }

    log_data.data = record;
    log_data.size = sizeof(struct db_log_record);
    ret = env->log_put(env, lsn, &log_data, 0);
    if(ret){
        fprintf(stderr, "Insert log record failed\n");
        return -1;
    }    
    return 0;
}

int delete(DB *dbp, DB_ENV *env, DBT *key){
    int ret;
    DB_LSN *lsn = malloc(sizeof(DB_LSN));
    struct db_log_record *record = malloc(sizeof(struct db_log_record));;
    DBT log_data;

    if((ret = dbp->del(dbp, NULL, key, 0)) != 0){
       fprintf(stderr, "Record delete failed\n");
       dbp->err(dbp, ret, "DB->del");
       return -1;
    }

    log_data.data = record;
    log_data.size = sizeof(struct db_log_record);
    ret = env->log_put(env, lsn, &log_data, 0);
    if(ret){
        fprintf(stderr, "Insert log record failed\n");
        return -1;
    }    
    return 0;
}

int update(DB *dbp, DB_ENV *env, DBT *key, DBT *data){
    int ret;
    DB_LSN *lsn = malloc(sizeof(DB_LSN));
    struct db_log_record *record = malloc(sizeof(struct db_log_record));;
    DBT log_data;

    if((ret = dbp->put(dbp, NULL, key, data, 0)) != 0){
        fprintf(stderr, "Record update failed\n");
        dbp->err(dbp, ret, "DB->put");
        return -1;
    }
   
    log_data.data = record;
    log_data.size = sizeof(struct db_log_record);
    ret = env->log_put(env, lsn, &log_data, 0);
    if(ret){
        fprintf(stderr, "Insert log record failed\n");
        return -1;
    }
    return 0;
}
