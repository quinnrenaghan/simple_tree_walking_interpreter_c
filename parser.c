#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "lexer.h"
#include "repl.h"
#include "token.h"

#define MAX_STATEMENTS 500

void p_next_token(parser* p) {
    p->curr_token = p->peek_token;
    p->peek_token = next_token(p->l);
}
parser* p_new(lexer* lexer) {
    parser* p = malloc(sizeof(parser));
    p->l = lexer;
    p_next_token(p);
    p_next_token(p);
    return p;
}


stmt* parse_set_statement(parser *p){
    stmt* s = malloc(sizeof(stmt));
    s->type = SET_STMT;
    set_stmt* curr_set = malloc(sizeof(set_stmt));
    
    if (!expect_peek(p, IDENT)) {
        return NULL;
    }

    s->data = malloc(sizeof(stmt_data));
    curr_set->identifier = p->curr_token;

    if(!expect_peek(p, ASSIGN)){
        return NULL;
    }

    while (!curr_token_is(p, SEMICOLON)) {
        p_next_token(p);
        if(p->curr_token->type == EOF_TOKEN){
            return NULL;
        }
    }

    s->data->set = curr_set;

    return s;
    
}

stmt* parse_statement(parser *p){
    switch(p->curr_token->type){
        case SET:
            return parse_set_statement(p);
        default:
            return NULL;
    }
}

stmt_list* parse_program(parser *p){
    stmt_list* stmt_list = malloc(sizeof(stmt_list));
    stmt_list->count = 0;
    stmt_list->capacity = MAX_STATEMENTS;
    stmt_list->statements = malloc(sizeof(stmt*) * stmt_list->capacity);
    while (p->curr_token->type != EOF_TOKEN) {
        stmt* stmt = parse_statement(p);
        if (stmt != NULL) {
            stmt_list->statements[stmt_list->count] = stmt;
            (stmt_list->count)++;
        }
        p_next_token(p);
    }
    return stmt_list;
}

int peek_token_is(parser* p, enum TokenType t) {
    if(p->peek_token == NULL){
        return 0;
    } else {
        return p->peek_token->type == t;
    }
}

int curr_token_is(parser* p, enum TokenType t) {
    if(p->curr_token == NULL){
        return 0;
    } else {
        return p->curr_token->type == t;
    }
}

int expect_peek(parser* p, enum TokenType t) {
    if (peek_token_is(p, t)) {
        p_next_token(p);
        return 1;
    } else {
        return 0;
    }
}
