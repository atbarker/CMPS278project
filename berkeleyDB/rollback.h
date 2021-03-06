#ifndef ROLLBACK_H
#define ROLLBACK_H

#include "utils.h"
#include "simpledb.h"
#include <db.h>

struct rollback_summary{
    DB_LSN target;
    uint32_t diffs_length;
    unsigned char *changed;
    unsigned char **diffs;
};

int rollback_destruct(struct rollback_summary *sum);

struct rollback_summary* rollback_linear(int trans, DB *dbp, DB_ENV *env, struct db_context *context);

struct rollback_summary* rollback_parallel(int number_records, int time_quanta, int number_partitions, int rollback_lsn, DB *dbp, DB_ENV *env, DB_LOGC *cursor, struct db_context *context, pthread_mutex_t *rollback_lock);

#endif
