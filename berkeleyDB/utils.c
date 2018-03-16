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

int bitmap_get(unsigned char *bitmap, int bitnum, int len){
    int byte = bitnum >> 3;
    int n = sizeof(bitnum)*8-3;
    int offset = ((unsigned) bitnum << n) >> n;
    if(bitmap[byte] & (1 << (7-offset))){
        return 1;
    }else{
        return 0;
    }
}

int bitmap_set(unsigned char *bitmap, int bitnum, int len, int bit){
    int byte = bitnum >> 3;
    int n = sizeof(bitnum)*8-3;
    int offset = ((unsigned) bitnum << n) >> n;
    if(bit){
        bitmap[byte] |= 1 << (7-offset);
    }else{
        bitmap[byte] &= ~(1 << (7-offset));
    }
    return 0;
}

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

void array_xor(unsigned char *in1, unsigned char *in2, unsigned char *out, int len){
    int i;
    for(i = 0; i < len; i++){
        out[i] = in1[i] ^ in2[i];
    }
}
