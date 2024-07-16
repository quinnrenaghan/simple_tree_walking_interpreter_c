#ifndef TOKEN_H 
#define TOKEN_H

enum TokenType lookup_identifier(char* identifier);

enum TokenType {
    // operators
    ASSIGN,
    PLUS,
    MINUS,
    BANG,
    ASTERISK,
    SLASH,
    LT,
    GT,

    //boolean operators
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
};

typedef struct {
    enum TokenType type;
    char* value;
} token;

#endif