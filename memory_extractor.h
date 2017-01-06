#ifndef MEMORY_EXTRACTOR_H_INCLUDED
#define MEMORY_EXTRACTOR_H_INCLUDED
#include "common_functions.h"

FILE* source;
int version;
char is_compressed;
int header_end;
unsigned char* memory;
enum hardware_mode_enum {forty, onetwenty, SamRam} machine_type; //hardware mode determines markers of different memory pages

int open_file(char* path); //opens the file in the appropriate mode
int check_header(); //analyzes the header of the file
int extract_pages(); //extracts page(s) based on header settings and merges them to one continuous memory block
unsigned char* decompress(long int starting_offset, long int length); //returns block of decompressed memory; if length == NULL, decompress until we hit version 1 ending block "0x00 0xED 0xED 0x00". Return value points to a newly allocated block.

#endif // MEMORY_EXTRACTOR_H_INCLUDED
