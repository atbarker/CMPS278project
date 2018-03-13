/*Functions for reading in files, printing, and other misc tasks
 *
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#define TRUE 1
#define FALSE 0

struct character{
    int id;
    char *name;
    char *class;
    int hp;
    int alive;
    int lvl;
}__attribute__((packed));

struct db_log_record{
    uint32_t time;
    uint32_t XID;
    uint32_t type;
    uint32_t key;
    uint32_t offset;
    struct character before;
    struct character after;   
}__attribute__((packed));

//perform a hex dump
void hex_dump(char *label, void *addr, int len);

#endif
