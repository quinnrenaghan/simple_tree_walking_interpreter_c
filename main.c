#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "repl.h"
#include "token.h"
void print_set_statement(set_stmt* set, int indent);
void print_ret_statement(ret_stmt* ret, int indent);

void print_expression(expr* expr, int indent) {
    for (int i = 0; i < indent; i++) {
        printf("\t");
    }
    printf("EXPRESSION FOUND\n");
    if (expr->type == LITERAL_EXPR) {
        literal* lit = expr->data.lit;
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
        }
    } else if (expr->type == PREFIX_EXPR) {
        prefix_expr* pre = expr->data.pre;
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

    } else if (expr->type == INFIX_EXPR) {
        infix_expr* inf = expr->data.inf;

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
    } else if (expr->type == COND_EXPR) {
        cond_expr* cond = expr->data.cond;
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
            if (s->type == SET_STMT) {
                print_set_statement(s->data.set, indent + 3);
            } else if (s->type == RET_STMT) {
                print_ret_statement(s->data.ret, indent + 3);
            } else if (s->type == EXPR) {
                print_expression(s->data.expr, indent + 3);
            }
        }
        if (cond->has_else) {
            for (int i = 0; i < indent + 2; i++) {
                printf("\t");
            }
            printf("ALTERNATIVE:\n");
            for (int i = 0; i < cond->alternative->count; i++) {
                stmt* s = cond->alternative->statements[i];
                if (s->type == SET_STMT) {
                    print_set_statement(s->data.set, indent + 3);
                } else if (s->type == RET_STMT) {
                    print_ret_statement(s->data.ret, indent + 3);
                } else if (s->type == EXPR) {
                    print_expression(s->data.expr, indent + 3);
                }
            }
        }
    }
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
}

void print_ret_statement(ret_stmt* ret, int indent) {
    for (int i = 0; i < indent; i++) {
        printf("\t");
    }
    printf("RET STATEMENT FOUND.\n");
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
}
