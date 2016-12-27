#include "common_functions.h"

void fatal_error(char* message)
{
  perror(message);
  exit(EXIT_FAILURE);
}
