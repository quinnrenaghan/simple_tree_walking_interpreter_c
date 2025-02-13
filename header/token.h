#ifndef TOKEN_H
#define TOKEN_H

// CONSTANTS
#define MAX_STATEMENTS 500
#define MAX_ERRORS 100
#define MAX_STR_LEN 100
#define MAX_PARAMS 20
#define MAX_ENV_SIZE 200

extern int token_created;
extern int token_freed;

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
    STRING,
} token_type;

typedef struct {
    token_type type;
    char *value;
    int ref_count;
} token;

token_type lookup_identifier(char *identifier);
void release_token(token *token);
void retain_token(token *token);
token* create_token(token *token);

#endif