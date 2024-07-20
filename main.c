#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "repl.h"
#include "token.h"

int main() {
    char* input = "set x = 5; return 10; return 100000;";
    lexer* l = new (input);
    parser* p = p_new(l);
    stmt_list* result = parse_program(p);
    if (p->error_count != 0) {
        for (int i = 0; i < p->error_count; i++) {
            printf("%s\n", p->errors[i]);
        }
    }
    printf("number of statements: %d\n", result->count);

    for (int i = 0; i < result->count; i++) {
        stmt* s = result->statements[i];
        printf("type: %s\n", s->stmt_token->value);
        if (s->type == SET_STMT) {
            printf("ident token type: %d\n", s->data->set->identifier->type);
            printf("ident token value: %s\n", s->data->set->identifier->value);
        } else if (s->type == RET_STMT) {
            // DO SOMETHING
        }
    }
    return 0;
}
