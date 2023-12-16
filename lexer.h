#ifndef LEXER_H
#define LEXER_H

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

// Token types
typedef enum {
    TOKEN_CLIENT_PROFILE,
    TOKEN_ASSIGN,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_COLON,
    TOKEN_PIPE,
    TOKEN_IDENTIFIER,
    TOKEN_STRING_LITERAL,
    TOKEN_EXERCISE,
    TOKEN_SETS,
    TOKEN_REST,
    TOKEN_INT_LITERAL,
    TOKEN_SHOW_PLANS,
    TOKEN_MONDAY,
    TOKEN_TUESDAY,
    TOKEN_WEDNESDAY,
    TOKEN_THURSDAY,
    TOKEN_FRIDAY,
    TOKEN_SATURDAY,
    TOKEN_SUNDAY
} TokenType;

// Token structure
typedef struct {
    TokenType type;  
    char* value;     
} Token;

#endif