#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "lexer.h"
#include "repl.h"
#include "token.h"

int peek_token_is(parser* p, token_type t) {
    if(p->peek_token == NULL){
        return 0;
    } else {
        return p->peek_token->type == t;
    }
}

int curr_token_is(parser* p, token_type t) {
    if(p->curr_token == NULL){
        return 0;
    } else {
        return p->curr_token->type == t;
    }
}

int expect_peek(parser* p, token_type t) {
    if (peek_token_is(p, t)) {
        p_next_token(p);
        return 1;
    } else {
        peek_error(p, t);
        return 0;
    }
}

char** errors(parser *p){
    return p->errors;
    
}

void peek_error(parser *p, token_type t){
    char* error = malloc(MAX_STR_LEN + 1);
    sprintf(error, "expected type: %d, received type: %d", t, p->peek_token->type);
    p->errors[p->error_count] = malloc(strlen(error) + 1);
    strcpy(p->errors[p->error_count], error);
    (p->error_count)++;
}

void p_next_token(parser* p) {
    p->curr_token = p->peek_token;
    p->peek_token = next_token(p->l);
}

stmt* parse_ret_statement(parser *p){
    stmt*s = malloc(sizeof(stmt));
    s->type = RET_STMT;
    s->stmt_token = p->curr_token;

    p_next_token(p);

    while (!curr_token_is(p, SEMICOLON)) {
        p_next_token(p);
        if(p->curr_token->type == EOF_TOKEN){
            return NULL;
        }
    }
    return s;
}

stmt* parse_set_statement(parser *p){
    stmt* s = malloc(sizeof(stmt));
    s->type = SET_STMT;
    s->stmt_token = p->curr_token;
    set_stmt* curr_set = malloc(sizeof(set_stmt));
    
    if (!expect_peek(p, IDENT)) {
        return NULL;
    }
    curr_set->identifier = p->curr_token;
    s->data = malloc(sizeof(stmt_data));

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
        case RETURN:
            return parse_ret_statement(p);
        default:
            return parse_expr_statement(p);
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

parser* p_new(lexer* lexer) {
    parser* p = malloc(sizeof(parser));
    p->l = lexer;
    p_next_token(p);
    p_next_token(p);
    p->errors = malloc(sizeof(char*) * MAX_ERRORS);
    p->error_count = 0;
    return p;
}

stmt* parse_expr_statement(parser *p){
    stmt* s = malloc(sizeof(stmt));
    s->stmt_token = p->curr_token;
    s->type = EXPR_STMT;
    expr_stmt* expr_stmt = malloc(sizeof(expr_stmt));
    s->data = malloc(sizeof(stmt_data));
    s->data->expr = expr_stmt;
    
    expr_stmt->value = parse_expression(p, LOWEST_PR);

    if(peek_token_is(p, SEMICOLON)){
        p_next_token(p);
    }
    return s;
}

expr* parse_expression(parser *p, precedence pr){
    return NULL;
}

