#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "lexer.h"
#include "repl.h"
#include "token.h"
#include <unistd.h>

/* checks if the next token matches the parameter.*/
int peek_token_is(parser* p, token_type t) {
    if (p->peek_token == NULL) {
        return 0;
    } else {
        return p->peek_token->type == t;
    }
}

/* checks if the current token matches the parameter.*/
int curr_token_is(parser* p, token_type t) {
    if (p->curr_token == NULL) {
        return 0;
    } else {
        return p->curr_token->type == t;
    }
}

/* checks for the expected token, and advances if it finds one.*/
int expect_peek(parser* p, token_type t) {
    if (peek_token_is(p, t)) {
        p_next_token(p);
        return 1;
    } else {
        peek_error(p, t);
        return 0;
    }
}
/* get the parser errors.*/
char** errors(parser* p) { return p->errors; }

/* adds an error to the list.*/
void add_error(parser* p, char* error) {
    p->errors[p->error_count] = error;
    p->error_count++;
}

/* logs error when the parser does not find the expected token.*/
void peek_error(parser* p, token_type t) {
    p->errors[p->error_count] = malloc(MAX_STR_LEN + 1);
    sprintf(p->errors[p->error_count], "expected type: %d, received type: %d",
            t, p->peek_token->type);
    (p->error_count)++;
}

/* advances the parser by one token.*/
void p_next_token(parser* p) {
    p->curr_token = p->peek_token;
    p->peek_token = next_token(p->l);
}

/* returns a statement representing the return statement that the parser is
 * currently located on, if a valid one exists.*/
stmt* parse_ret_statement(parser* p) {
    stmt* s = malloc(sizeof(stmt));
    s->type = RET_STMT;
    s->token = p->curr_token;

    p_next_token(p);

    while (!curr_token_is(p, SEMICOLON)) {
        p_next_token(p);
    }
    return s;
}

/* returns a statement representing the set statement that the parser is
 * currently located on, if a valid one exists.*/
stmt* parse_set_statement(parser* p) {
    stmt* s = malloc(sizeof(stmt));
    s->type = SET_STMT;
    s->token = p->curr_token;
    set_stmt* curr_set = malloc(sizeof(set_stmt));

    if (!expect_peek(p, IDENT)) {
        return NULL;
    }
    curr_set->identifier = p->curr_token;

    if (!expect_peek(p, ASSIGN)) {
        return NULL;
    }

    while (!curr_token_is(p, SEMICOLON)) {
        p_next_token(p);
    }

    s->data.set = curr_set;

    return s;
}

/* calls the correct parser helper method based on the type of the current
 * token.*/
stmt* parse_statement(parser* p) {
    switch (p->curr_token->type) {
        case SET:
            return parse_set_statement(p);
        case RETURN:
            return parse_ret_statement(p);
        default:
            return parse_expr_statement(p);
    }
}

/* parses an entire program, storing and returning the statements.*/
stmt_list* parse_program(parser* p) {
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

/* creates a new lexer, initializes all necessary fields.*/
parser* p_new(lexer* lexer) {
    parser* p = malloc(sizeof(parser));
    p->l = lexer;
    p_next_token(p);
    p_next_token(p);
    p->errors = malloc(sizeof(char*) * MAX_ERRORS);
    p->error_count = 0;
    return p;
}

/* returns an expression statement representing the statement that the parser is
 * currently located on, if a valid one exists*/
stmt* parse_expr_statement(parser* p) {
    stmt* s = malloc(sizeof(stmt));
    s->token = p->curr_token;
    s->type = EXPR;
    expr* expr = malloc(sizeof(expr));
    expr = parse_expression(p, LOWEST_PR);
    s->data.expr = expr;

    if (peek_token_is(p, SEMICOLON)) {
        p_next_token(p);
    }
    return s;
}

/* calls the correct helper method for the current expression, based on the
 * precedence and type of expression.*/
expr* parse_expression(parser* p, precedence pr) {
    expr* left_expr;
    switch (p->curr_token->type) {
        case IDENT:
        case INT:
        case TRUE:
        case FALSE:
            left_expr = parse_lit(p);
            break;
        case MINUS:
        case BANG:
            left_expr = parse_prefix(p);
            break;
        case LPAREN:
        case IF:
        case FUNCTION:
        case LBRACE:
        default: {
            char* error = malloc(MAX_STR_LEN + 1);
            sprintf(error, "do not have expression supported for token: %s",
                    p->curr_token->value);
            add_error(p, error);
            left_expr = NULL;
        }
    }
    return left_expr;
}

/* parses all literals, based on each of the 5 types.*/
expr* parse_lit(parser* p) {
    expr* left_expr = malloc(sizeof(expr));
    left_expr->type = LITERAL_EXPR;

    literal* l = malloc(sizeof(literal));

    switch (p->curr_token->type) {
        case IDENT:
            l->type = IDENT_LIT;
            l->val.token = p->curr_token;
            break;
        case INT:
            l->type = INT_LIT;
            l->val.num = atoi(p->curr_token->value);
            break;
        default:
            return NULL;
    }

    left_expr->data.lit = l;
    return left_expr;
}

expr* parse_prefix(parser* p) {
    expr* ex = malloc(sizeof(expr));
    ex->type = PREFIX_EXPR;

    prefix_expr* pre = malloc(sizeof(prefix_expr));
    pre->operator= p->curr_token;

    p_next_token(p);

    pre->right = parse_expression(p, PREFIX_PR);
    ex->data.pre = pre;
    return ex;
}
