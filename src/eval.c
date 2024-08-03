#include "eval.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "parser.h"
#include "token.h"

object *eval_program(stmt_list *program, environment *env) {
    object *obj;
    for (int i = 0; i < program->count; i++) {
        obj = eval_stmt(program->statements[i], env);

        if (obj->type == RETURN_OBJ || obj->type == ERROR_OBJ) {
            return obj;
        }
    }
    return obj;
}

object *eval_stmt(stmt *stmt, environment *env) {
    object *obj;
    switch (stmt->type) {
        case EXPR:
            obj = eval_expr(stmt->data.expr, env);
            break;
        case RET_STMT:
            obj = eval_ret(stmt->data.ret, env);
            break;
        case SET_STMT:
            obj = eval_set(stmt->data.set, env);
            break;
        default:
            obj->to_string = print_null;
            obj->type = NULL_OBJ;
            break;
    }
    release_stmt(stmt);
    return obj;
}

object *eval_set(set_stmt *set, environment *env) {
    object *value = eval_expr(set->value, env);
    if (is_error(value)) {
        return value;
    }
    env_set(set->identifier, value, env);
    return value;
}

object *eval_ret(ret_stmt *ret, environment *env) {
    object *value = eval_expr(ret->value, env);
    if (is_error(value)) {
        return value;
    }
    object *obj = malloc(sizeof(object));
    obj->type = RETURN_OBJ;
    obj->val.return_value = value;
    obj->to_string = print_ret;
    return obj;
}

object *eval_expr(expr *ex, environment *env) {
    object *obj, *right, *left, *condition, *func;
    literal *l;
    call_expr *c;
    switch (ex->type) {
        case LITERAL_EXPR:
            l = ex->data.lit;
            switch (l->type) {
                case INT_LIT:
                    obj = malloc(sizeof(object));
                    obj->type = INT_OBJ;
                    obj->val.num = l->val.num;
                    obj->to_string = print_int;
                    break;
                case BOOL_LIT:
                    obj = malloc(sizeof(object));
                    obj->type = BOOL_OBJ;
                    obj->val.boolean = l->val.boolean;
                    obj->to_string = print_bool;
                    break;
                case IDENT_LIT:
                    obj = env_get(l->val.token, env);
                    if (obj == NULL) {
                        obj = malloc(sizeof(object));
                        obj->to_string = print_error;
                        obj->type = ERROR_OBJ;
                        obj->val.error_message = malloc(MAX_STR_LEN + 1);
                        strcpy(obj->val.error_message, "undefined identifier");
                    }
                    break;
                case FN_LIT:
                    obj = malloc(sizeof(object));
                    obj->type = FUNCTION_OBJ;
                    fn *fn_literal = l->val.function;
                    obj->val.function = malloc(sizeof(function));

                    obj->val.function->body = fn_literal->body;
                    obj->val.function->params = fn_literal->params;
                    obj->val.function->env = env;
                    obj->to_string = NULL;
                    break;
                case STRING_LIT:
                    obj = malloc(sizeof(object));
                    obj->type = STRING_OBJ;
                    obj->val.string = l->val.string;
                    obj->to_string = print_string;
                    break;
                default:
                    obj = NULL;
                    break;
            }
            break;
        case PREFIX_EXPR:
            right = eval_expr(ex->data.pre->right, env);
            if (is_error(right)) {
                return right;
            }
            obj = eval_prefix(ex->data.pre->operator, right);
            break;
        case INFIX_EXPR:
            right = eval_expr(ex->data.inf->right, env);
            if (is_error(right)) {
                return right;
            }
            left = eval_expr(ex->data.inf->left, env);
            if (is_error(left)) {
                return left;
            }
            obj = eval_infix(ex->data.inf->operator, right, left);
            break;
        case COND_EXPR:
            condition = eval_expr(ex->data.cond->condition, env);
            if (is_error(condition)) {
                return condition;
            }
            if (is_truthy(condition)) {
                obj = eval_program(ex->data.cond->consequence, env);
            } else if (ex->data.cond->alternative != NULL) {
                obj = eval_program(ex->data.cond->alternative, env);
            } else {
                obj = malloc(sizeof(object));
                obj->to_string = print_null;
                obj->type = NULL_OBJ;
            }
            break;
        case CALL_EXPR:
            c = ex->data.call;
            func = eval_expr(c->fn, env);
            if (is_error(func)) {
                return func;
            }
            object_list *args = eval_args(c->args, env);
            if (args->count == 1 && is_error(args->objects[0])) {
                return args->objects[0];
            }
            obj = apply_function(func, args);
            break;
        default:
            obj = malloc(sizeof(object));
            obj->to_string = print_null;
            obj->type = NULL_OBJ;
            break;
    }
    return obj;
}

