#ifndef EVAL_H
#define EVAL_H

#include <stdbool.h>
#include "ast.h"

typedef char* (*to_string_func)(void*);

typedef enum {
    INT_OBJ,
    BOOL_OBJ,
    NULL_OBJ,
} object_type;

typedef struct{
    object_type type;
    union {
        int num;
        bool boolean;
    } val;
    to_string_func to_string;
} object;

object* eval_program(stmt_list* program);
object* eval_stmt(stmt* stmt);
object* eval_expr(expr* ex);
object* eval_infix(token* operator, object* right, object* left);
object* eval_integer_infix(token* operator, object* right, object *left);
object* eval_prefix(token* operator, object* right);
object* eval_minus(object* right);
object* eval_bang(object* right);

bool is_truthy(object* obj);
char* print_bool(void* o);
char* print_int(void* o);
char* print_null(void * o);

#endif