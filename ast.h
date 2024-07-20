#ifndef AST_H
#define AST_H

#include "token.h"

typedef struct expr expr;

typedef enum {
    SET_STMT,
    // add other statement types
} stmt_type;

typedef struct {
    token* identifier;
    expr* value;
} set_stmt;

typedef union {
    set_stmt* set;
    // add other statement types
} stmt_data;

typedef struct {
    stmt_type type;
    stmt_data* data;
} stmt;

typedef struct stmt_list {
    stmt** statements;
    int count;
    int capacity;
} stmt_list;

#endif