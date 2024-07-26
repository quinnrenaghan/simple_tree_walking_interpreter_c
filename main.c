#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "repl.h"
#include "token.h"

void print_expression(expr* expr) {
    printf("EXPRESSION FOUND\n");
    if (expr->type == LITERAL_EXPR) {
        literal* lit = expr->data.lit;
        printf("\tLITERAL FOUND\n");
        printf("\t\tLITERAL TYPE: %d\n", lit->type);
        if (lit->type == IDENT_LIT) {
            printf("\t\tLITERAL DATA: %s\n", lit->val.token->value);
        } else if (lit->type == INT_LIT) {
            printf("\t\tLITERAL DATA: %d\n", lit->val.num);
        }
    } else if (expr->type == PREFIX_EXPR) {
        prefix_expr* pre = expr->data.pre;
        printf("\tPREFIX FOUND\n");
        printf("\tOPERATOR: %s\n", pre->operator->value);
        printf("RIGHT: ");
        print_expression(pre->right);
    }
}

void print_set_statement(set_stmt* set) {
    printf("SET STATEMENT FOUND.\n");
    printf("\tIDENTIFIER VALUE: %s\n", set->identifier->value);
    printf("\tIDENTIFIER TYPE: %d\n", set->identifier->type);
}

void print_ret_statement(ret_stmt* ret) { printf("RET STATEMENT FOUND.\n"); }

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
                    print_set_statement(set);
                } else if (s->type == RET_STMT) {
                    ret_stmt* ret = s->data.ret;
                    print_ret_statement(ret);
                } else {
                    expr* expr = s->data.expr;
                    print_expression(expr);
                }
            }
        }
        printf("$ ");
    }
    return 0;
}
