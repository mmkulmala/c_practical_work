#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int charToInt(char c){
  return c - '0';
}

void de_rle(char* rle) {

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

int main(int argc, char** argv) {
  FILE *fp;
  long lSize;
  char *buffer;

  fp = fopen (argv[1], "rb" );
  if( !fp ) perror(argv[1]),exit(1);

  fseek( fp , 0L , SEEK_END);
  lSize = ftell( fp );
  rewind( fp );

  /* allocate memory for entire content */
  buffer = calloc( 1, lSize+1 );
  if( !buffer ) fclose(fp),fputs("memory alloc fails",stderr),exit(1);

  /* copy the file into the buffer */
  if( 1!=fread( buffer , lSize, 1 , fp) )
  fclose(fp),free(buffer),fputs("entire read fails",stderr),exit(1);
  
  de_rle(buffer);
  fclose(fp);
  free(buffer);
  return 0;
}