#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "eval.h"
#include "lexer.h"
#include "parser.h"
#include "repl.h"
#include "token.h"

/*
void print_set_statement(set_stmt* set, int indent);
void print_ret_statement(ret_stmt* ret, int indent);
void print_statement(stmt* s, int indent);
void print_identifier(token* t, int indent);

void print_expression(expr* ex, int indent) {
    for (int i = 0; i < indent; i++) {
        printf("\t");
    }
    printf("EXPRESSION FOUND\n");
    if (ex->type == LITERAL_EXPR) {
        literal* lit = ex->data.lit;
        for (int i = 0; i < indent + 1; i++) {
            printf("\t");
        }
        printf("LITERAL FOUND\n");

        for (int i = 0; i < indent + 2; i++) {
            printf("\t");
        }
        printf("LITERAL TYPE: %d\n", lit->type);

        for (int i = 0; i < indent + 2; i++) {
            printf("\t");
        }
        if (lit->type == IDENT_LIT) {
            printf("LITERAL DATA: %s\n", lit->val.token->value);
        } else if (lit->type == INT_LIT) {
            printf("LITERAL DATA: %d\n", lit->val.num);
        } else if (lit->type == BOOL_LIT) {
            printf("LITERAL DATA: %d\n", lit->val.boolean);
        } else if (lit->type == FN_LIT) {
            printf("FUNCTION FOUND\n");
            for (int i = 0; i < indent + 2; i++) {
                printf("\t");
            }
            printf("PARAMS:\n");
            for (int i = 0; i < lit->val.function->params->count; i++) {
                print_identifier(lit->val.function->params->tokens[i],
                                 indent + 3);
            }
            for (int i = 0; i < indent + 2; i++) {
                printf("\t");
            }
            printf("BODY:\n");
            for (int i = 0; i < lit->val.function->body->count; i++) {
                print_statement(lit->val.function->body->statements[i],
                                indent + 3);
            }
        }
    } else if (ex->type == PREFIX_EXPR) {
        prefix_expr* pre = ex->data.pre;
        for (int i = 0; i < indent + 1; i++) {
            printf("\t");
        }
        printf("PREFIX FOUND\n");

        for (int i = 0; i < indent + 2; i++) {
            printf("\t");
        }
        printf("OPERATOR: %s\n", pre->operator->value);

        for (int i = 0; i < indent + 2; i++) {
            printf("\t");
        }
        printf("RIGHT:\n");
        print_expression(pre->right, indent + 2);

    } else if (ex->type == INFIX_EXPR) {
        infix_expr* inf = ex->data.inf;

        for (int i = 0; i < indent + 1; i++) {
            printf("\t");
        }
        printf("INFIX FOUND\n");
        for (int i = 0; i < indent + 2; i++) {
            printf("\t");
        }
        printf("OPERATOR: %s\n", inf->operator->value);

        for (int i = 0; i < indent + 2; i++) {
            printf("\t");
        }
        printf("LEFT:\n");
        print_expression(inf->left, indent + 2);

        for (int i = 0; i < indent + 2; i++) {
            printf("\t");
        }

        printf("RIGHT:\n");
        print_expression(inf->right, indent + 2);
    } else if (ex->type == COND_EXPR) {
        cond_expr* cond = ex->data.cond;
        for (int i = 0; i < indent + 1; i++) {
            printf("\t");
        }
        printf("CONDITIONAL FOUND\n");
        for (int i = 0; i < indent + 2; i++) {
            printf("\t");
        }
        printf("CONDITION:\n");
        print_expression(cond->condition, indent + 2);

        for (int i = 0; i < indent + 2; i++) {
            printf("\t");
        }
        printf("CONSEQUENCE:\n");

        for (int i = 0; i < cond->consequence->count; i++) {
            stmt* s = cond->consequence->statements[i];
            print_statement(s, indent + 3);
        }
        if (cond->alternative != NULL) {
            for (int i = 0; i < indent + 2; i++) {
                printf("\t");
            }
            printf("ALTERNATIVE:\n");
            for (int i = 0; i < cond->alternative->count; i++) {
                stmt* s = cond->alternative->statements[i];
                print_statement(s, indent + 3);
            }
        }
    } else if (ex->type == CALL_EXPR) {
        call_expr* call = ex->data.call;
        for (int i = 0; i < indent + 1; i++) {
            printf("\t");
        }
        printf("CALL FOUND\n");
        for (int i = 0; i < indent + 2; i++) {
            printf("\t");
        }

        printf("FUNCTION:\n");
        print_expression(call->fn, indent + 2);

        for (int i = 0; i < indent + 2; i++) {
            printf("\t");
        }
        printf("ARGS:\n");
        for (int i = 0; i < call->args->count; i++) {
            expr *e = call->args->exprs[i];
            print_expression(e, indent + 3);
        }
    }
}

void print_identifier(token* t, int indent) {
    for (int i = 0; i < indent; i++) {
        printf("\t");
    }
    printf("TOKEN TYPE: %d\n", t->type);
    for (int i = 0; i < indent; i++) {
        printf("\t");
    }
    printf("TOKEN VALUE: %s\n", t->value);
}

void print_set_statement(set_stmt* set, int indent) {
    for (int i = 0; i < indent; i++) {
        printf("\t");
    }
    printf("SET STATEMENT FOUND:\n");

    for (int i = 0; i < indent + 1; i++) {
        printf("\t");
    }
    printf("IDENTIFIER VALUE: %s\n", set->identifier->value);

    for (int i = 0; i < indent + 1; i++) {
        printf("\t");
    }
    printf("IDENTIFIER TYPE: %d\n", set->identifier->type);

    for (int i = 0; i < indent + 1; i++) {
        printf("\t");
    }

    printf("VALUE:\n");
    print_expression(set->value, indent + 1);
}

void print_ret_statement(ret_stmt* ret, int indent) {
    for (int i = 0; i < indent; i++) {
        printf("\t");
    }
    printf("RET STATEMENT FOUND.\n");
    for (int i = 0; i < indent + 1; i++) {
        printf("\t");
    }
    printf("VALUE:\n");
    print_expression(ret->value, indent + 1);
}

void print_statement(stmt* s, int indent) {
    if (s->type == SET_STMT) {
        print_set_statement(s->data.set, indent);
    } else if (s->type == RET_STMT) {
        print_ret_statement(s->data.ret, indent);
    } else if (s->type == EXPR) {
        print_expression(s->data.expr, indent);
    }
}

int main() {
    char* input = malloc(MAX_STR_LEN + 1);
    printf("$ ");
    while (fgets(input, MAX_STR_LEN, stdin)) {
        lexer* l = new (input);
        parser* p = p_new(l);
        stmt_list* result = parse_program(p);
        if (p->error_count != 0) {
            for (int i = 0; i < p->error_count; i++) {
                printf("%s\n", p->errors[i]);
            }
        } else {
            for (int i = 0; i < result->count; i++) {
                stmt* s = result->statements[i];
                if (s->type == SET_STMT) {
                    set_stmt* set = s->data.set;
                    print_set_statement(set, 0);
                } else if (s->type == RET_STMT) {
                    ret_stmt* ret = s->data.ret;
                    print_ret_statement(ret, 0);
                } else {
                    expr* expr = s->data.expr;
                    print_expression(expr, 0);
                }
            }
        }
        printf("$ ");
    }
    return 0;
} */

int main() {
    char* input = malloc(MAX_STR_LEN + 1);
    printf("$ ");

    environment* env = malloc(sizeof(environment));
    env->capacity = MAX_ENV_SIZE;
    env->count = 0;
    env->bindings = malloc(sizeof(binding*) * MAX_ENV_SIZE);
    env->outer = NULL;

    while (fgets(input, MAX_STR_LEN, stdin)) {
        lexer* l = new (input);
        parser* p = p_new(l);
        stmt_list* result = parse_program(p);
        if (p->error_count != 0) {
            for (int i = 0; i < p->error_count; i++) {
                printf("%s\n", p->errors[i]);
            }
        } else {
            object* evaluated = eval_program(result, env);
            if (evaluated != NULL) {
                printf("%s\n", evaluated->to_string(evaluated));
            }
        }

        release_token(p->curr_token);
        release_token(p->peek_token);
        printf("$ ");
    }
    printf("REPORT:\nTOKEN CREATED: %d\nTOKEN FREED: %d\n", token_created, token_freed);
    return 0;
}