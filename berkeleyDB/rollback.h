#ifndef ROLLBACK_H
#define ROLLBACK_H

#include "utils.h"
#include "simpledb.h"
#include <db.h>

struct rollback_summary{
    DB_LSN target;
    struct character **diffs;
};

struct rollback_summary* rollback_linear(DB_LSN lsn, DB *dbp, DB_ENV *env, struct db_context *context);

struct rollback_summary* rollback_parallel(int number_records, int time_quanta, int number_partitions, int rollback_lsn, struct db_context *context);

#endif
