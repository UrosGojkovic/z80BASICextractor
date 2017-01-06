#include "common_functions.h"
#include "basic_to_ascii.h"
#include "memory_extractor.h"

int main(int argc, char** argv)
{
  if (argc > 1)
  {
    open_file(argv[1]);
    check_header();
    extract_pages();
    extract_basic(memory);
  }
  else
  {
    printf("usage: Z80extractor [file name or path]\n");
  }
  return 0;
}
