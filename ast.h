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
typedef struct environment environment;

/* enum: all the statement types. statements are the building blocks of the
 * language.*/
typedef enum {
    SET_STMT,
    RET_STMT,
    EXPR_STMT,
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

/* expr_stmt struct contains a pointer to an expression. */
typedef struct {
    expr* value;
} expr_stmt;

/* the stmt_data union contains a pointer to 1 out of the 3 types of
 * statements.*/
typedef union {
    set_stmt* set;
    ret_stmt* ret;
    expr_stmt* expr;
} stmt_data;

/* stmt struct. a stmt has a type, data, and token (first token of the
 * statement).*/
typedef struct {
    stmt_type type;
    stmt_data* data;
    token* stmt_token;
} stmt;

/* a stmt_list has an array of statements, a count, and a capacity. used to
 * store all of the statements from a program.*/
typedef struct stmt_list {
    stmt** statements;
    int count;
    int capacity;
} stmt_list;

/* an expr_pair contains 2 expression (left/right)*/
typedef struct expr_pair {
    expr* first;
    expr* second;
} expr_pair;

/* expr_pairs_list has an array of pairs.*/
typedef struct expr_pair_list {
    expr_pair* expr_pairs;
    int count;
    int capacity;
} expr_pair_list;

/* expr_list has an array of expressions.*/
typedef struct expr_list {
    expr** exprs;
    int count;
    int capacity;
} expr_list;

/* token_list has an array of tokens.*/
typedef struct token_list {
    token** tokens;
    int count;
    int capacity;
} token_list;

/* fn_lit struct. a function literal has the parameters, a list of statements,
 * and an environment. */
struct fn_lit {
    token_list params;
    stmt_list body;
    environment* env;
};

/* the literal_data union stores one of the 7 types of literal data.*/
typedef union {
    int i;
    bool b;
    token* t;
    char* s;
    struct fn_lit fn;
    expr_list arr;
    expr_pair_list map;
} literal_data;

/* literal struct. a literal stores the literal type, and the data*/
typedef struct {
    literal_type type;
    literal_data* data;
} literal;

/* the infix_expr struct stores the operator, and the expression that are on
 * either side.*/
struct infix_expr {
    token* op;
    expr* left;
    expr* right;
};

/* the prefix_expr struct has the operator, and the expression that it
 * precedes.*/
struct prefix_expr {
    token* op;
    expr* right;
};

/* if_expr struct. if_expr has the condition, consequences, and an optional else
 * statements, with consequences.*/
struct if_expr {
    expr* condition;
    stmt_list* consequence;
    bool has_alt;
    stmt_list* alternative;
};

/* call_expr has the function, and a list of the arguments. */
struct call_expr {
    expr* func;
    expr_list* args;
};

/* expr_data contains a pointer to 1 of the 5 types of expressions */
typedef union {
    struct infix_expr* inf;
    struct prefix_expr* pre;
    struct if_expr* ifelse;
    struct call_expr* call;
    literal* lit;
} expr_data;

/* expr struct. an expr contains a type, and data.*/
typedef struct expr {
    expr_type type;
    expr_data* data;
} expr;

#endif