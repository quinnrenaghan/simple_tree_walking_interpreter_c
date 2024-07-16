#include "lexer.h"
#include "token.h"
#include "repl.h"
#include <stdio.h>
#include <stdlib.h>


int main(){
    printf("Q Programming Language Loaded! Enter Commands.\n");
    start(stdin, stdout);
    return 0;
}