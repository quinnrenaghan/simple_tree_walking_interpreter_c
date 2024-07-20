#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "lexer.h"
#include "token.h"

typedef struct{
    lexer* l;
    token* curr_token;
    token* peek_token;
    char** errors;
    int error_count;
} parser;

int peek_token_is(parser* p, token_type t);
int curr_token_is(parser* p, token_type t);
int expect_peek(parser* p, token_type t);
char** errors(parser *p);
void peek_error(parser *p, token_type t);
void p_next_token(parser* p);
stmt* parse_ret_statement(parser *p);
stmt* parse_set_statement(parser *p);
stmt* parse_statement(parser *p);
stmt_list* parse_program(parser *p);
parser* p_new(lexer* lexer);
stmt* parse_expr_statement(parser *p);
expr* parse_expression(parser *p, precedence pr);

#endif