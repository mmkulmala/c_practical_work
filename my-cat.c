#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    FILE *fp; // tiedosto pointteri
    const int bufferSize = 4096; // bufferin koko
    char buffer[bufferSize]; // bufferi
    int currentFile = (argc > 1 ? 1 : 0); // muuttuja, jolla kerrotaan monta tiedostoa luetaan, jollei yhtään anneta laitetaan muuttujan arvoksi 0

    if (argc < 1) { // ei mitään catattavaa täällä..
        exit(0);
    } else {
        while (currentFile < argc) { // luupataan kunnes parametrien määrä on pienempi kuin currentFile muuttujan arvo
            fp = fopen(argv[argc -1], "rb"); // avataan tiedosto luettavaksi flägillä: read binary
        
            if(fp == NULL) { // tiedoston avaaminen epäonnistui
                fprintf(stderr, "my-cat: cannot open file\n");
                exit(1);
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
