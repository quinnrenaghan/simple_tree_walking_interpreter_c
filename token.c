#include <string.h>
#include "token.h"
#include <stdio.h>


enum TokenType lookup_identifier(char* identifier){
    if(!strcmp(identifier, "let")){
        return LET;
    } else if (!strcmp(identifier, "fn")){
        return FUNCTION;
    } else {
        return IDENT;
    }
}

