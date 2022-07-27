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
#define WORDSSIZE 1024
#define BUFFERSIZE 1024

/**
 * Funktioiden prototyypit
 */
int readFromStdinToStdout();
int readFromFileWriteToStdout(const char *filename);
int readFromFileWriteToFile(const char *fn_read, const char *fn_write);

int main(int argc, char *argv[])
{
    int ret = 0;

    /**
     * päätellään argumenttien määrästä operaatio, joka tehdään:
     * 1 = luetaan stdin:stä stdout:iin
     * 2 = luetaan annetusta tiedostosta stdout:iin
     * 3 = luetaan annetusta tiedostosta annettuun tiedostoon
     */
    if (argc >3) { // tarkistus liian isolle parametri määrälle
        fprintf(stdout, "");
        exit(1);
    }
    switch (argc) {
        case 1:
            ret = readFromStdinToStdout();
            break;
        case 2:
            ret = readFromFileWriteToStdout(argv[1]);
            break;
        case 3:
            ret = readFromFileWriteToFile(argv[1], argv[2]);
            break;
        default:
            break;
    }

    return ret;
}

/**
 * @brief luetaan stdinistä kääntäen stdoutiin
 * https://stackoverflow.com/questions/41518039/how-to-input-strings-into-an-array-in-c
 * @return int 
 */
int readFromStdinToStdout()
{
    char *words[WORDSSIZE];
    char buffer[BUFFERSIZE];
    size_t i; 
    size_t counter = 0;
    size_t buflen;

    /* loopataan kunnes raja tulee vastaan */
    for (i = 0; i < WORDSSIZE; i++) {

        /* luetaan lause ja tarkastetaan virhe */
        printf("Anna lause: ");
        if (fgets(buffer, BUFFERSIZE, stdin) == NULL) {
            fprintf(stderr, "Error reading string into buffer.\n");
            exit(EXIT_FAILURE);
        }

        /* poistetaan newline bufferista ja tarkastetaan overflow */
        buflen = strlen(buffer);
        if (buflen > 0) {
            if (buffer[buflen-1] == '\n') {
                buffer[buflen-1] = '\0';
            } else {
                fprintf(stderr, "Exceeded buffer length of %d.\n", BUFFERSIZE);
                exit(EXIT_FAILURE);
            }
        }

        /* mitään ei tullut bufferiin, joten lopetetaan */
        if (!*buffer) {
            break;
        }

        /* allokoidaan tilaa words[i] js null terminaattorille */
        words[counter] = malloc(strlen(buffer)+1);

        /* kopioidaan pointeriin */
        strcpy(words[counter], buffer);

        /* laskurin kasvatus */
        counter++;
    }

    /* lukeminen valmis käännetään lauseet */
    printf("\nKirjoittamasi lauseet olivat käänteisessä järjestyksessä:\n");
    for (int j = counter -1; j >= 0; j--) {
        fprintf(stdout," %s\n", words[j]);
        free(words[j]);
        words[j] = NULL;
    }

    return 0;
}

/**
 * @brief luetaan tiedosto ja käännetään tiedostoon
 * 
 * @param fn_read 
 * @param fn_write 
 * @return int 
 */
int readFromFileWriteToFile(const char *fn_read, const char *fn_write)
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

/**
 * @brief luetaan tiedostosto kääntäen stdoutiin
 * 
 * @param filename 
 * @return int 
 */
int readFromFileWriteToStdout(const char *filename)
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