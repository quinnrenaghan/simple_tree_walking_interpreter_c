#include "lexer.h"
#include "token.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

lexer* new(char* input){
    lexer *l = malloc(sizeof(lexer));
    l->input = input;
    read_char(l);
    return l;
}

void read_char(lexer *l){
    if(l->read_position >= strlen(l->input)){
        l->ch = '\0';
    } else {
        l->ch = l->input[l->read_position];
    }
    l->position = l->read_position;
    l->read_position++;
}

token* next_token(lexer *l){
    token *my_token = malloc(sizeof(token));
    eat_whitespace(l);
    switch(l->ch){
        case '=':
            my_token->type = ASSIGN;
            my_token->value = "=";
            break;
        case '+':
            my_token->type = PLUS;
            my_token->value = "+";
            break;
        case '(':
            my_token->type = LPAREN;
            my_token->value = "(";
            break;
        case ')':  
            my_token->type = RPAREN;
            my_token->value = ")";
            break;
        case '{':
            my_token->type = LBRACE;
            my_token->value = "{";
            break;
        case '}':
            my_token->type = RBRACE;
            my_token->value = "}";
            break;
        case ',':
            my_token->type = COMMA;
            my_token->value = ",";
            break;
        case ';':
            my_token->type = SEMICOLON;
            my_token->value = ";";
            break;
        case '\0':
            my_token->type = EOF_TOKEN;
            my_token->value = "";
            break;
        default:
            if(is_letter(l->ch)){
                my_token->value = read_identifier(l);
                my_token->type = lookup_identifier(my_token->value);
                return my_token;
            } else if(is_digit(l->ch)){
                my_token->type = INT;
                my_token->value = read_int(l);
                return my_token;
            } else {
                my_token->type = ILLEGAL;
                my_token->value = &(l->ch);
            }
    }
    read_char(l);
    return my_token;
}

char* read_identifier(lexer *l){
    char* identifier;
    int start_pos = l->position;
    while(is_letter(l->ch)){
        read_char(l);
    }

    identifier = malloc(l->position - start_pos + 1);
    strncpy(identifier, l->input + start_pos, l->position - start_pos);
    identifier[l->position - start_pos + 1] = '\0';

    return identifier;
}

char* read_int(lexer *l){
    char* identifier;
    int start_pos = l->position;
    while(is_digit(l->ch)){
        read_char(l);
    }

    identifier = malloc(l->position - start_pos + 1);
    strncpy(identifier, l->input + start_pos, l->position - start_pos);
    identifier[l->position - start_pos + 1] = '\0';

    return identifier;
}

int is_letter(char c){
    return 'a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || c == '_';
}

int is_digit(char c){
    return '0' <= c && c <= '9';
}

void eat_whitespace(lexer *l){
    while(l->ch == ' ' || l->ch == '\t' || l->ch == '\n' || l->ch == '\r'){
        read_char(l);
    }

}

int main(){
    char* input = "let five=5";
    lexer *l = new(input);
    token *t1 = next_token(l);
    token *t2 = next_token(l);
    token *t3 = next_token(l);
    token *t4 = next_token(l);
    printf("type: %d value: %s\n", t1->type, t1->value);
    printf("type: %d value: %s\n", t2->type, t2->value);
    printf("type: %d value: %s\n", t3->type, t3->value);
    printf("type: %d value: %s\n", t4->type, t4->value);
    return 0;
}