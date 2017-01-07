#include "common_functions.h"
#include "basic_to_ascii.h"
#include "memory_extractor.h"

int cols;
FILE* output;
char* path;

void process_command_arguments(int argc, char** argv)
{
  int i;
  int output_file_index = cols = 0;
  for (i = 1; i < argc; i++)
  {
    if (strcmp("-c", argv[i]) == 0)
    {
      cols = atoi(argv[++i]);
    }
    else if (strcmp("-o", argv[i]) == 0)
    {
      if (output_file_index != -1)
      {
        output_file_index = i + 1;
      }
      i++;
    }
    else if (strcmp("-stdout", argv[i]) == 0)
    {
      output = stdout;
      output_file_index = -1;
    }
    else
    {
      open_file(argv[i]);
      path = strdup(argv[i]);
    }
  }
  if (output_file_index > 0)
  {
    output = fopen(argv[output_file_index], "w");
  }
  else if (output_file_index == 0)
  {
    //replace "z80" in the path with "txt"
    char* extension_begin = strrchr(path, '.');
    int extension_begin_index = extension_begin - path;
    path[extension_begin_index + 1]='t';
    path[extension_begin_index + 2]='x';
    path[extension_begin_index + 3]='t';
    output = fopen(path, "w");
    if (output == NULL)
    {
      fatal_error("Can't open file for output:");
    }
  }

}

int main(int argc, char** argv)
{
  if (argc > 1)
  {
    process_command_arguments(argc, argv);
    check_header();
    extract_pages();
    extract_basic(memory, output, cols);
    if (output != stdout)
    {
      fclose(output);
    }
  }
  else
  {
    printf("z80BASICextractor v1.0beta4\n\n");
    printf("usage: z80extractor [file name or path] [-o output_file] [-stdout]\n");
    //printf("-c\twrap lines to COLS number of characters, default: no wrapping\n");
    printf("-o\toutput to the specified file (it will be overwritten if exists!), default: output file has the same name as input with extension changed to .txt\n");
    printf("-stdout\toutput to screen instead to a file\n");
  }
  return 0;
}