object *apply_function(object *func, object_list *args) {
    environment *extended_env = malloc(sizeof(environment));

    function *f = func->val.function;
    extended_env->outer = f->env;
    extended_env->count = 0;
    extended_env->capacity = MAX_ENV_SIZE;
    extended_env->bindings = malloc(sizeof(binding *) * extended_env->capacity);

    for (int i = 0; i < f->params->count; i++) {
        env_set(f->params->tokens[i], args->objects[i], extended_env);
    }

    object *evaluated = eval_program(f->body, extended_env);
    if (evaluated->type == RETURN_OBJ) {
        return evaluated->val.return_value;
    }
    return evaluated;
}

object_list *eval_args(expr_list *args, environment *env) {
    object_list *list = malloc(sizeof(object_list));
    list->count = 0;
    list->capacity = MAX_PARAMS;
    list->objects = malloc(sizeof(object *) * list->capacity);

    for (int i = 0; i < args->count; i++) {
        object *obj = eval_expr(args->exprs[i], env);
        if (is_error(obj)) {
            for (int j = 0; j < i; j++) {
                free(list->objects[j]);
                list->objects[0] = obj;
                list->count = 1;
                return list;
            }
        }
        list->objects[list->count] = obj;
        (list->count)++;
    }
    return list;
}

object *eval_infix(token *operator, object * right, object *left) {
    object *obj;
    if (left->type == INT_OBJ && right->type == INT_OBJ) {
        obj = eval_integer_infix(operator, right, left);
    } else if (operator->type == EQ && right->type == BOOL_OBJ &&
               left->type == BOOL_OBJ) {
        obj = malloc(sizeof(object));
        obj->type = BOOL_OBJ;
        obj->to_string = print_bool;
        obj->val.boolean = (left->val.boolean == right->val.boolean);
    } else if (operator->type == NOT_EQ && right->type == BOOL_OBJ &&
               left->type == BOOL_OBJ) {
        obj = malloc(sizeof(object));
        obj->type = BOOL_OBJ;
        obj->to_string = print_bool;
        obj->val.boolean = (left->val.boolean != right->val.boolean);
    } else if (operator->type == PLUS && right->type == STRING_OBJ &&
               left->type == STRING_OBJ) {
        obj = malloc(sizeof(object));
        obj->type = STRING_OBJ;
        obj->to_string = print_string;
        obj->val.string = malloc(MAX_STR_LEN + 1);
        strcat(obj->val.string, left->val.string);
        strcat(obj->val.string, right->val.string);
    } else {
        obj = malloc(sizeof(object));
        obj->to_string = print_error;
        obj->type = ERROR_OBJ;
        obj->val.error_message = malloc(MAX_STR_LEN + 1);
        strcpy(obj->val.error_message, "type mismatch for infix operator.");
    }
    return obj;
}

object *eval_integer_infix(token *operator, object * right, object *left) {
    int left_val = left->val.num;
    int right_val = right->val.num;
    object *obj = malloc(sizeof(object));

    switch (operator->type) {
        case PLUS:
            obj->type = INT_OBJ;
            obj->to_string = print_int;
            obj->val.num = left_val + right_val;
            break;
        case MINUS:
            obj->type = INT_OBJ;
            obj->to_string = print_int;
            obj->val.num = left_val - right_val;
            break;
        case ASTERISK:
            obj->type = INT_OBJ;
            obj->to_string = print_int;
            obj->val.num = left_val * right_val;
            break;
        case SLASH:
            obj->type = INT_OBJ;
            obj->to_string = print_int;
            obj->val.num = left_val / right_val;
            break;
        case LT:
            obj->type = BOOL_OBJ;
            obj->to_string = print_bool;
            obj->val.boolean = left_val < right_val;
            break;
        case GT:
            obj->type = BOOL_OBJ;
            obj->to_string = print_bool;
            obj->val.boolean = left_val > right_val;
            break;
        case EQ:
            obj->type = BOOL_OBJ;
            obj->to_string = print_bool;
            obj->val.boolean = left_val == right_val;
            break;
        case NOT_EQ:
            obj->type = BOOL_OBJ;
            obj->to_string = print_bool;
            obj->val.boolean = left_val != right_val;
            break;
        default:
            obj = NULL;
            break;
    }
    return obj;
}

