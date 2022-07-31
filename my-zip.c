#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>

/**
 * @brief lähde: https://stackoverflow.com/questions/14002954/c-programming-how-to-read-the-whole-file-contents-into-a-buffer
 * my-zip ratkasuni
 */
/**
 * @brief Gets how long should RLE be
 * 
 * @param s 
 * @return int 
 */
int run_length(char* s) 
{
  char c = s[0];
  if (c == '\0') return 0;
  int length = 1;
  for(; s[length] == c; length++);
  return length;
}

/**
 * @brief Get String length
 * 
 * @param i 
 * @return int 
 */
int strlen_i(int i) 
{
  return snprintf(NULL, 0, "%i", i);
}

/**
 * @brief RLE length
 * 
 * @param s 
 * @return int 
 */
int rle_len(char* s) 
{
  int len = 0;
  int rl = run_length(s);
  while (rl > 0) {
    len += 1 + strlen_i(rl);
    s += rl;
    rl = run_length(s);
  }
  return len;
}

/**
 * @brief RLE encode
 * 
 * @param s 
 * @return char* 
 */
char* rle_encode(char* s) 
{
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

/**
 * @brief Write binary to output
 * Idea taken from: https://stackoverflow.com/questions/39322417/how-to-printf-binary-data-in-c
 * @param input 
 */
void print_rle(char* input) 
{
  char* output = rle_encode(input);
  
  size_t size = sizeof(output); /* or however much you're planning to write */
  fwrite(output, 1, size, stdout); // write output to stdout
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
    print_rle(buffer); // print rle
    free(buffer); // free buffer
  }
  
  return 0;
}