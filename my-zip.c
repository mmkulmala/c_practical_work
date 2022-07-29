#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

int run_length(char* s) {
  char c = s[0];
  if (c == '\0') return 0;
  int length = 1;
  for(; s[length] == c; length++);
  return length;
}

int strlen_i(int i) {
  return snprintf(NULL, 0, "%i", i);
}

int rle_len(char* s) {
  int len = 0;
  int rl = run_length(s);
  while (rl > 0) {
    len += 1 + strlen_i(rl);
    s += rl;
    rl = run_length(s);
  }
  return len;
}

char* rle(char* s) {
  int old_len = strlen(s);
  int new_len = rle_len(s);

  if (old_len <= new_len) {
    return strdup(s); // provides free()able copy
  }

  char* out = malloc((new_len+1) * sizeof(char));

  int out_next = 0;
  int rl = run_length(s);
  while (rl > 0) {
    out_next += sprintf(&out[out_next], "%c%i", s[0], rl);
    s += rl;
    rl = run_length(s);
  }
  out[out_next] = '\0';

  return out;
}

void test_rle_len(char* input, int expected) {
  assert(rle_len(input) == expected);
}

void test_rle(char* input, char* expected) {
  char* output = rle(input);
  assert(strcmp(output, expected) == 0);
  free(output);
}

int charToInt(char c){
  return c - '0';
}

void print_rle(char* input) {
  char* output = rle(input);
  // from: https://stackoverflow.com/questions/3213827/how-to-iterate-over-a-string-in-c
  for (char character = *output; character != '\0'; character = *++output)
  {
    if (isdigit(character)) {
      int int_to_compress = charToInt(character);
      fwrite(&int_to_compress,sizeof(int),1, stdout);
    } else {
      fwrite(&character, sizeof(char), 1, stdout);
    }
  }
  fflush(stdout);
}

int main(int argc, char** argv) {
  FILE *fp;
  long lSize;
  char *buffer;

  for(int i = 1; i < argc; i++) {
    fp = fopen (argv[i], "rb" );
    if( !fp ) perror(argv[i]),exit(1);

    fseek( fp , 0L , SEEK_END);
    lSize = ftell( fp );
    rewind( fp );

    /* allocate memory for entire content */
    buffer = calloc( 1, lSize+1 );
    if( !buffer ) fclose(fp),fputs("memory alloc fails",stderr),exit(1);

    /* copy the file into the buffer */
    if( 1!=fread( buffer , lSize, 1 , fp) )
    fclose(fp),free(buffer),fputs("entire read fails",stderr),exit(1);
    
    print_rle(buffer);
    fclose(fp);
    free(buffer);
  }
  
  return 0;
}