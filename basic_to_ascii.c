#include"basic_to_ascii.h"

void extract_basic(unsigned char* block, FILE* output, int cols)
{
  int prog_start_addr = block[PROG_VAR_ADDR]; //lower byte of the address of BASIC program
  prog_start_addr += block[PROG_VAR_ADDR + 1] * BYTE_LEN; //higher byte of the address of BASIC program
  prog_start_addr -= MEM_OFF; //add the (negative) offset of block compared to the real memory layout

  int prog_end_addr = block[VARS_VAR_ADDR]; //lower byte of the address of BASIC variables (end of program)
  prog_end_addr += block[VARS_VAR_ADDR + 1] * BYTE_LEN; //higher byte
  prog_end_addr -= MEM_OFF; //same as above

  //printf("BASIC program begins at: %d, and ends at: %d. (in local memory space)\n", prog_start_addr, prog_end_addr);

  int i, j, line_number;// line_length; // current_line_length;
  for(i = prog_start_addr; i < prog_end_addr; i = j + 1) //add 1 byte for \r we stopped at
  {
    //current_line_length = 0;
    line_number = block[i] * BYTE_LEN;
    line_number += block[i + 1];
    fprintf(output, "%4d", line_number);
    //line_length = block[i + 2];
    //line_length += block[i+3] * BYTE_LEN;
    /*
    line length is not needed because we use \r as line terminator, which is apparently more resistant to errors (for example: wrong line length value in BASIC memory) and more in line with how LIST and LLIST command display the program
    */
    //fprintf(output, "\nline begin: %d\n", i);
    //fprintf(output, "line length: %d\n", line_length);
    for (j = i + 4; block[j] != '\r'; j++)
    {
      if (block[j] == 14)
      {
        j += 5; //character value 14 marks the begining of number constant value after the constant's string representation; we don't need this so we just skip the next 5 bytes
      }
      else if (block[j] >= 32 && block[j] <= 126)
      {
        if (block[j] == '`')
        {
          //fprintf(output, "GBP"); //Spectrum uses this character for GBP sign
          //a hack way to print UTF-8 version of this character
          fprintf(output, "\xC2\xA3");
        }
        else
        {
          fprintf(output, "%c", block[j]);
        }
      }
      else if (block[j] == 127)
      {
        //fprintf(output, "(c)"); //copyright sign is missing in the ASCII table
        //a hack way to insert UTF-8 representation of the symbol, may not work in terminals
        fprintf(output, "\xC2\xA9");
      }
      else if (block[j] >= 165)
      {
        if (block[j] <= 196)
        {
          switch(block[j]) //these keywords only have space to the right of them
          {
            case 165: fprintf(output, "RND "); break;
            case 166: fprintf(output, "INKEY$ "); break;
            case 167: fprintf(output, "PI "); break;
            case 168: fprintf(output, "FN "); break;
            case 169: fprintf(output, "POINT "); break;
            case 170: fprintf(output, "SCREEN$ "); break;
            case 171: fprintf(output, "ATTR "); break;
            case 172: fprintf(output, "AT "); break;
            case 173: fprintf(output, "TAB "); break;
            case 174: fprintf(output, "VAL$ "); break;
            case 175: fprintf(output, "CODE "); break;
            case 176: fprintf(output, "VAL "); break;
            case 177: fprintf(output, "LEN "); break;
            case 178: fprintf(output, "SIN "); break;
            case 179: fprintf(output, "COS "); break;
            case 180: fprintf(output, "TAN "); break;
            case 181: fprintf(output, "ASN "); break;
            case 182: fprintf(output, "ACS "); break;
            case 183: fprintf(output, "ATN "); break;
            case 184: fprintf(output, "LN "); break;
            case 185: fprintf(output, "EXP "); break;
            case 186: fprintf(output, "INT "); break;
            case 187: fprintf(output, "SOR "); break;
            case 188: fprintf(output, "SGN "); break;
            case 189: fprintf(output, "ABS "); break;
            case 190: fprintf(output, "PEEK "); break;
            case 191: fprintf(output, "IN "); break;
            case 192: fprintf(output, "USR "); break;
            case 193: fprintf(output, "STR$ "); break;
            case 194: fprintf(output, "CHR$ "); break;
            case 195: fprintf(output, "NOT "); break;
            case 196: fprintf(output, "BIN "); break;
            default: break;
          }
        }
        else
        {
          //these keywords add extra space before them if there isn't one already present
          if (block[j-1] != ' ' && block[j-1] < 165 && (block[j] < 199 || block[j] > 201)) //if the previous symbol wasn't a space or one of the keywords (other than <=, >=, <>), add space before the keyword
          {
            fprintf(output, " ");
          }
          switch(block[j])
          {
            case 197: fprintf(output, "OR "); break;
            case 198: fprintf(output, "AND "); break;
            case 199: fprintf(output, "<="); break; //take note: no space after <=, >=, <>
            case 200: fprintf(output, ">="); break;
            case 201: fprintf(output, "<>"); break;
            case 202: fprintf(output, "LINE "); break;
            case 203: fprintf(output, "THEN "); break;
            case 204: fprintf(output, "TO "); break;
            case 205: fprintf(output, "STEP "); break;
            case 206: fprintf(output, "DEF FN "); break;
            case 207: fprintf(output, "CAT "); break;
            case 208: fprintf(output, "FORMAT "); break;
            case 209: fprintf(output, "MOVE "); break;
            case 210: fprintf(output, "ERASE "); break;
            case 211: fprintf(output, "OPEN# "); break;
            case 212: fprintf(output, "CLOSE# "); break;
            case 213: fprintf(output, "MERGE "); break;
            case 214: fprintf(output, "VERIFY "); break;
            case 215: fprintf(output, "BEEP "); break;
            case 216: fprintf(output, "CIRCLE "); break;
            case 217: fprintf(output, "INK "); break;
            case 218: fprintf(output, "PAPER "); break;
            case 219: fprintf(output, "FLASH "); break;
            case 220: fprintf(output, "BRIGHT "); break;
            case 221: fprintf(output, "INVERSE "); break;
            case 222: fprintf(output, "OVER "); break;
            case 223: fprintf(output, "OUT "); break;
            case 224: fprintf(output, "LPRINT "); break;
            case 225: fprintf(output, "LLIST "); break;
            case 226: fprintf(output, "STOP "); break;
            case 227: fprintf(output, "READ "); break;
            case 228: fprintf(output, "DATA "); break;
            case 229: fprintf(output, "RESTORE "); break;
            case 230: fprintf(output, "NEW "); break;
            case 231: fprintf(output, "BORDER "); break;
            case 232: fprintf(output, "CONTINUE "); break;
            case 233: fprintf(output, "DIM "); break;
            case 234: fprintf(output, "REM "); break;
            case 235: fprintf(output, "FOR "); break;
            case 236: fprintf(output, "GO TO "); break;
            case 237: fprintf(output, "GO SUB "); break;
            case 238: fprintf(output, "INPUT "); break;
            case 239: fprintf(output, "LOAD "); break;
            case 240: fprintf(output, "LIST "); break;
            case 241: fprintf(output, "LET "); break;
            case 242: fprintf(output, "PAUSE "); break;
            case 243: fprintf(output, "NEXT "); break;
            case 244: fprintf(output, "POKE "); break;
            case 245: fprintf(output, "PRINT "); break;
            case 246: fprintf(output, "PLOT "); break;
            case 247: fprintf(output, "RUN "); break;
            case 248: fprintf(output, "SAVE "); break;
            case 249: fprintf(output, "RANDOMIZE "); break;
            case 250: fprintf(output, "IF "); break;
            case 251: fprintf(output, "CLS "); break;
            case 252: fprintf(output, "DRAW "); break;
            case 253: fprintf(output, "CLEAR "); break;
            case 254: fprintf(output, "RETURN "); break;
            case 255: fprintf(output, "COPY "); break;
            default: break;
          }
        }
      }
    }
    //fprintf(output,"\ncharacters read: %d, line len %d\n", j-i, line_length);
    fprintf(output, "\n");
  }
}

