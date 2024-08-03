#ifndef EVAL_H
#define EVAL_H

#include <stdbool.h>
#include "ast.h"

typedef struct object object;
typedef struct environment environment;
typedef char *(*to_string_func)(object*);

typedef enum {
    INT_OBJ,
    BOOL_OBJ,
    NULL_OBJ,
    RETURN_OBJ,
    ERROR_OBJ,
    FUNCTION_OBJ,
    STRING_OBJ
} object_type;

typedef struct{
    token *identifier;
    object *value;
}binding;

typedef struct environment{
    binding **bindings;
    int count;
    int capacity;
    environment *outer;
}environment;

typedef struct{
    stmt_list *body;
    environment *env;
    token_list *params;
}function;

typedef struct object{
    object_type type;
    union {
        int num;
        bool boolean;
        object *return_value;
        char *error_message;
        function *function;
        char *string;
    } val;
    to_string_func to_string;
} object;

typedef struct{
    object **objects;
    int count;
    int capacity;
}object_list;


object *eval_program(stmt_list *program, environment *env);
object *eval_stmt(stmt *stmt, environment *env);
object *eval_set(set_stmt *set, environment *env);
object *eval_ret(ret_stmt *ret, environment *env);
object *eval_expr(expr *ex, environment *env);
object_list *eval_args(expr_list *args, environment *env);
object *apply_function(object *func, object_list*args);
object *eval_infix(token *operator, object *right, object *left);
object *eval_integer_infix(token *operator, object *right, object *left);
object *eval_prefix(token *operator, object *right);
object *eval_minus(object *right);
object *eval_bang(object *right);

bool is_truthy(object *obj);
char *print_bool(object *o);
char *print_int(object *o);
char *print_null(object  *o);
char *print_ret(object *obj);
char *print_error(object *obj);
char *print_string(object *obj);
bool is_error(object *obj);

object *env_get(token *ident, environment *env);
object *env_set(token *ident, object *obj, environment *env);

#endif