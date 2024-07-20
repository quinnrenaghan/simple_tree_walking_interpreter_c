#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "token.h"
#include "ast.h"

typedef struct{
    lexer* l;
    token* curr_token;
    token* peek_token;
} parser;

void p_next_token(parser* p);
parser* p_new(lexer* lexer);
stmt* parse_set_statement(parser *p);
stmt* parse_statement(parser *p);
stmt_list* parse_program(parser *p);
int peek_token_is(parser* p, enum TokenType t);
int curr_token_is(parser* p, enum TokenType t);
int expect_peek(parser* p, enum TokenType t);


#endif