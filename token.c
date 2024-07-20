#include "token.h"

#include <stdio.h>
#include <string.h>

token_type lookup_identifier(char* identifier) {
    if (!strcmp(identifier, "set")) {
        return SET;
    } else if (!strcmp(identifier, "fn")) {
        return FUNCTION;
    } else if (!strcmp(identifier, "true")) {
        return TRUE;
    } else if (!strcmp(identifier, "false")) {
        return FALSE;
    } else if (!strcmp(identifier, "if")) {
        return IF;
    } else if (!strcmp(identifier, "else")) {
        return ELSE;
    } else if (!strcmp(identifier, "return")) {
        return RETURN;
    } else {
        return IDENT;
    }
}
