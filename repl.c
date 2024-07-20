#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "token.h"

int start(FILE* in, FILE* out) {
    char line[528];
    fprintf(out, "$");
    while (fgets(line, sizeof(line), in) != NULL) {
        lexer* l = new (line);
        parser* p = p_new(l);
        stmt_list* result = parse_program(p);
        for (int i = 0; i < result->count; i++) {
            stmt* s = result->statements[i];
            printf("type: %d\n", s->type);
            printf("identifier token type: %d\n", s->data->set->identifier->type);
            printf("identifier token value: %s\n",
                   s->data->set->identifier->value);
        }
        fprintf(out, "$");
    }
    return 0;
}