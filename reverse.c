#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Vakiot tiedoston lukemiseen ja kirjoittamiseen
 */
#define READ_FILE "r"
#define WRITE_FILE "w"
#define LSIZ 1024
#define RSIZ 1024
#define RSTDINSIZ 1024
#define BUFFSIZE 1024

/**
 * Funktioiden prototyypit
 */
int read_stdin_stdout();
int read_write_std(const char *filename);
int read_write_file(const char *fn_read, const char *fn_write);

int main(int argc, char *argv[])
{
    int ret = 0;

    /**
     * päätellään argumenttien määrästä operaatio, joka tehdään:
     * 1 = luetaan stdin:stä stdout:iin
     * 2 = luetaan annetusta tiedostosta stdout:iin
     * 3 = luetaan annetusta tiedostosta annettuun tiedostoon
     */
    switch (argc) {
        case 1:
            ret = read_stdin_stdout();
            break;
        case 2:
            read_write_std(argv[1]);
            break;
        case 3:
            ret = read_write_file(argv[1], argv[2]);
            break;
        default:
            break;
    }

    return ret;
}

int read_stdin_stdout()
{
    char *words[RSTDINSIZ];
    char buffer[BUFFSIZE];
    size_t i, count = 0, slen;

    /* looppaa kunnes raja RSTDINSIZ tulee vastaan */
    for (i = 0; i < RSTDINSIZ; i++) {

        /* luetaan lause ja tarkastetaan virhe */
        printf("Anna lause: ");
        if (fgets(buffer, BUFFSIZE, stdin) == NULL) {
            fprintf(stderr, "Error reading string into buffer.\n");
            exit(EXIT_FAILURE);
        }

        /* poistetaan newline bufferista ja tarkastetaan overflow */
        slen = strlen(buffer);
        if (slen > 0) {
            if (buffer[slen-1] == '\n') {
                buffer[slen-1] = '\0';
            } else {
                printf("Exceeded buffer length of %d.\n", BUFFSIZE);
                exit(EXIT_FAILURE);
            }
        }

        /* mitään ei tullut, joten lopetetaan*/
        if (!*buffer) {
            break;
        }

        /* allocate space for `words[i]` and null terminator */
        words[count] = malloc(strlen(buffer)+1);

        /* checking return of malloc, very good to do this */
        if (!words[count]) {
            printf("Cannot allocate memory for string.\n");
            exit(EXIT_FAILURE);
        }

        /* if everything is fine, copy over into your array of pointers */
        strcpy(words[count], buffer);

        /* increment count, ready for next space in array */
        count++;
    }

    /* lukeminen valmis käännetään lauseet */
    printf("\nKirjoittamasi lauseet olivat käänteisessä järjestyksessä:\n");
    for (int j = count -1; j >= 0; j--) {
        printf("%s\n", words[j]);
        free(words[j]);
        words[j] = NULL;
    }

    return 0;
}

int read_write_file(const char *fn_read, const char *fn_write)
{
    char line[RSIZ][LSIZ];
    FILE *fp_read = NULL;
    FILE *fp_write = NULL;
    int i = 0; // rivien määrä
    int tot = 0; // rivien määrä yhteensä

    // avataan tiedosto
    fp_read = fopen(fn_read, READ_FILE);

    // jos tiedosto on NULL keskeytetään suoritus
    if (fp_read == NULL)
    {
        fprintf(stderr, "Error opening file");
        exit(EXIT_FAILURE);
    }

    // luetaan tiedosto
    while(fgets(line[i], LSIZ, fp_read))
    {
        line[i][strlen(line[i]) - 1] = '\0';
        i++;
    }
    tot = i;

    fp_write = fopen(fn_write, WRITE_FILE);

    if (fp_write == NULL)
    {
        fprintf(stderr, "Error writing to file");
        exit(EXIT_FAILURE);
    }

    /**
    * käännetään annetut rivit lukemalla array lopusta alkuun
    */
    for(int j = tot -1; j >= 0; j--)
    {
        fprintf(fp_write," %s\n", line[j]);
    }

    fclose(fp_read);
    fclose(fp_write);
    return 0;
}

int read_write_std(const char *filename)
{
    char line[RSIZ][LSIZ];
    FILE *fp_read = NULL;
    int i = 0;
    int tot = 0;

    fp_read = fopen(filename, READ_FILE);

    if (fp_read == NULL)
    {
        fprintf(stderr, "Error opening file");
        exit(EXIT_FAILURE);
    }

    // luetaan tiedosto
    while(fgets(line[i], LSIZ, fp_read))
    {
        line[i][strlen(line[i]) - 1] = '\0';
        i++;
    }
    tot = i;

    /**
     * käännetään annetut rivit lukemalla array lopusta alkuun
     */
    for(int j = tot -1; j >= 0; j--)
    {
        fprintf(stdout," %s\n", line[j]);
    }
    printf("\n");
    fclose(fp_read);

    return 0;
}