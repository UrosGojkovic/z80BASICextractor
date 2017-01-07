#include "memory_extractor.h"

int open_file(char* path)
{
  source = fopen(path, "r+b");
  if (source == NULL)
  {
    fatal_error("Failed to open file specified");
  }
  return 0;
}

int check_header()
{
  version = 0;
  unsigned char byte;
  fseek(source, 6, SEEK_SET);
  fread(&byte, 1, 1, source);
  if (byte == 0)
  {
    fread(&byte, 1, 1, source);
    if (byte == 0)
    {
      printf("File is not in version 1 format...\n");
    }
    else
    {
      printf("File is in version 1 format...\n");
      version = 1;
    }
  }
  if (version == 1)
  {
    //if version is 1 check for the compression flag
    fseek(source, 12, SEEK_SET);
    fread(&byte, 1, 1, source);
    if (byte & 0x10)
    {
      is_compressed = TRUE;
    }
    else
    {
      is_compressed = FALSE;
    }
    switch(is_compressed)
    {
      case TRUE: printf("File is compressed...\n"); break;
      case FALSE: printf("File is not compressed...\n"); break;
      default: break; //silently ignore other values
    }
  }
  else
  {
    fseek(source, 30, SEEK_SET);
    fread(&byte, 1, 1, source);
    if (byte == 23)
    {
      printf("File is in version 2 format...\n");
      version = 2;
      header_end = 55;
    }
    else if (byte == 54 || byte == 55)
    {
      printf("File is in version 3 format...\n");
      version = 3;
      header_end = 30 + byte + 1;
    }
    else
    {
      printf("File is in uknown format...\n");
      //return 1;
      exit(EXIT_FAILURE); //leave like this until we enable batch proccessing
    }
    fseek(source, 34, SEEK_SET);
    fread(&byte, 1, 1, source);
    char* type;
    if (version == 2)
    {
      switch(byte)
      {
        case   0: type = "48k"; machine_type = forty; break;
        case   1: type = "48k + Interface 1"; machine_type = forty; break;
        case   2: type = "SamRam"; machine_type = SamRam; break;
        case   3: type = "128k"; machine_type = onetwenty; break;
        case   4: type = "128k + Interface 1"; machine_type = onetwenty; break;
        case   7: type = "Spectrum +3"; break;
        case   9: type = "Pentagon (128k)"; break;
        case  10: type = "Scorpion (256k)"; break;
        case  11: type = "Didaktik-Kompakt"; break;
        case  12: type = "Spectrum +2"; break;
        case  13: type = "Spectrum +2A"; break;
        case  14: type = "TC2048"; break;
        case  15: type = "TC2068"; break;
        case 128: type = "TS2068"; break;
        default: type = "unknown"; break;
      }
    }
    if (version == 3)
    {
      switch (byte)
      {
        case   0: type = "48k"; machine_type = forty; break;
        case   1: type = "48k + Interface 1"; machine_type = forty; break;
        case   2: type = "SamRam"; machine_type = SamRam; break;
        case   3: type = "48k + MGT"; machine_type = forty; break;
        case   4: type = "128k"; machine_type = onetwenty; break;
        case   5: type = "128k + Interface 1"; machine_type = onetwenty; break;
        case   6: type = "128k + MGT"; machine_type = onetwenty; break;
        case   7: type = "Spectrum +3"; break;
        case   9: type = "Pentagon (128k)"; break;
        case  10: type = "Scorpion (256k)"; break;
        case  11: type = "Didaktik-Kompakt"; break;
        case  12: type = "Spectrum +2"; break;
        case  13: type = "Spectrum +2A"; break;
        case  14: type = "TC2048"; break;
        case  15: type = "TC2068"; break;
        case 128: type = "TS2068"; break;
        default: type = "unknown"; break;
      }
    }
    //missing hardware type modifier check
    printf("Machine type is %s.\n", type);
  }
  return 0;
}

