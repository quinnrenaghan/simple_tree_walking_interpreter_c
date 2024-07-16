#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "lexer.h"


int start(FILE* in, FILE* out){
    char line[528];
    fprintf(out, "$");
    while(fgets(line, sizeof(line),in) != NULL){
        lexer *l = new(line);
        token *t = next_token(l);
        while(t->type != EOF_TOKEN){
            fprintf(out, "Type: %d Value: %s\n", t->type, t->value);
            t = next_token(l);
            fprintf(out, "$");
        }
    }
    return 0;
}