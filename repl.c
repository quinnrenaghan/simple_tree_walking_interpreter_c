#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* begins the read, evaluate, print, and loop.*/
int start(FILE* in, FILE* out) {
    char line[528];
    fprintf(out, "$");
    while (fgets(line, sizeof(line), in) != NULL) {
        fprintf(out, "$");
    }
    return 0;
}