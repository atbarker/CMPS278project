/*Functions for reading in files, printing, and other misc tasks
 *
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#define TRUE 1
#define FALSE 0

//get and set instructions for a basic bitmap, bitmap is in the form of a byte array
int bitmap_get(unsigned char *bitmap, int bitnum, int len);
//this one returns the index of the newly selected bit
int bitmap_get_rand(unsigned char *bitmap, int len);
int bitmap_set(unsigned char *bitmap, int bitnum, int len, int bit);

//perform a hex dump
void hex_dump(char *label, void *addr, int len);

//xor two byte arrays
void array_xor(unsigned char *in1, unsigned char *in2, unsigned char *out, int len);

#endif
