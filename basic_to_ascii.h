#ifndef BASIC_TO_ASCII_H_INCLUDED
#define BASIC_TO_ASCII_H_INCLUDED
#include "common_functions.h"
#include "memory_extractor.h"

void extract_basic (unsigned char* block, FILE* output, int cols);
void extract_basic_variables (unsigned char* block, FILE* output, int cols);
double convert_number(unsigned char number[5]);

#endif // BASIC_TO_ASCII_H_INCLUDED