void extract_basic_variables(unsigned char* block, FILE* output, int cols)
{
  int vars_begin_addr = block[VARS_VAR_ADDR];
  vars_begin_addr += block[VARS_VAR_ADDR + 1] * BYTE_LEN; //higher byte
  vars_begin_addr -= MEM_OFF;

  int vars_end_addr = block[E_LINE_VAR_ADDR];
  vars_end_addr += block[E_LINE_VAR_ADDR + 1] * BYTE_LEN;
  vars_end_addr -= MEM_OFF;
  int i, j;
  for (i = vars_begin_addr; i < vars_end_addr; )
  {
    unsigned char number[5];
    if((block[i] & 0xE0) == 0x80) //if the first 3 bits of the byte are 100
    {
      //it's an array of numbers
      if (block[i] != 0x80) // 0x80 value marks the end of variables
      {
        i += iterate_over_array(&block[i], 5, output);
      }
      else
      {
        i++; //we read the last byte and we should end the loop
      }
    }
    else if((block[i] & 0xE0) == 0xA0) // == 101
    {
      //it's a number with name longer than a single character
      fprintf(output, "%c", (block[i] & 0x1F) + 0x60); //first letter
      i++; //move to the next byte
      while((block[i] & 0x80) == 0) //while first bit is 0
      {
        fprintf(output, "%c", block[i]);
        i++;
      }
      fprintf(output, "%c = ", block[i] & 0x7F); //the last character of the name with the first 1 turned into 0
      i++;
      for (j = 0; j < 5; j++)
      {
        number[j] = block[i + j];
      }
      fprintf(output, "%g\n", convert_number(number));
      i += 5;
    }
    else if((block[i] & 0xE0) == 0xE0) // == 111
    {
      //it's a control variable for FOR-NEXT loop
      fprintf(output, "FOR loop %c = ", (block[i] & 0x1F) + 0x60);
      i++;
      for (j = 0; j < 5; j++)
      {
        number[j] = block[i + j];
      }
      i += 5;
      fprintf(output, "%g", convert_number(number));
      for (j = 0; j < 5; j++)
      {
        number[j] = block[i + j];
      }
      i += 5;
      fprintf(output, " to %g, ", convert_number(number));
      for (j = 0; j < 5; j++)
      {
        number[j] = block[i + j];
      }
      i += 5;
      fprintf(output, "step %g, ", convert_number(number));
      int loop_line = block[i] + block[i + 1] * BYTE_LEN;
      i += 2;
      int statement_number = block[i];
      fprintf(output, "loop back at %d:%d\n", loop_line, statement_number);
      i++;
    }
    else if((block[i] & 0xE0) == 0x40) // == 010
    {
      //it's a string
      fprintf(output, "string %c = \"", (block[i] & 0x1F) + 0x60);
      i++;
      int string_len = block[i] + block[i + 1] * BYTE_LEN;
      i += 2;
      for (j = 0; j < string_len; j++)
      {
        fprintf(output, "%c", block[i + j]);
      }
      fprintf(output, "\"\n");
      i += string_len;
    }
    else if((block[i] & 0xE0) == 0xC0) // == 110
    {
      //it's an array of characters (single byte values)
      i += iterate_over_array(&block[i], 1, output);
    }
    else
    {
      //it's none of the above, so it's probably a number variable named with a single letter
      if(block[i] > 32 && block[i] < 128) //printable ASCII, without space
      {
        //assume it's a variable name
        fprintf(output,"%c = ", block[i]);
        //the next five bytes are the value
        for (j = 0; j < 5; j++)
        {
          number[j] = block[i + 1 + j];
        }
        fprintf(output, "%g\n", convert_number(number));
        i += 6; //shift for 5 bytes of value + 1 byte of letter
      }
    }
  }
}

