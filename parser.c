#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "lexer.h"
#include "repl.h"
#include "token.h"

int peek_precedence(parser* p) { return get_precedence(p->peek_token); }
int curr_precedence(parser* p) { return get_precedence(p->curr_token); }

/* checks if the next token matches the parameter.*/
bool peek_token_is(parser* p, token_type t) {
    if (p->peek_token == NULL) {
        return 0;
    } else {
        return p->peek_token->type == t;
    }
}

/* checks if the current token matches the parameter.*/
bool curr_token_is(parser* p, token_type t) {
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
    ret_stmt* ret = malloc(sizeof(ret_stmt));
    p_next_token(p);
    ret->value = parse_expression(p, LOWEST_PR);
    if (peek_token_is(p, SEMICOLON)) {
        p_next_token(p);
    }
    s->data.ret = ret;
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
    p_next_token(p);
    curr_set->value = parse_expression(p, LOWEST_PR);
    if (peek_token_is(p, SEMICOLON)) {
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
        case FUNCTION:
            left_expr = parse_lit(p);
            break;
        case MINUS:
        case BANG:
            left_expr = parse_prefix(p);
            break;
        case LPAREN:
            left_expr = parse_group(p);
            break;
        case IF:
            left_expr = parse_conditional(p);
            break;
        default: {
            char* error = malloc(MAX_STR_LEN + 1);
            sprintf(error,
                    "do not have prefix expression supported for token: %s",
                    p->curr_token->value);
            add_error(p, error);
            left_expr = NULL;
        }
    }

    while (!peek_token_is(p, SEMICOLON) && pr < peek_precedence(p)) {
        p_next_token(p);
        left_expr = parse_infix(p, left_expr);
    }
    return left_expr;
}

/* parses all literals, based on each of the 5 types.*/
expr* parse_lit(parser* p) {
    expr* ex = malloc(sizeof(expr));
    ex->type = LITERAL_EXPR;
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
        case TRUE:
        case FALSE:
            l->type = BOOL_LIT;
            l->val.boolean = curr_token_is(p, TRUE);
            break;
        case FUNCTION:
            l->type = FN_LIT;
            l->val.function = parse_function(p);
            break;
        default:
            return NULL;
    }
    ex->data.lit = l;
    return ex;
}

/* takes current token as the operator, and creates prefix expression*/
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

/* takes current token as the operator, and the expression parameter as the left
side, and creates an infix expression or function call*/
expr* parse_infix(parser* p, expr* left_expr) {
    expr* ex = malloc(sizeof(expr));
    switch (p->curr_token->type) {
        case LPAREN:
            ex->type = CALL_EXPR;
            call_expr* call = malloc(sizeof(call_expr));
            call->token = p->curr_token;
            call->fn = left_expr;
            call->args = parse_args(p);
            ex->data.call = call;
            break;
        default:
            ex->type = INFIX_EXPR;
            infix_expr* inf = malloc(sizeof(infix_expr));
            inf->operator= p->curr_token;
            inf->left = left_expr;
            precedence prec = curr_precedence(p);
            p_next_token(p);
            inf->right = parse_expression(p, prec);
            ex->data.inf = inf;
    }
    return ex;
}

/* parses parentheses (groups) with highest precedence*/
expr* parse_group(parser* p) {
    p_next_token(p);
    expr* ex = parse_expression(p, LOWEST_PR);
    if (!expect_peek(p, RPAREN)) {
        return NULL;
    }
    return ex;
}

/* returns conditional expression starting with the current token*/
expr* parse_conditional(parser* p) {
    expr* ex = malloc(sizeof(expr));
    ex->type = COND_EXPR;
    cond_expr* cond = malloc(sizeof(cond_expr));
    cond->token = p->curr_token;
    cond->alternative = NULL;
    if (!expect_peek(p, LPAREN)) {
        return NULL;
    }
    p_next_token(p);
    cond->condition = parse_expression(p, LOWEST_PR);
    if (!expect_peek(p, RPAREN)) {
        return NULL;
    }
    if (!expect_peek(p, LBRACE)) {
        return NULL;
    }
    cond->consequence = parse_block(p);
    if (peek_token_is(p, ELSE)) {
        p_next_token(p);
        if (!expect_peek(p, LBRACE)) {
            return NULL;
        }
        cond->alternative = parse_block(p);
    }
    ex->data.cond = cond;
    return ex;
}

/* returns list of statements representing block enclosed in curly braces*/
stmt_list* parse_block(parser* p) {
    stmt_list* list = malloc(sizeof(stmt_list));
    list->capacity = MAX_STATEMENTS;
    list->count = 0;
    list->statements = malloc(sizeof(stmt*) * list->capacity);
    p_next_token(p);
    while (!curr_token_is(p, RBRACE) && !curr_token_is(p, EOF_TOKEN)) {
        stmt* stmt = parse_statement(p);
        if (stmt != NULL) {
            list->statements[list->count] = stmt;
            (list->count)++;
        }
        p_next_token(p);
    }
    return list;
}

/* returns list of comma separated parameters (identifiers) in a function
 * definition, starting on the current token*/
token_list* parse_params(parser* p) {
    token_list* list = malloc(sizeof(token_list));
    list->count = 0;
    list->capacity = MAX_PARAMS;
    list->tokens = malloc(sizeof(token*) * list->capacity);
    if (peek_token_is(p, RPAREN)) {
        p_next_token(p);
        return list;
    }
    p_next_token(p);
    list->tokens[list->count] = p->curr_token;
    (list->count)++;
    while (peek_token_is(p, COMMA)) {
        p_next_token(p);
        p_next_token(p);
        list->tokens[list->count] = p->curr_token;
        (list->count)++;
    }
    if (!expect_peek(p, RPAREN)) {
        return NULL;
    }
    return list;
}

/* returns function, starting on the current token*/
fn* parse_function(parser* p) {
    fn* func = malloc(sizeof(fn));
    func->token = p->curr_token;
    if (!expect_peek(p, LPAREN)) {
        return NULL;
    }
    func->params = parse_params(p);
    if (!expect_peek(p, LBRACE)) {
        return NULL;
    }
    func->body = parse_block(p);
    return func;
}

/* returns list of comma separated arguments in a function call, starting on the
 * current token*/
expr_list* parse_args(parser* p) {
    expr_list* list = malloc(sizeof(expr_list));
    list->count = 0;
    list->capacity = MAX_PARAMS;
    list->exprs = malloc(sizeof(expr*) * list->capacity);
    if (peek_token_is(p, RPAREN)) {
        p_next_token(p);
        return list;
    }
    p_next_token(p);
    list->exprs[list->count] = parse_expression(p, LOWEST_PR);
    (list->count)++;
    while (peek_token_is(p, COMMA)) {
        p_next_token(p);
        p_next_token(p);
        list->exprs[list->count] = parse_expression(p, LOWEST_PR);
        (list->count)++;
    }
    if (!expect_peek(p, RPAREN)) {
        return NULL;
    }
    return list;
}