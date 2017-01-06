#include"basic_to_ascii.h"

void extract_basic(unsigned char* block)
{
  int prog_start_addr = block[PROG_VAR_ADDR]; //lower byte of the address of BASIC program
  prog_start_addr += block[PROG_VAR_ADDR + 1]*BYTE_LEN; //higher byte of the address of BASIC program
  prog_start_addr -= MEM_OFF; //add the (negative) offset of block compared to the real memory layout

  int prog_end_addr = block[VARS_VAR_ADDR]; //lower byte of the address of BASIC variables (end of program)
  prog_end_addr += block[VARS_VAR_ADDR + 1]*BYTE_LEN; //higher byte
  prog_end_addr -= MEM_OFF; //same as above

  //printf("BASIC program begins at: %d, and ends at: %d. (in local memory space)\n", prog_start_addr, prog_end_addr);

  int i, line_number, line_length;
  for(i = prog_start_addr; i < prog_end_addr; i += (line_length + 4)) //add 4 bytes for line number and line length
  {
    line_number = block[i] * BYTE_LEN; //added type casts to prevent compiler warnings
    line_number += block[i + 1];
    printf("%4d", line_number);
    line_length = block[i + 2];
    line_length += block[i+3] * BYTE_LEN;
    int j;
    for (j = i + 4; j < i + line_length + 4; j++)
    {
      if (block[j] == 14)
      {
        j += 5; //character value 14 marks the begining of number constant value after the constant's string representation; we don't need this so we just skip the next 5 bytes
      }
      else if (block[j] >= 32 && block[j] <= 126)
      {
        if (block[j] == '`')
        {
          printf("GBP"); //Spectrum uses this character for GBP sign
          //a hack way to print UTF-8 version of this character
          //printf("\xC2\xA3");
        }
        else
        {
          printf("%c", block[j]);
        }
      }
      else if (block[j] == 127)
      {
        printf("(c)"); //copyright sign is missing in the ASCII table
        //a hack way to insert UTF-8 representation of the symbol, may not work in terminals
        //printf("\xC2");
        //printf("\xA9");
      }
      else if (block[j] >= 165)
      {
        if (block[j] <= 196)
        {
          switch(block[j]) //these keywords only have space to the right of them
          {
            case 165: printf("RND "); break;
            case 166: printf("INKEY$ "); break;
            case 167: printf("PI "); break;
            case 168: printf("FN "); break;
            case 169: printf("POINT "); break;
            case 170: printf("SCREEN$ "); break;
            case 171: printf("ATTR "); break;
            case 172: printf("AT "); break;
            case 173: printf("TAB "); break;
            case 174: printf("VAL$ "); break;
            case 175: printf("CODE "); break;
            case 176: printf("VAL "); break;
            case 177: printf("LEN "); break;
            case 178: printf("SIN "); break;
            case 179: printf("COS "); break;
            case 180: printf("TAN "); break;
            case 181: printf("ASN "); break;
            case 182: printf("ACS "); break;
            case 183: printf("ATN "); break;
            case 184: printf("LN "); break;
            case 185: printf("EXP "); break;
            case 186: printf("INT "); break;
            case 187: printf("SOR "); break;
            case 188: printf("SGN "); break;
            case 189: printf("ABS "); break;
            case 190: printf("PEEK "); break;
            case 191: printf("IN "); break;
            case 192: printf("USR "); break;
            case 193: printf("STR$ "); break;
            case 194: printf("CHR$ "); break;
            case 195: printf("NOT "); break;
            case 196: printf("BIN "); break;
            default: break;
          }
        }
        else
        {
          //these keywords add extra space before them if there isn't one already present
          if (block[j-1] != ' ' && block[j-1] < 165 && (block[j] < 199 || block[j] > 201)) //if the previous symbol wasn't a space or one of the keywords (other than <=, >=, <>), add space before the keyword
          {
            printf(" ");
          }
          switch(block[j])
          {
            case 197: printf("OR "); break;
            case 198: printf("AND "); break;
            case 199: printf("<="); break; //take note: no space after <=, >=, <>
            case 200: printf(">="); break;
            case 201: printf("<>"); break;
            case 202: printf("LINE "); break;
            case 203: printf("THEN "); break;
            case 204: printf("TO "); break;
            case 205: printf("STEP "); break;
            case 206: printf("DEF FN "); break;
            case 207: printf("CAT "); break;
            case 208: printf("FORMAT "); break;
            case 209: printf("MOVE "); break;
            case 210: printf("ERASE "); break;
            case 211: printf("OPEN# "); break;
            case 212: printf("CLOSE# "); break;
            case 213: printf("MERGE "); break;
            case 214: printf("VERIFY "); break;
            case 215: printf("BEEP "); break;
            case 216: printf("CIRCLE "); break;
            case 217: printf("INK "); break;
            case 218: printf("PAPER "); break;
            case 219: printf("FLASH "); break;
            case 220: printf("BRIGHT "); break;
            case 221: printf("INVERSE "); break;
            case 222: printf("OVER "); break;
            case 223: printf("OUT "); break;
            case 224: printf("LPRINT "); break;
            case 225: printf("LLIST "); break;
            case 226: printf("STOP "); break;
            case 227: printf("READ "); break;
            case 228: printf("DATA "); break;
            case 229: printf("RESTORE "); break;
            case 230: printf("NEW "); break;
            case 231: printf("BORDER "); break;
            case 232: printf("CONTINUE "); break;
            case 233: printf("DIM "); break;
            case 234: printf("REM "); break;
            case 235: printf("FOR "); break;
            case 236: printf("GO TO "); break;
            case 237: printf("GO SUB "); break;
            case 238: printf("INPUT "); break;
            case 239: printf("LOAD "); break;
            case 240: printf("LIST "); break;
            case 241: printf("LET "); break;
            case 242: printf("PAUSE "); break;
            case 243: printf("NEXT "); break;
            case 244: printf("POKE "); break;
            case 245: printf("PRINT "); break;
            case 246: printf("PLOT "); break;
            case 247: printf("RUN "); break;
            case 248: printf("SAVE "); break;
            case 249: printf("RANDOMIZE "); break;
            case 250: printf("IF "); break;
            case 251: printf("CLS "); break;
            case 252: printf("DRAW "); break;
            case 253: printf("CLEAR "); break;
            case 254: printf("RETURN "); break;
            case 255: printf("COPY "); break;
            default: break;
          }
        }
      }
    }
    printf("\n");
  }
}
