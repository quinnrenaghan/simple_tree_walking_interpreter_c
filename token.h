#ifndef TOKEN_H
#define TOKEN_H

#define MAX_STATEMENTS 500
#define MAX_ERRORS 100
#define MAX_STR_LEN 100


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

typedef struct {
    token_type type;
    char* value;
} token;

token_type lookup_identifier(char* identifier);

#endif