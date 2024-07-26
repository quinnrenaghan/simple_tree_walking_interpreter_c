#ifndef AST_H
#define AST_H

#include <stdbool.h>

#include "token.h"

/* enum: all the literal types.*/
typedef enum {
    INT_LIT,
    STRING_LIT,
    BOOL_LIT,
    IDENT_LIT,
    FN_LIT,
    ARRAY_LIT,
    MAP_LIT,
    NULL_LIT,
} literal_type;

/* enum: the order of precedence in Q.*/
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

/* enum: all the expression types.*/
typedef enum {
    INFIX_EXPR,
    PREFIX_EXPR,
    LITERAL_EXPR,
    IF_EXPR,
    CALL_EXPR
} expr_type;

typedef struct expr expr;

/* enum: all the statement types. statements are the building blocks of the
 * language.*/
typedef enum {
    SET_STMT,
    RET_STMT,
    EXPR,
} stmt_type;

/* a set statement contains an identifier (i.e. x), and an expression value
 * (i.e. y + 20).*/
typedef struct {
    token* identifier;
    expr* value;
} set_stmt;

/* ret_stmt struct. a return statement contains an expression representing what
 * will be returned.*/
typedef struct {
    expr* value;
} ret_stmt;

/* stmt struct. a stmt has a type, data, and token (first token of the
 * statement).*/
typedef struct {
    stmt_type type;
    token* token;
    union {
        set_stmt* set;
        ret_stmt* ret;
        expr* expr;
    } data;
} stmt;

/* a stmt_list has an array of statements, a count, and a capacity. used to
 * store all of the statements from a program.*/
typedef struct stmt_list {
    stmt** statements;
    int count;
    int capacity;
} stmt_list;

/* literal struct. a literal stores the literal type, and the value*/
typedef struct {
    literal_type type;
    union {
        int num;
        token* token;
    } val;
} literal;

/* the prefix_expr struct has the operator, and the expression that it
 * precedes.*/
typedef struct {
    token* operator;
    expr* right;
} prefix_expr;

/* expr struct. an expr contains a type, and data.*/
typedef struct expr {
    expr_type type;
    union {
        literal* lit;
        prefix_expr *pre;
    } data;
} expr;


#endif