#ifndef COMMON_FUNCTIONS_H_INCLUDED
#define COMMON_FUNCTIONS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#ifndef TRUE
#define TRUE 1
#endif // TRUE

#ifndef FALSE
#define FALSE 0
#endif // FALSE

#define PAGE_SIZE 16384
#define PROG_VAR_ADDR 0x1C53
#define VARS_VAR_ADDR 0x1C4B //BASIC program block ends with VARS (BASIC variables)
#define E_LINE_VAR_ADDR 0x1C59 //VARS block ends with E_LINE (editing line) prefixed by 0x80
#define MEM_OFF 0x4000
#define BYTE_LEN 0x100

void fatal_error(char* message);

#endif // COMMON_FUNCTIONS_H_INCLUDED
