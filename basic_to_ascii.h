#ifndef BASIC_TO_ASCII_H_INCLUDED
#define BASIC_TO_ASCII_H_INCLUDED
#include "common_functions.h"
#include "memory_extractor.h"

int array_index; //used when traversing arrays
char buffer[50]; //output buffer for printing numbers;

void extract_basic (unsigned char* block, FILE* output, int cols);
void extract_basic_variables (unsigned char* block, FILE* output, int cols);
char* convert_number (unsigned char number[5]);
int iterate_over_array (unsigned char* start, int element_length, FILE* output); //returns the number of bytes read
void iterate (int current_dimension, int max_dimension, int* dimension_size, int* current_coords, char letter, int element_size, unsigned char* start, FILE* output);

#endif // BASIC_TO_ASCII_H_INCLUDED
