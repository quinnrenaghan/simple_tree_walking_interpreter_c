#ifndef TOKEN_H 
#define TOKEN_H

enum TokenType lookup_identifier(char* identifier);

enum TokenType {
    ILLEGAL,
    EOF_TOKEN,
    IDENT,
    INT,
    ASSIGN,
    PLUS,
    COMMA,
    SEMICOLON,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    FUNCTION,
    LET
};

typedef struct {
    enum TokenType type;
    char* value;
} token;

#endif