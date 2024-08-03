#ifndef AST_H
#define AST_H

#include <stdbool.h>
#include "token.h"

typedef enum {
    INT_LIT,
    STRING_LIT,
    BOOL_LIT,
    IDENT_LIT,
    FN_LIT
} literal_type;

typedef enum {
    LOWEST_PR,
    EQUALS_PR,
    LG_PR,
    SUM_PR,
    PRODUCT_PR,
    PREFIX_PR,
    CALL_PR,
    INDEX_PR
} precedence;

typedef enum {
    INFIX_EXPR,
    PREFIX_EXPR,
    LITERAL_EXPR,
    COND_EXPR,
    CALL_EXPR
} expr_type;

typedef struct expr expr;

typedef enum {
    SET_STMT,
    RET_STMT,
    EXPR,
} stmt_type;

typedef struct {
    token *identifier;
    expr *value;
} set_stmt;

typedef struct {
    expr *value;
} ret_stmt;

typedef struct {
    stmt_type type;
    union {
        set_stmt *set;
        ret_stmt *ret;
        expr *expr;
    } data;
    int ref_count;
} stmt;

typedef struct stmt_list {
    stmt **statements;
    int count;
    int capacity;
} stmt_list;

typedef struct {
    token **tokens;
    int count;
    int capacity;
} token_list;

typedef struct {
    expr **exprs;
    int count;
    int capacity;
} expr_list;

typedef struct {
    token_list *params;
    stmt_list *body;
} fn;

typedef struct {
    literal_type type;
    union {
        int num;
        token *token;
        bool boolean;
        fn *function;
        char* string;
    } val;
} literal;

typedef struct {
    token *operator;
    expr *right;
} prefix_expr;

typedef struct {
    token *operator;
    expr *right;
    expr *left;
} infix_expr;

typedef struct {
    expr *condition;
    stmt_list *consequence;
    stmt_list *alternative;
} cond_expr;

typedef struct {
    expr *fn;
    expr_list *args;
} call_expr;

typedef struct expr {
    expr_type type;
    union {
        literal *lit;
        prefix_expr *pre;
        infix_expr *inf;
        cond_expr *cond;
        call_expr *call;
    } data;
} expr;

precedence get_precedence(token *t);

#endif