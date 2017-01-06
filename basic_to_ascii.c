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
          fprintf(output, "GBP"); //Spectrum uses this character for GBP sign
          //a hack way to print UTF-8 version of this character
          //fprintf(output, "\xC2\xA3");
        }
        else
        {
          fprintf(output, "%c", block[j]);
        }
      }
      else if (block[j] == 127)
      {
        fprintf(output, "(c)"); //copyright sign is missing in the ASCII table
        //a hack way to insert UTF-8 representation of the symbol, may not work in terminals
        //fprintf(output, "\xC2");
        //fprintf(output, "\xA9");
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
