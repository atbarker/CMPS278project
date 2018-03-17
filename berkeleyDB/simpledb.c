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

    int new_key = bitmap_get_next(context->id_bitmap, context->bitmap_size); 

    if(new_key == -1){

    }

    key.data = &new_key;
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
//    record->offset = 0;
    record->data_length = sizeof(struct character);
    memcpy(&record->data, ch, 40);
    
    log_data.data = record;
    log_data.size = sizeof(struct db_log_record);
    ret = env->log_put(env, lsn, &log_data, 0);
    if(ret){
        fprintf(stderr, "Insert log record failed\n");
        return -1;
    }
    
    context->recent_lsn = *lsn;
    context->number_lsn = lsn->offset;
    context->next_available_id++;
    context->number_keys++;
    bitmap_set(context->id_bitmap, new_key, context->bitmap_size, 1);

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
    
    if((ret = dbp->exists(dbp, NULL, &keyt, 0)) == DB_NOTFOUND){
        fprintf(stderr, "Key does not exist\n");
        return -1;
    }

    if((ret = dbp->get(dbp, NULL, &keyt, &data, 0)) != 0){
        fprintf(stderr, "Record retrieve failed\n");
        dbp->err(dbp, ret, "DB->get");
    }

    record->time = time(NULL);
    record->XID = 0;
    record->type = 1;
    record->key = key;
    //record->offset = 0;
    record->data_length = sizeof(struct character);

    log_data.data = record;
    log_data.size = sizeof(struct db_log_record);
    ret = env->log_put(env, lsn, &log_data, 0);
    if(ret){
        fprintf(stderr, "Insert log record failed\n");
        return -1;
    }

    context->recent_lsn = *lsn;
    context->number_lsn = lsn->offset; 
    
    free(lsn);
    free(record);
    return 0;
}

int delete(DB *dbp, DB_ENV *env, int key, struct db_context *context){
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
    data.size = sizeof(struct character);

    if((ret = dbp->exists(dbp, NULL, &keyt, 0)) == DB_NOTFOUND){
        fprintf(stderr, "Key does not exist\n");
        return -1;
    }

    if((ret = dbp->get(dbp, NULL, &keyt, &data, 0)) != 0){
        fprintf(stderr, "Record retrieve for delete log failed.\n");
        dbp->err(dbp, ret, "DB->del");
        return -1;
    }

    if((ret = dbp->del(dbp, NULL, &keyt, 0)) != 0){
       fprintf(stderr, "Record delete failed\n");
       dbp->err(dbp, ret, "DB->del");
       return -1;
    }
   
    //have to retrieve record before delete
    record->time = time(NULL);
    record->type = 2;
    record->key = key;
    //record->offset = 0;
    record->data_length = sizeof(struct character);
    memcpy(&record->data, &data.data, sizeof(struct character));

    log_data.data = record;
    log_data.size = sizeof(struct db_log_record);
    ret = env->log_put(env, lsn, &log_data, 0);
    if(ret){
        fprintf(stderr, "Insert log record failed\n");
        return -1;
    }
    
    context->recent_lsn = *lsn;
    context->number_lsn = lsn->offset;
    context->number_keys--;
    bitmap_set(context->id_bitmap, key, context->bitmap_size, 0);
    
    free(lsn);
    free(record);
    return 0;
}

int update(DB *dbp, DB_ENV *env, int key, struct character *ch, struct db_context *context){
    int ret;
    DB_LSN *lsn = malloc(sizeof(DB_LSN));
    struct db_log_record *record = malloc(sizeof(struct db_log_record));;
    DBT log_data;
    DBT keyt, data, get_data;

    memset(&log_data, 0, sizeof(DBT));
    memset(&data, 0, sizeof(DBT));
    memset(&keyt, 0, sizeof(DBT));
    memset(&get_data, 0, sizeof(DBT));    

    keyt.data = &key;
    keyt.size = sizeof(int);
    data.data = ch;
    data.size = sizeof(struct character);
    get_data.size = sizeof(struct character);
    get_data.flags = DB_DBT_MALLOC;
    
    memset(&log_data, 0, sizeof(DBT));

    if((ret = dbp->exists(dbp, NULL, &keyt, 0)) == DB_NOTFOUND){
        fprintf(stderr, "Key does not exist %d \n", key);
        free(lsn);
        free(record);
        return -1;
    }

    if((ret = dbp->get(dbp, NULL, &keyt, &get_data, 0)) != 0){
        fprintf(stderr, "Record retrieve for update log failed.\n");
        dbp->err(dbp, ret, "DB->del");
        free(lsn);
        free(record);
        free(get_data.data);
        return -1;
    }

    if((ret = dbp->put(dbp, NULL, &keyt, &data, 0)) != 0){
       fprintf(stderr, "Record insert failed\n");
       dbp->err(dbp, ret, "DB->put");
       return -1;
    }

    record->time = time(NULL);
    record->XID = 0;
    record->type = 0;
    record->key = key;
    //record->offset = 0;
    record->data_length = sizeof(struct character);
    memcpy(&record->data, &get_data.data, sizeof(struct character));
    
    log_data.data = record;
    log_data.size = sizeof(struct db_log_record);
    ret = env->log_put(env, lsn, &log_data, 0);
    if(ret){
        fprintf(stderr, "Insert log record failed\n");
        return -1;
    }

    context->recent_lsn = *lsn;
    context->number_lsn = lsn->offset;
    context->next_available_id++;

    free(lsn);
    free(record);
    free(get_data.data);
    return 0;
}
