/*Functions for reading in files, printing, and other misc tasks
 *
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

struct WALheader{
    uint32_t signature;
    uint32_t version;
    uint32_t page_size;
    uint32_t sequence;
    uint32_t salt1;
    uint32_t salt2;
    uint32_t checksum1;
    uint32_t checksum2;
}__attribute__((packed));

struct frame_header{
    uint32_t page_number;
    uint32_t size_in_pages;
    uint32_t salt1;
    uint32_t salt2;
    uint32_t checksum1;
    uint32_t checksum2;
}__attribute__((packed));

struct frame{
    struct frame_header frame_head;
    void *page_contents;
};

struct wal_file{
    struct WALheader *file_head;
    struct frame **frames;
};

//perform a hex dump
void hex_dump(char *label, void *addr, int len);

//read the WAL file into memory
struct wal_file* read_wal(char* filename);

#endif
