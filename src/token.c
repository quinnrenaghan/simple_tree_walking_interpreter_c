#include "token.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int token_created = 0;
int token_freed = 0;

token_type lookup_identifier(char *identifier) {
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

void release_token(token *token) {
    if (token != NULL) {
        if (token->ref_count == 1) {
            token_freed++;
            free(token->value);
            free(token);
        } else {
            token->ref_count--;
        }
    }
}

void retain_token(token *token){
    if(token != NULL){
        (token->ref_count++);
    }
}

token *create_token(token *t){
    t = malloc(sizeof(token));
    t->ref_count = 1;
    token_created++;
    return t;
}
