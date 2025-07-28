#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/**
 * @brief Character to integer
 * Source: https://interviewsansar.com/char-to-int-c-program/
 * 
 * @param c 
 * @return int 
 */
int charToInt(char c){
  return c - '0';
}

/**
 * @brief Decode RLE
 * 
 * @param rle 
 */
void decode_rle(char* rle) {

  size_t i = 0;
  while (rle[i] != '\0') {
  
    if (isdigit(rle[i])) {
      int amountToWrite = charToInt(rle[i]);
      for(int j = 0; j < amountToWrite; j++) {
        printf("%c", rle[i-1]);
      }
      
    }
    i++;
  }
  printf("\n");
}

int main(int argc, char** argv) 
{
  // idea from: https://stackoverflow.com/questions/14002954/c-programming-how-to-read-the-whole-file-contents-into-a-buffer
  for(int i = 1; i < argc; i++) {
    FILE *fp = fopen (argv[i], "rb" );
    if(!fp) {
      fprintf(stderr, "reading failed %s\n", argv[i]);
      exit(1);
    }

    fseek(fp , 0L , SEEK_END);
    long fsize = ftell( fp );
    rewind(fp);

    /* allocate memory for entire content */
    char *buffer = malloc(fsize+1);
    if(!buffer) {
      fclose(fp);
      fprintf(stderr, "memory alloc fails");
      exit(1);
    }
    /* copy the file into the buffer */
    if(1!=fread( buffer , fsize, 1 , fp)) { // if not successful ..
      fclose(fp); // .. close file
      free(buffer); // .. free buffer
      fprintf(stderr, "reading fails"),
      exit(1);
    }
    fclose(fp); // close successful read
    buffer[fsize] = '\0'; // null terminator
    decode_rle(buffer); // print rle
    free(buffer); // free buffer
  }
  return 0; 
}
