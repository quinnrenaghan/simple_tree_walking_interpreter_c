#include "lexer.h"
#include "token.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* creates a new lexer, which will convert the given string.*/
lexer *new(char *input) {
    lexer *l = malloc(sizeof(lexer));
    l->input = input;
    read_char(l);
    return l;
}

/* advances the lexer positions, and updates the current character, accounting
 * for  the case that it reaches the end*/
void read_char(lexer *l) {
    if (l->read_position >= strlen(l->input)) {
        l->ch = '\0';
    } else {
        l->ch = l->input[l->read_position];
    }
    l->position = l->read_position;
    l->read_position++;
}

/* returns the token associated with the character that the lexer currently sits
 * on.*/
token *next_token(lexer *l) {
    token *my_token = malloc(sizeof(token));
    eat_whitespace(l);
    switch (l->ch) {
        // check for 2 character token.
        case '=':
            if (peek_ahead(l) == '=') {
                read_char(l);
                my_token->type = EQ;
                my_token->value = malloc(sizeof(char) * 2 + 1);
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
        // check for 2 character token.
        case '!':
            if (peek_ahead(l) == '=') {
                read_char(l);
                my_token->type = NOT_EQ;
                my_token->value = malloc(sizeof(char) * 2 + 1);
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
            // string found. check for keyword.
            if (is_letter(l->ch)) {
                my_token->value = read_identifier(l);
                my_token->type = lookup_identifier(my_token->value);
                // number found.
            } else if (is_digit(l->ch)) {
                my_token->value = read_int(l);
                my_token->type = INT;
                // anything else: ILLEGAL
            } else {
                my_token->type = ILLEGAL;
                my_token->value = malloc(sizeof(char) + 1);
                strcpy(my_token->value, &(l->ch));
            }
    }
    read_char(l);
    return my_token;
}

/* reads letters until non-letter character, stores it in string, and returns
 * string.*/
char *read_identifier(lexer *l) {
    char *identifier;
    int start_pos = l->position;
    while (is_letter(peek_ahead(l))) {
        read_char(l);
    }
    identifier = malloc(l->position + 1 - start_pos + 1);
    strncpy(identifier, l->input + start_pos, l->position - start_pos + 1);
    identifier[l->position + 1 - start_pos + 1] = '\0';

    return identifier;
}

/* read_int. reads digits until non-digit character, stores it in string, and
 * returns string.*/
char *read_int(lexer *l) {
    char *identifier;
    int start_pos = l->position;
    while (is_digit(peek_ahead(l))) {
        read_char(l);
    }

    identifier = malloc(l->position + 1 - start_pos + 1);
    strncpy(identifier, l->input + start_pos, l->position - start_pos + 1);
    identifier[l->position + 1 - start_pos + 1] = '\0';

    return identifier;
}

/* checks if character is a letter.*/
int is_letter(char c) {
    return 'a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || c == '_';
}

/* hecks if character is a digit.*/
int is_digit(char c) { return '0' <= c && c <= '9'; }

/* advances the lexer past any whitespace.*/
void eat_whitespace(lexer *l) {
    while (l->ch == ' ' || l->ch == '\t' || l->ch == '\n' || l->ch == '\r') {
        read_char(l);
    }
}

/* returns the character 1 position ahead.*/
char peek_ahead(lexer *l) {
    if (l->read_position >= strlen(l->input)) {
        return '\0';
    } else {
        return l->input[l->read_position];
    }
}