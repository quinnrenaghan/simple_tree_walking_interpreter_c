#ifndef LEXER_H 
#define LEXER_H

#include "token.h"

typedef struct {
    char* input;
    int position;
    int read_position;
    char ch;
} lexer;

token* next_token(lexer *l);
void read_char(lexer *l);
lexer* new(char* input);
char* read_identifier(lexer *l);
int is_letter(char c);
void eat_whitespace(lexer *l);
int is_digit(char c);
char* read_int(lexer *l);
char peek_ahead(lexer *l);

#endif