double convert_number(unsigned char number[5])
{
  //TODO: Burger's float to decimal conversion algorithm
  if(number[0] == 0 && (number[1] == 0 || number[1] == 0xFF) && number[4] == 0) //check if it's a 16-bit (sort of) signed integer (N-th complement), used for integers in +/- 65536 range.
  {
    //it's definitely a 16-bit integer
    int converted_value = 0;
    if(number[1] == 0) //if it's a positive
    {
      converted_value = number[2] + number[3]*BYTE_LEN;
      return (double) converted_value;
    }
    else
    {
      //it's a negative
      converted_value = ~converted_value; //turn zeros into ones (fill with ones since the starting value is all zeros)
      converted_value ^= 0xFFFF; //turn the last 2 bytes to zeroes again
      converted_value = number[2] + number[3]*BYTE_LEN;
      return (double) converted_value; //now we have the number written as 32-bit signed (N-th complement) integer
    }
  }
  else
  {
    //it's a floating point number
    double exponent = pow(2, number[0] - 128); //the real exponent
    double mantissa = 0;
    double current_position_value = 0.5;
    double sign;
    if ((number[1] & 0x80) == 0x80)
    {
      //the first bit is 1 -> negative
      sign = -1;
    }
    else
    {
      sign = 1; //it's positive
      number[1] |= 0x80; //make implicit 1 appear again (replacing the 0 as the sign) for the next step
    }
    int i, j;
    for (i = 1; i < 4; i++)
    {
      for(j = 0; j < 8; j++)
      {
        if((number[i] & 0x80) == 0x80)
        {
          mantissa += current_position_value;
        }
        number[i] <<= 1;
        current_position_value /= 2;
      }
    }
    return (sign*mantissa*exponent);
  }
}

