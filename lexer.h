#ifndef LEXER_H
#define LEXER_H

#include "token.h"

typedef struct {
    char *input;
    int position;
    int read_position;
    char ch;
} lexer;

lexer* new(char *input);
void read_char(lexer *l);
token* next_token(lexer *l);
char* read_identifier(lexer *l);
char* read_int(lexer *l);
int is_letter(char c);
int is_digit(char c);
void eat_whitespace(lexer *l);
char peek_ahead(lexer *l);
char *read_string(lexer *l);

#endif