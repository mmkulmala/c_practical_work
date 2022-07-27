#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    FILE *fp;
    const int bufferSize = 4096;
    char buffer[bufferSize];
    int currentFile = (argc > 1 ? 1 : 0);

    if (argc < 1) {
        exit(0);
    } else {
        while (currentFile < argc) { 
            fp = fopen(argv[argc -1], "rb");
        
            if(fp == NULL) {
                fprintf(stderr, "my-cat: cannot open file\n");
                exit(1);
            }
            while (fgets(buffer, bufferSize, (fp == NULL ? stdin : fp))) {
                int length = strlen(buffer);
                buffer[length -1] = '\0';
                fprintf(stdout, "%s\n", buffer);
            }

            fclose(fp);
            currentFile++;
        }
    }
    
    return 0;
}
