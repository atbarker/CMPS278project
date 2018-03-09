#ifndef ROLLBACK_H
#define ROLLBACK_H

#include "utils.h"

void* rollback_linear(struct wal_file *log);

void* rollback_parallel(struct wal_file *log, int time_quanta, int number_partitions);

#endif