int iterate_over_array(unsigned char* start, int element_length, FILE* output)
{
  char letter = (start[0] & 0x1F) + 0x60; //last 5 bits + offset introduced by the BASIC interpreter
  int array_size = start[1] + start[2] * BYTE_LEN;
  int number_of_dimensions = start[3];
  int* dimension_size = malloc(number_of_dimensions * sizeof(int));
  if (dimension_size == NULL)
  {
    fatal_error("iterate_over_array(): Can't allocate memory");
  }
  int* current_coords = malloc(number_of_dimensions * sizeof(int));
  if (current_coords == NULL)
  {
    fatal_error("iterate_over_array(): Can't allocate memory");
  }
  int i;
  for (i = 0; i < number_of_dimensions; i++)
  {
    dimension_size[i] = start[4 + i * 2] + start[5 + i * 2] * BYTE_LEN;
  }
  array_index = 4 + number_of_dimensions * 2;
  iterate(0, number_of_dimensions, dimension_size, current_coords, letter, element_length, start, output);
  return array_size + 3;
}

void iterate(int current_dimension, int max_dimension, int* dimension_size, int* current_coords, char letter, int element_size, unsigned char* start, FILE* output)
{
  if (current_dimension >= max_dimension)
  {
    //we reached the element and we should print the stuff we need
    fprintf(output, "%c(", letter);
    int i;
    for (i = 0; i < max_dimension; i++)
    {
      fprintf(output, "%d", current_coords[i] + 1); //on Spectrum indexing starts from 1
      if (i != max_dimension - 1)
      {
        fprintf(output, ", ");
      }
    }
    fprintf(output, ") = ");

    if (element_size == 5)
    {
      //print the numbers
      unsigned char number[5];
      for (i = 0; i < 5; i++)
      {
        number[i] = start[array_index];
        array_index++;
      }
      fprintf(output, "%g\n", convert_number(number));

    }
    if (element_size == 1)
    {
      //print characters (and their numeric values)
      fprintf(output, "%c (%d)\n", start[array_index], start[array_index]);
      array_index++;
    }
    return;
  }
  int i;
  for (i = 0; i < dimension_size[current_dimension]; i++)
  {
    current_coords[current_dimension] = i;
    iterate(current_dimension + 1, max_dimension, dimension_size, current_coords, letter, element_size, start, output);
  }

}
