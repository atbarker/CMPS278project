#ifndef ROLLBACK_H
#define ROLLBACK_H

#include "utils.h"

void* rollback_linear(int rollback_lsn);

void* rollback_parallel(int number_records, int time_quanta, int number_partitions, int rollback_lsn);

#endif
