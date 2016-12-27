#ifndef BASIC_TO_ASCII_H_INCLUDED
#define BASIC_TO_ASCII_H_INCLUDED
#include "common_functions.h"
#include "memory_extractor.h"

char** line_buffer; //array of strings, where each string is a single instruction in basic

char** build_line_buffer(char* block);

#endif // BASIC_TO_ASCII_H_INCLUDED
