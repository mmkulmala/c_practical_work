#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * @brief my-cat ohjelma
 * lähde: https://austinedger.medium.com/unix-commands-lets-build-cat-59b8a91b9708
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char **argv)
{
    FILE *fp; // tiedosto pointteri
    const int bufferSize = 4096; // bufferin koko
    char buffer[bufferSize]; // bufferi
    int currentFile = optind; // muuttuja, joka kertoo indeksin seuraavaan käsiteltävään ar

    if (argc < 1) { // ei mitään catattavaa täällä..
        exit(0);
    } else {
        while (currentFile <= argc) { // luupataan kunnes parametrien määrä on pienempi tai yhtä suuri kuin currentFile muuttujan arvo
            if (currentFile != argc) {
                fp = fopen(argv[currentFile], "rb"); // avataan tiedosto luettavaksi flägillä: read binary
                if (fp == NULL) { // tiedoston avaaminen epäonnistui
                    fprintf(stderr, "my-cat: cannot open file\n");
                    exit(1);
                }
            }

            while (fgets(buffer, bufferSize, (fp == NULL ? stdin : fp))) { // luetaan tiedoston sisältö tarkastamalla, ettei fp ole null ternary-tavalla
                int length = strlen(buffer); // katsotaan bufferin koko
                buffer[length -1] = '\0'; // viimeiseksi merkiksi null terminator
                fprintf(stdout, "%s\n", buffer); // tulostetaan tiedoston sisältö
            }

            fclose(fp); // suljetaan tiedosto
            currentFile++;
        }
    }
    
    return 0;
}