object *eval_prefix(token *operator, object * right) {
    object *obj;
    switch (operator->type) {
        case BANG:
            obj = eval_bang(right);
            break;
        case MINUS:
            obj = eval_minus(right);
            break;
        default:
            obj = NULL;
            break;
    }
    return obj;
}

object *eval_minus(object *right) {
    object *obj = malloc(sizeof(object));
    if (right->type != INT_OBJ) {
        obj->to_string = print_error;
        obj->type = ERROR_OBJ;
        obj->val.error_message = malloc(MAX_STR_LEN + 1);
        strcpy(obj->val.error_message, "invalid operand for - operator");
    } else {
        obj->type = INT_OBJ;
        obj->to_string = print_int;
        obj->val.num = -1 * right->val.num;
    }
    return obj;
}
object *eval_bang(object *right) {
    object *obj = malloc(sizeof(object));
    obj->type = BOOL_OBJ;
    obj->to_string = print_bool;
    switch (right->type) {
        case BOOL_OBJ:
            if (right->val.boolean == true) {
                obj->val.boolean = false;
            } else {
                obj->val.boolean = true;
            }
            break;
        case NULL_OBJ:
            obj->val.boolean = true;
            break;
        case INT_OBJ:
            obj->val.boolean = right->val.num > 0;
            break;
        default:
            obj->val.boolean = false;
            break;
    }
    return obj;
}

char *print_int(object *obj) {
    char *str = malloc(MAX_STR_LEN + 1);
    sprintf(str, "%d", obj->val.num);
    return str;
}

char *print_bool(object *obj) {
    char *str = malloc(MAX_STR_LEN + 1);
    sprintf(str, "%s", obj->val.boolean ? "true" : "false");
    return str;
}

char *print_null(object *obj) {
    char *str = malloc(5);
    strcpy(str, "null");
    return str;
}

char *print_ret(object *obj) {
    switch (obj->val.return_value->type) {
        case INT_OBJ:
            return print_int(obj->val.return_value);
        case BOOL_OBJ:
            return print_bool(obj->val.return_value);
        case ERROR_OBJ:
            return print_error(obj->val.return_value);
        default:
            return print_null(obj->val.return_value);
    }
}

char *print_error(object *obj) {
    char *str = malloc(MAX_STR_LEN + 1);
    sprintf(str, "ERROR: %s", obj->val.error_message);
    return str;
}

char *print_string(object *obj) { return obj->val.string; }

bool is_truthy(object *obj) {
    if (obj == NULL || obj->type == NULL_OBJ) {
        return false;
    }
    if (obj->type == BOOL_OBJ && obj->val.boolean == false) {
        return false;
    }
    if (obj->type == INT_OBJ && obj->val.num <= 0) {
        return false;
    }
    return true;
}

bool is_error(object *obj) {
    if (obj != NULL) {
        return obj->type == ERROR_OBJ;
    }
    return false;
}

object *env_get(token *ident, environment *env) {
    for (int i = 0; i < env->count; i++) {
        if (!strcmp(env->bindings[i]->identifier->value, ident->value)) {
            return env->bindings[i]->value;
        }
    }
    if (env->outer != NULL) {
        object *result = env_get(ident, env->outer);
        if (result != NULL) {
            return result;
        }
    }
    return NULL;
}

object *env_set(token *ident, object *obj, environment *env) {
    for (int i = 0; i < env->count; i++) {
        if (!strcmp(env->bindings[i]->identifier->value, ident->value)) {
            env->bindings[i]->value = obj;
            return obj;
        }
    }

    env->bindings[env->count] = malloc(sizeof(binding));
    env->bindings[env->count]->identifier = ident;
    retain_token(ident);

    env->bindings[env->count]->value = obj;
    env->count++;

    return obj;
}
