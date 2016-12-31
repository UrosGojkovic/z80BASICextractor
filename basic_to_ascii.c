#include"basic_to_ascii.h"

char** build_line_buffer(MemBlock block)
{
  unsigned char byte;
  int i, count=0;
  char** buffer;
  int* b, breaks=NULL; /*niz i pom.niz za pozicije novih redova*/
  for(i=0;byte!=\0;i++)
  {
    byte=block[i];
    if(byte==\r)
    {
      count++;
   	  b=(int*)realloc(breaks, count*sizeof(int));
		if(b==NULL){
	     	perror("Failed to reallocate memory: breaks");
		}
		else
		breaks=b;
		breaks[count-1]=i;
	 
    }
  }
  if(count==0)
  {
    return NULL;
  }
  else
  {
    buffer=malloc(count*sizeof(char*));
    int k; /*duzina linije preko preth pozicija...*/
    for(k=count;k>1;k--){
    	breaks[k-1]=breaks[k-1]-breaks[k-2];
    }
  
   if(buffer==NULL)
    {
      perror("Failed to allocate memory: build_line_buffer");
    }
    else
    {
      int j=0;
      for(i=0;i<count;i++)
      {
      	buffer[i]=malloc(breaks[i]*sizeof(char));
        //build byte-strings
		for(k=0;block[j+i]!=\r;k++){
      	  buffer[i][k]=block[j+i];  	  
        }
          buffer[i][k]=\n;
	      j++;
      }
      buffer[i][k]=\0;
      i=k=0;
      
      while(buffer[i][k]!=\0){
        if(buffer[i][k]==\r){
          i++;
		  printf("\n");
        }
        else{
      	if(buffer[i][k]<32) continue;
      	else{ if(buffer[i][k]<128){
      			switch(buffer[i][k]){ /*standardi se razlikuju kod...*/
      				case 127: 
					  buffer[i][k]=169;
					  break;
      				case 96:
      				  buffer[i][k]=163;
      				  break;
      				case 94:
      				  buffer[i][k]=2191; /*utf-8 karakter!*/
      				default: ;
				}
				printf("%c", buffer[i][k]);
		     }
		     
      		else switch(buffer[i][k]){
      			case 165:
      				printf("RND");
       			case 166:
       				printf("INKEY$");
       			case 167:
       				printf("PI");
       			case 168:
       				printf("FN");
       			case 169:
       				printf("POINT");
       			case 170:
      				printf("SCREENS");
      			case 171:
      				printf("ATTR");
       			case 172:
       				printf("AT");
       			case 173:
       				printf("TAB");
       			case 174:
       				printf("VAL$");
      			case 175:
       				printf("CODE");
       			case 176:
       				printf("VAL");
       			case 177:
       				printf("LEN");
       			case 178:
       				printf("SIN");
       			case 179:
       				printf("COS");
       			case 180:
       				printf("TAN");
       			case 181:
       				printf("ASN");
       			case 182:
       				printf("ACS");
       			case 183:
       				printf("ATN");
       			case 184:
       				printf("LN");
       			case 185:
       				printf("EXP");
       			case 186:
       				printf("INT");
       			case 187:
       				printf("SOR");
       			case 188:
       				printf("SGN");
       			case 189:
       				printf("ABS");
       			case 190:
       				printf("PEEK");
       			case 191:
       				printf("IN");
       			case 192:
       				printf("USR");
       			case 193:
       				printf("STRS");
       			case 194:
       				printf("CHRS");
      			case 195:
       				printf("NOT");
       			case 196:
       				printf("BIN");
       			case 197:
       				printf("OR");
       			case 198:
       				printf("AND");
       			case 199:
       				printf("<=");
       			case 200:
       				printf(">=");
       			case 201:
       				printf("<>");
       			case 202:
       				printf("LINE");
       			case 203:
       				printf("THEN");
       			case 204:
       				printf("TO");
      			case 205:
       				printf("STEP");
       			case 206:
       				printf("DEF FN");
       			case 207:
       				printf("CAT");
       			case 208:
       				printf("FORMAT");
       			case 209:
       				printf("MOVE");
       			case 210:
       				printf("ERASE");
       			case 211:
       				printf("OPEN #");
       			case 212:
       				printf("CLOSE #");
       			case 213:
       				printf("MERGE");
       			case 214:
       				printf("VERIFY");
      			case 215:
       				printf("BEEP");
       			case 216:
       				printf("CIRCLE");
       			case 217:
       				printf("INK");
       			case 218:
       				printf("PAPER");
       			case 219:
       				printf("FLASH");
       			case 220:
       				printf("BRIGHT");
       			case 221:
       				printf("INVERSE");
       			case 222:
       				printf("OVER");
       			case 223:
       				printf("OUT");
       			case 224:
       				printf("LPRINT");
      			case 225:
       				printf("LLIST");
       			case 226:
       				printf("STOP");
       			case 227:
       				printf("READ");
       			case 228:
       				printf("DATA");
       			case 229:
       				printf("RESTORE");
       			case 230:
       				printf("NEW");
       			case 231:
       				printf("BORDER");
       			case 232:
       				printf("CONTINUE");
       			case 233:
       				printf("DIM");
       			case 234:
       				printf("REM");
      			case 235:
       				printf("FOR");
       			case 236:
       				printf("GO TO");
       			case 237:
       				printf("GO SUB");
       			case 238:
       				printf("INPUT");
       			case 239:
       				printf("LOAD");
       			case 240:
       				printf("LIST");
       			case 241:
       				printf("LET");
       			case 242:
       				printf("PAUSE");
       			case 243:
       				printf("NEXT");
       			case 244:
/*this one...*/ printf("POKE"); /*A FAV! :)*/
      			case 245:
       				printf("PRINT");
       			case 246:
       				printf("PLOT");
       			case 247:
       				printf("RUN");
       			case 248:
       				printf("SAVE");
       			case 249:
       				printf("RANDOMIZE");
       			case 250:
       				printf("IF");
       			case 251:
       				printf("CLS");
       			case 252:
       				printf("DRAW");
       			case 253:
       				printf("CLEAR");
       			case 254:
       				printf("RETURN");
      			case 255:	
       				printf("COPY");				
      			default: ;
      			/* da li naredba sadrzi i razmak
				  ili se zasebno kodira kao sl kar
				  i da li za sve
				  ili za neke
				*/
				// --> ptintf(" ");
              
      		}
          }
        }
		  k++;
		}
      	
      }
        	  
      
      
    }
  }
}
