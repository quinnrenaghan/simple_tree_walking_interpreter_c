#ifndef AST_H
#define AST_H

#include "token.h"

typedef enum {
    INFIX_EXPR,
    PREFIX_EXPR,
    LITERAL_EXPR,
    IF_EXPR,
    CALL_EXPR
} expr_type;

typedef union {
    int holder;
} expr_data;

typedef struct expr {
    expr_type type;
    expr_data data;
} expr;

typedef enum {
    LOWEST_PR,
    EQUALS_PR,
    LESSGREATER_PR,
    SUM_PR,
    PRODUCT_PR,
    PREFIX_PR,
    CALL_PR,
    INDEX_PR
} precedence;

typedef enum {
    SET_STMT,
    RET_STMT,
    EXPR_STMT,
} stmt_type;

typedef struct {
    token* identifier;
    expr* value;
} set_stmt;

typedef struct {
    expr* value;
} ret_stmt;

typedef struct {
    expr* value;
} expr_stmt;

typedef union {
    set_stmt* set;
    ret_stmt* ret;
    expr_stmt* expr;
} stmt_data;

typedef struct {
    stmt_type type;
    stmt_data* data;
    token* stmt_token;
} stmt;

typedef struct stmt_list {
    stmt** statements;
    int count;
    int capacity;
} stmt_list;

#endif