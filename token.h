#ifndef TOKEN_H
#define TOKEN_H

/* constants to be used throughout the project*/
#define MAX_STATEMENTS 500
#define MAX_ERRORS 100
#define MAX_STR_LEN 100

/* enum: all the token types*/
typedef enum {
    // operators
    ASSIGN,
    PLUS,
    MINUS,
    BANG,
    ASTERISK,
    SLASH,
    LT,
    GT,

    // boolean operators
    EQ,
    NOT_EQ,

    // syntax
    COMMA,
    SEMICOLON,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,

    // other
    ILLEGAL,
    EOF_TOKEN,

    // keywords
    FUNCTION,
    SET,
    TRUE,
    FALSE,
    IF,
    ELSE,
    RETURN,

    // type
    IDENT,
    INT,
} token_type;

/* a token has a type and a string value.*/
typedef struct {
    token_type type;
    char* value;
} token;

token_type lookup_identifier(char* identifier);

#endif