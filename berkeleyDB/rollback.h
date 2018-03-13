#ifndef ROLLBACK_H
#define ROLLBACK_H

#include "utils.h"
#include <db.h>

void* rollback_linear(DB_LSN lsn, DB *dbp, DB_ENV *env);

void* rollback_parallel(int number_records, int time_quanta, int number_partitions, int rollback_lsn);

#endif
