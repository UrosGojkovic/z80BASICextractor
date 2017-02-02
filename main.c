#include "common_functions.h"
#include "basic_to_ascii.h"
#include "memory_extractor.h"

int cols;
FILE* output;
FILE* vars_output;

void process_command_arguments(int argc, char** argv)
{
  int i, vars_only = FALSE, list_only = FALSE;
  char* extension_begin;
  char* path_for_list;
  char* path_for_vars;
  int extension_begin_index, path_len;
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
      output_file_index = -1;
    }
    else if (strcmp("-vars", argv[i]) == 0)
    {
      vars_only = TRUE;
    }
    else if (strcmp("-list", argv[i]) == 0)
    {
      list_only = TRUE;
    }
    else
    {
      open_file(argv[i]);
      path_for_list = strdup(argv[i]);
      path_for_vars = strdup(argv[i]);
    }
  }

  if (vars_only && list_only) //if both are set, reset the exclusivity
  {
    vars_only = FALSE;
    list_only = FALSE;
  }
  //we had the name supplied with -o
  if (output_file_index > 0)
  {
    if (vars_only != TRUE)
    {
      output = fopen(argv[output_file_index], "w");
      if (output == NULL)
      {
        fatal_error("proces_command_arguments(): Can't open file for program output");
      }
    }

    if (list_only != TRUE)
    {
      path_len = strlen(argv[output_file_index]);
      path_for_vars = malloc(path_len + 6); //we remove the extension from the provided path and replace it with "-vars.txt" to mark the files with variables
      if (path_for_vars == NULL)
      {
        fatal_error("process_command_arguments(): Can't allocate memory");
      }
      path_for_vars = strcpy(path_for_vars, argv[output_file_index]); //copy the path to the enlarged buffer
      extension_begin = strrchr(path_for_vars, '.'); //get the pointer to the last . in the filename
      if (extension_begin == NULL)
      {
        path_for_vars = realloc(path_for_vars, path_len + 10); //if there is no extension, add sufix to the end of file name
        if (path_for_vars == NULL)
        {
          fatal_error("process_command_arguments(): Can't allocate memory");
        }
        extension_begin = &(path_for_vars[path_len]);
      }
      extension_begin = strcpy(extension_begin, "-vars.txt"); //overwrite the end
      vars_output = fopen(path_for_vars, "w");
      if (vars_output == NULL)
      {
        fatal_error("process_command_arguments(): Can't open file for variables output");
      }
    }
  }
  else if (output_file_index == 0)
  {
    if (vars_only != TRUE)
    {
      //replace "z80" in the path with "txt"
      extension_begin = strrchr(path_for_list, '.');
      extension_begin_index = extension_begin - path_for_list;
      path_for_list[extension_begin_index + 1] = 't';
      path_for_list[extension_begin_index + 2] = 'x';
      path_for_list[extension_begin_index + 3] = 't';
      output = fopen(path_for_list, "w");
      if (output == NULL)
      {
        fatal_error("process_command_arguments(): Can't open file for program output");
      }
    }

    if (list_only != TRUE)
    {
      path_len = strlen(path_for_vars);
      path_for_vars = realloc(path_for_vars, path_len + 6);
      if (path_for_vars == NULL)
      {
        fatal_error("process_command_arguments(): Can't allocate memory");
      }
      extension_begin = strrchr(path_for_vars, '.');
      extension_begin = strcpy(extension_begin, "-vars.txt");
      vars_output = fopen(path_for_vars, "w");
      if (vars_output == NULL)
      {
        fatal_error("process_command_arguments(): Can't open file for variables output");
      }
    }
  }
  else {
    //-stdout option
    if (vars_only != TRUE)
    {
      output = stdout;
    }
    if (list_only != TRUE)
    {
      vars_output = stdout;
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
    extract_basic_variables(memory, vars_output, cols);
    if (output != stdout)
    {
      fclose(output);
      fclose(vars_output);
    }
  }
  else
  {
    printf("z80BASICextractor v1.0beta5\n\n");
    printf("usage: z80extractor [input file name or path] [-o output_file] [-stdout] [-list] [-vars]\n");
    //printf("-c\twrap lines to COLS number of characters, default: no wrapping\n");
    printf("default: program output files have the same name as input with extension changed to '.txt' for listings and '-vars.txt' for variables.\n");
    printf("-o\toutput program to the specified file (it will be overwritten if exists!), variables will use the name before extension and add '-vars.txt' to it\n");
    printf("-stdout\toutput to screen instead to a file\n");
    printf("-list\toutput listings only\n");
    printf("-vars\toutput variables only\n");
  }
  return 0;
}