int extract_pages()
{
  printf("Begin extraction of pages...\n");
  unsigned char* pages[12];
  int i;
  for (i = 0; i < 12; i++)
  {
    pages[i] = NULL; //prep the array
  }
  unsigned char byte;
  int len;

  if (version == 1)
  {
    //extract version 1 memory block which has no paging
    if (is_compressed == TRUE)
    {
      memory = decompress(30, 0);
    }
    else
    {
      memory = malloc(3*PAGE_SIZE); //48k RAM = 3*16k RAM = 3*16,384 = 49,152
      if (memory == NULL)
      {
        fatal_error("extract_pages(): Can't allocate memory");
      }
      fseek(source, 30, SEEK_SET);
      if (fread(memory, 1, 3 * PAGE_SIZE, source) != 3*PAGE_SIZE)
      {
        printf("ERROR: Memory block has less than 48k bytes!\n");
        exit(EXIT_FAILURE);
      }
    }
  }
  if (version > 1)
  {
    fseek(source, header_end, SEEK_SET);
    long int end = ftell(source);
    while(!feof(source))
    {
      fseek(source, end, SEEK_SET); //reset position to the value from the end of the loop
      //get the length of the block
      fread(&byte, 1, 1, source);
      len = byte; //lower byte comes first
      fread(&byte, 1, 1, source);
      len += byte * BYTE_LEN; //add higher byte
      //get the page number
      fread(&byte, 1, 1, source);
      printf("Length of the block is %d...\n", len);
      //higher version files do not use compression flag, instead they use the block length as an indicator
      if (len == 0xffff)
      {
        //uncompressed page
         printf("Extracting uncompressed page %d...\n", byte);
        pages[byte] = malloc(PAGE_SIZE);
        if (pages[byte] == NULL)
        {
          fatal_error("extract_pages(): Can't allocate memory");
        }
        if (fread(pages[byte], 1, PAGE_SIZE, source) != PAGE_SIZE)
        {
          fatal_error("Unexpected end of stream");
        }
      }
      else
      {
        //compressed page
        printf("Extracting compressed page %d...\n", byte);
        pages[byte] = decompress(ftell(source), len);
      }
      //a trick to trigger EOF flag
      /*
        Explanation: our reading of the file is byte perfect and should stop when we read the last byte of the file.
        But, EOF is triggered when we try to read PAST the last byte, which simply doesn't happen in our case.

        So we use the next 2 lines to test the water:
        - if it's the last byte, fread will trigger EOF and feof() will bring us out of the loop,
        - if it's not the last byte we will use end variable to undo the reading of that extra byte.

        The reason why we use "fseek-to-the-end" at the start of the loop (instead of doing it immediately after fread) is that fseek resets EOF flag, thus, nullfying what we are trying to do.
      */
      end = ftell(source); //save the current position so we can come back if it's not end of file
      fread(&byte,1,1,source); //read a byte: if we are at the end we will trigger EOF
    }

    if (machine_type == forty)
    {
      memory = calloc(3*PAGE_SIZE, 1); //allocate space for all of RAM and set to 0
      if (memory == NULL)
      {
        fatal_error("extract_pages(): Can't allocate memory");
      }

      if (pages[8] != NULL)
      {
        memmove(memory, pages[8], PAGE_SIZE);
        printf("Extracted page 8...\n");
      }

      if (pages[4] != NULL)
      {
        memmove(memory + PAGE_SIZE, pages[4], PAGE_SIZE);
        printf("Extracted page 4...\n");
      }

      if (pages[5] != NULL)
      {
        memmove(memory + (2 * PAGE_SIZE), pages[5], PAGE_SIZE);
        printf("Extracted page 5...\n");
      }
    }
    //TODO: if machine_type other than forty
  }
  fclose(source);
  return 0;
}

unsigned char* decompress(long int starting_offset, long int length)
{
  unsigned char* page=NULL;
  int end_of_string=0;
  if (length == 0)
  {
    //TODO: version 1 with single continous block
  }
  else
  {
    fseek(source, starting_offset, SEEK_SET);
    page = malloc(PAGE_SIZE);
    if (page == NULL)
    {
      fatal_error("decompress(): Can't allocate memory");
    }
    long int i;
    long int end = starting_offset + length;
    unsigned char byte;
    for(i = starting_offset; i < end; i++)
    {
      fread(&byte, 1, 1, source);
      if (byte == 0xED)
      {
        long int cur_pos = ftell(source); //address of 0xED byte+1
        fread(&byte, 1, 1, source);
        if (byte == 0xED)
        {
          //there were two ED next to each other => apply decompression
          unsigned char block_length;
          unsigned char content;
          fread(&block_length, 1, 1, source);
          fread(&content, 1, 1, source);
          int j;
          for (j = 0; j < block_length; j++)
          {
            page[end_of_string] = content;
            end_of_string++;
          }
          i=i + 3; //add the 3 extra bytes we read to the loop var
        }
        else
        {
          //it was just one 0xED
          page[end_of_string] = 0xED;
          end_of_string++;
          fseek(source, cur_pos, SEEK_SET); //reset the position to the byte after 0xED
        }
      }
      else
      {
        page[end_of_string] = byte;
        end_of_string++;
      }
    }
  }
  return page;
}
