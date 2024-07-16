#include <string.h>
#include "token.h"
#include <stdio.h>


enum TokenType lookup_identifier(char* identifier){
    if(!strcmp(identifier, "let")){
        return LET;
    } else if (!strcmp(identifier, "fn")){
        return FUNCTION;
    } else if (!strcmp(identifier, "true")){
        return TRUE;
    } else if(!strcmp(identifier, "false")){
        return FALSE;
    } else if(!strcmp(identifier, "if")){
        return IF;
    } else if (!strcmp(identifier, "else")){
        return ELSE;
    } else if (!strcmp(identifier, "return")){
        return RETURN;
    } else {
        return IDENT;
    }
}

