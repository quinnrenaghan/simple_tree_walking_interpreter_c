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
            if(peek_ahead(l) == '='){
                read_char(l);
                my_token->type = EQ;
                my_token->value = malloc(sizeof(char)*2 + 1);
                strcpy(my_token->value, "==");
            } else {
                my_token->type = ASSIGN;
                my_token->value = malloc(sizeof(char) + 1);
                strcpy(my_token->value, "=");
            }
            break;
        case '+':
            my_token->type = PLUS;
            my_token->value = malloc(sizeof(char) + 1);
            strcpy(my_token->value, "+");
            break;
        case '-':
            my_token->type = MINUS;
            my_token->value = malloc(sizeof(char) + 1);
            strcpy(my_token->value, "-");
            break;
        case '!':
            if(peek_ahead(l) == '='){
                read_char(l);
                my_token->type = NOT_EQ;
                my_token->value = malloc(sizeof(char)*2 + 1);
                strcpy(my_token->value, "!=");
            } else {
                my_token->type = BANG;
                my_token->value = malloc(sizeof(char) + 1);
                strcpy(my_token->value, "!");
            }
            break;
        case '/':
            my_token->type = SLASH;
            my_token->value = malloc(sizeof(char) + 1);
            strcpy(my_token->value, "/");
            break;
        case '*':
            my_token->type = ASTERISK;
            my_token->value = malloc(sizeof(char) + 1);
            strcpy(my_token->value, "*");
            break;
        case '<':
            my_token->type = LT;
            my_token->value = malloc(sizeof(char) + 1);
            strcpy(my_token->value, "<");
            break;
        case '>':
            my_token->type = GT;
            my_token->value = malloc(sizeof(char) + 1);
            strcpy(my_token->value, ">");
            break;
        case ',':
            my_token->type = COMMA;
            my_token->value = malloc(sizeof(char) + 1);
            strcpy(my_token->value, ",");
            break;
        case ';':
            my_token->type = SEMICOLON;
            my_token->value = malloc(sizeof(char) + 1);
            strcpy(my_token->value, ";");
            break;
        case '(':
            my_token->type = LPAREN;
            my_token->value = malloc(sizeof(char) + 1);
            strcpy(my_token->value, "(");
            break;
        case ')':  
            my_token->type = RPAREN;
            my_token->value = malloc(sizeof(char) + 1);
            strcpy(my_token->value, ")");
            break;
        case '{':
            my_token->type = LBRACE;
            my_token->value = malloc(sizeof(char) + 1);
            strcpy(my_token->value, "{");
            break;
        case '}':
            my_token->type = RBRACE;
            my_token->value = malloc(sizeof(char) + 1);
            strcpy(my_token->value, "}");
            break;
        case '\0':
            my_token->type = EOF_TOKEN;
            my_token->value = malloc(sizeof(char) + 1);
            strcpy(my_token->value, "");
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

char peek_ahead(lexer *l){
    if(l->read_position >= strlen(l->input)){
        return '\0';
    } else {
        return l->input[l->read_position];
    }
}

int main(){
    char* input = "let five = 5; \n let ten = 10; \n let add = fn(x, y) { \n x + y;};  \n let result = add(five, ten); !-/*5; 5 < 10 > 5; \n if (5 < 10) {  return true; \n } else { return false;}     10 == 10; 10 != 9;";
    lexer *l = new(input);
    token *t = next_token(l);


    while(t->type != 17){
        printf("type: %d value: %s\n", t->type, t->value);
        t = next_token(l);
    }
    return 0;
}