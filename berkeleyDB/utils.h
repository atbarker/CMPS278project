/*Functions for reading in files, printing, and other misc tasks
 *
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#define TRUE 1
#define FALSE 0

//perform a hex dump
void hex_dump(char *label, void *addr, int len);

#endif
