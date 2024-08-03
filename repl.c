#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "ast.h"
#include "parser.h"
#include "eval.h"

/* begins the read, evaluate, print, and loop.*/
int start(FILE* in, FILE* out) {
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
    return 0;
}