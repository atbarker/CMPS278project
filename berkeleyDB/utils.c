//utils.c
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
//because SQLite programmers are smoking something
#include <sys/stat.h>

#include "utils.h"

void hex_dump(char *label, void *addr, int len){
    int i;
    unsigned char buff[17];
    unsigned char *pc = (unsigned char*)addr;

    if(label != NULL){
        printf("%s:\n", label);
    }
    if(len == 0){
        printf("no length specified\n");
	return;
    }
    if(len < 0){
        printf("negative length\n");
	return;
    }

    for(i = 0; i < len; i++){
        if((i%16) == 0){
            if(i != 0){
                printf(" %s\n", buff);
            }
            printf(" %04x ", i);
        }
        printf(" %02x", pc[i]);
 
        if((pc[i] < 0x20) || (pc[i] > 0x7e)){
            buff[i % 16] = '.';
        }else{
            buff[i % 16] = pc[i];
        }
        buff[(i % 16) + 1] = "\0";
    }
    while ((i%16) != 0){
        printf("  ");
        i++;
    }
    printf(" %s\n", buff);
}

void* read_page(int filedesc, struct WALheader *head, struct frame_header *frme){
    void *page_contents;
    uint32_t *big = malloc(FRAME_SIZE);

    printf("Current Offset: %lu bytes\n", lseek(filedesc, 0, SEEK_CUR));

    if(!read(filedesc, big, FRAME_SIZE)){
        return NULL;
    }

    frme->page_number = be32toh(big[0]);
    frme->size_in_pages = be32toh(big[1]);
    frme->salt1 = be32toh(big[2]);
    frme->salt2 = be32toh(big[3]);
    frme->checksum1 = be32toh(big[4]);
    frme->checksum2 = be32toh(big[5]);    

    printf("Page number: %x\n", frme->page_number);
    printf("Size in pages: %x\n", frme->size_in_pages);
    printf("Salt1: %x\n", frme->salt1);
    printf("Salt2: %x\n", frme->salt2);
    printf("Checksum1: %x\n", frme->checksum1);
    printf("Checksum2: %x\n", frme->checksum2);

    page_contents = malloc(head->page_size);

    if(!read(filedesc,page_contents, head->page_size)){
        return NULL;
    }

    hex_dump("page 1", page_contents, head->page_size);

    return page_contents;
}

struct wal_file* read_wal(char* filename){

    struct WALheader *head = malloc(HEADER_SIZE);
    struct frame_header *frame1 = malloc(FRAME_SIZE);
    struct wal_file *file = malloc(sizeof(struct wal_file));
    uint32_t *bigEndian = malloc(HEADER_SIZE);

    int filedesc = open(filename, O_RDONLY);

    
    if(!filedesc){
	printf("danger will robinson, danger\n");
        return NULL;
    }

    if(!read(filedesc, bigEndian, HEADER_SIZE)){
	printf("danger will robinson, danger read\n");
        return NULL;
    }
    //to host endianness
    head->signature = be32toh(bigEndian[0]);
    head->version = be32toh(bigEndian[1]);
    head->page_size = be32toh(bigEndian[2]);
    head->sequence = be32toh(bigEndian[3]);
    head->salt1 = be32toh(bigEndian[4]);
    head->salt2 = be32toh(bigEndian[5]);
    head->checksum1 = be32toh(bigEndian[6]);
    head->checksum2 = be32toh(bigEndian[7]);

    file->file_head = head;
    file->number_pages = wal_size(filename, head->page_size);

    //printf("Signature: %x\n", head->signature);
  
}
