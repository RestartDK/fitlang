#ifndef LEXER_H
#define LEXER_H

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

// Token types
typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_KEYWORD,
    TOKEN_INTEGER,
    TOKEN_FLOAT,
    TOKEN_STRING,
    TOKEN_TIME,
    TOKEN_OPERATOR,
    TOKEN_PUNCTUATION,
    TOKEN_ASSIGNMENT,
    TOKEN_EOF
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    char* value;
} Token;

// List of keywords
extern const char* keywords[];

// Check if a string is a keyword
int isKeyword(const char* str);

#endif // LEXER_H