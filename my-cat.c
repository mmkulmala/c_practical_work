#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Vakiot tiedoston lukemiseen
 */
#define READ_FILE "r"
#define LSIZ 1024
#define RSIZ 1024

/**
 * @brief Luetaan tiedosto stdoutiin
 * 
 * @param filename 
 * @param len 
 * @return int 
 */
int read_to_std(char *filename[], int len)
{
    for(int i = 1; i < len; i++) { // aloitetaan indeksistä 1
        char line[RSIZ][LSIZ];
        FILE *fp_read = NULL;
        int l = 0;

        fp_read = fopen(filename[i], READ_FILE);
        if( !fp_read ) perror(filename[i]),exit(1);

        // luetaan tiedosto
        while(fgets(line[l], LSIZ, fp_read))
        {
            fprintf(stdout," %s", line[l]);
            l++;
        }

        printf("\n");
        fclose(fp_read);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    read_to_std(argv, argc);

    return 0;
}

