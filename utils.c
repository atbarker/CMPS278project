//utils.c
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
//because SQLite programmers are smoking something
#include <endian.h>

#define HEADER_SIZE 32
#define FRAME_SIZE 24


//SQLite's WAL header for each file
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

struct frame{
    uint32_t page_number;
    uint32_t size_in_pages;
    uint32_t salt1;
    uint32_t salt2;
    uint32_t checksum1;
    uint32_t checksum2;
}__attribute__((packed));

int readPage(int filedesc, struct WALheader *head, struct frame *frme){
    void *page_contents;
    uint32_t *big = malloc(FRAME_SIZE);

    if(!read(filedesc, big, FRAME_SIZE)){
        return -1;
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
        return -1;
    }

    close(filedesc);
    return 0;
}

int main(){

    struct WALheader *head = malloc(HEADER_SIZE);
    struct frame *frame1 = malloc(FRAME_SIZE);
    uint32_t *bigEndian = malloc(HEADER_SIZE);

    int filedesc = open("walbackup", O_RDONLY);
    if(!filedesc){
	printf("danger will robinson, danger\n");
        return -1;
    }

    if(!read(filedesc, bigEndian, HEADER_SIZE)){
	printf("danger will robinson, danger read\n");
        return -1;
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

    //printf("Signature: %x\n", head->signature);
    printf("Version: %x\n", head->version);
    printf("page_size: %x\n", head->page_size);
    printf("sequence: %x\n", head->sequence);
    printf("salt1: %x\n", head->salt1);
    printf("salt2: %x\n", head->salt2);
    printf("checksum1: %x\n", head->checksum1);
    printf("checksum1: %x\n", head->checksum2);
    
    free(bigEndian);
    readPage(filedesc, head, frame1);
    return 0;
}
