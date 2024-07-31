#include "eval.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "parser.h"
#include "token.h"

object* eval_program(stmt_list* program) {
    object* o;
    for (int i = 0; i < program->count; i++) {
        o = eval_stmt(program->statements[i]);
    }
    return o;
}

object* eval_stmt(stmt* stmt) {
    object* o;
    switch (stmt->type) {
        case EXPR:
            o = eval_expr(stmt->data.expr);
            break;
        default:
            o->to_string = print_null;
            o->type = NULL_OBJ;
            break;
    }
    return o;
}

object* eval_expr(expr* ex) {
    object *obj, *right, *left, *condition;
    switch (ex->type) {
        case LITERAL_EXPR:
            switch (ex->data.lit->type) {
                case INT_LIT:
                    obj = malloc(sizeof(object));
                    obj->type = INT_OBJ;
                    obj->val.num = ex->data.lit->val.num;
                    obj->to_string = print_int;
                    break;
                case BOOL_LIT:
                    obj = malloc(sizeof(object));
                    obj->type = BOOL_OBJ;
                    obj->val.boolean = ex->data.lit->val.boolean;
                    obj->to_string = print_bool;
                    break;
                default:
                    obj = NULL;
                    break;
            }
            break;
        case PREFIX_EXPR:
            right = eval_expr(ex->data.pre->right);
            obj = eval_prefix(ex->data.pre->operator, right);
            break;
        case INFIX_EXPR:
            right = eval_expr(ex->data.inf->right);
            left = eval_expr(ex->data.inf->left);
            obj = eval_infix(ex->data.inf->operator, right, left);
            break;
        case COND_EXPR:
            condition = eval_expr(ex->data.cond->condition);
            if (is_truthy(condition)) {
                obj = eval_program(ex->data.cond->consequence);
            } else if (ex->data.cond->alternative != NULL) {
                obj = eval_program(ex->data.cond->alternative);
            } else {
                obj->to_string = print_null;
                obj->type = NULL_OBJ;
            }
            break;
        default:
            obj->to_string = print_null;
            obj->type = NULL_OBJ;
            break;
    }
    return obj;
}

bool is_truthy(object* obj){
    if(obj == NULL || obj->type == NULL_OBJ){
        return false;
    } 
    if(obj->type == BOOL_OBJ && obj->val.boolean == false){
        return false;
    }
    if(obj->type == INT_OBJ && obj->val.num <= 0){
        return false;
    }
    return true;
}

object* eval_infix(token* operator, object * right, object* left) {
    object* obj;
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
    } else {
        obj->to_string = print_null;
        obj->type = NULL_OBJ;
    }
    return obj;
}

object* eval_integer_infix(token* operator, object * right, object* left) {
    int left_val = left->val.num;
    int right_val = right->val.num;
    object* obj = malloc(sizeof(object));

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
            obj->to_string = print_null;
            obj->type = NULL_OBJ;
            break;
    }
    return obj;
}

object* eval_prefix(token* operator, object * right) {
    object* obj;
    switch (operator->type) {
        case BANG:
            obj = eval_bang(right);
            break;
        case MINUS:
            obj = eval_minus(right);
            break;
        default:
            obj->to_string = print_null;
            obj->type = NULL_OBJ;
            break;
    }
    return obj;
}

object* eval_minus(object* right) {
    object* obj = malloc(sizeof(object));
    if (right->type != INT_OBJ) {
        obj->to_string = print_null;
        obj->type = NULL_OBJ;
    } else {
        obj->type = INT_OBJ;
        obj->to_string = print_int;
        obj->val.num = -1 * right->val.num;
    }
    return obj;
}
object* eval_bang(object* right) {
    object* obj = malloc(sizeof(object));
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

char* print_int(void* o) {
    object* obj = (object*)o;
    char* str = malloc(MAX_STR_LEN + 1);
    sprintf(str, "%d", obj->val.num);
    return str;
}

char* print_bool(void* o) {
    object* obj = (object*)o;
    char* str = malloc(MAX_STR_LEN + 1);
    sprintf(str, "%s", obj->val.boolean ? "true" : "false");
    return str;
}

char* print_null(void* o) {
    char* str = malloc(5);
    strcpy(str, "null");
    return str;
}
