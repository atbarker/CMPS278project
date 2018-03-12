#ifndef ROLLBACK_H
#define ROLLBACK_H

#include "utils.h"

void* rollback_linear();

void* rollback_parallel(int number_records, int time_quanta, int number_partitions);

#endif
