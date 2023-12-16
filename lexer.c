#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"

// List of keywords
const char* keywords[] = {
    "ClientProfile", "WorkoutPlan", "int", "string", "float",
    "Function", "If", "Else", "ForEach", "UpdateProfile",
    "LogWorkout", "ShowProgress", "SaveExcel", "Input", "Output",
    "Try", "Catch", "sets", "reps", "time",
    NULL // Sentinel value
};

int isKeyword(const char* str) {
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) return 1;
    }
    return 0;
}

// Tokenize an operator
Token tokenizeOperator(const char** src) {
    char op[3];
    int i = 0;
    while (strchr("+-*/><=!&|", **src) && i < 2) {
        op[i++] = *(*src)++;
    }
    op[i] = '\0';

    Token token;
    token.type = TOKEN_OPERATOR;
    token.value = strdup(op);
    return token;
}

// Tokenize punctuation
Token tokenizePunctuation(const char* src) {
    Token token;
    token.type = TOKEN_PUNCTUATION;
    token.value = strndup(src, 1);
    return token;
}

// Tokenize assignment symbol
Token tokenizeAssignment(const char* src) {
    Token token;
    token.type = TOKEN_ASSIGNMENT;
    token.value = strndup(src, 1); // Assuming '=' is the assignment symbol
    return token;
}

// Tokenize a number (integer or float)
Token tokenizeNumber(const char** src) {
    char numberBuffer[64]; // Size to accommodate floats
    int i = 0;
    int isFloat = 0; // Flag to indicate if the number is a float

    while (isdigit(**src) || (**src == '.' && !isFloat)) {
        if (**src == '.') {
            isFloat = 1; // Mark as float if a decimal point is encountered
        }
        numberBuffer[i++] = *(*src)++;
    }

    numberBuffer[i] = '\0';

    Token token;
    token.type = isFloat ? TOKEN_FLOAT : TOKEN_INTEGER;
    token.value = strdup(numberBuffer);
    return token;
}


// Tokenize a string including quotation marks
Token tokenizeString(const char** src) {
    char stringBuffer[256]; // Arbitrary size
    int i = 0;

    // Include the starting quote
    stringBuffer[i++] = *(*src)++;

    while (**src != '\"' && **src != '\0') {
        stringBuffer[i++] = *(*src)++;
    }

    // Include the closing quote if present
    if (**src == '\"') {
        stringBuffer[i++] = *(*src)++;
    }

    stringBuffer[i] = '\0';
    Token token;
    token.type = TOKEN_STRING;
    token.value = strdup(stringBuffer);
    return token;
}

// Tokenize a single word
Token tokenizeWord(char* word) {
    Token token;
    if (isKeyword(word)) {
        token.type = TOKEN_KEYWORD;
    } else {
        token.type = TOKEN_IDENTIFIER;
    }
    token.value = strdup(word);
    return token;
}

// Tokenize a time format
Token tokenizeTime(const char** src) {
    char timeBuffer[6]; // Format HH:MM
    int i = 0;

    while (isdigit(**src) || **src == ':') {
        timeBuffer[i++] = *(*src)++;
    }

    timeBuffer[i] = '\0';

    Token token;
    token.type = TOKEN_TIME; // You need to add TOKEN_TIME to the TokenType enum
    token.value = strdup(timeBuffer);
    return token;
}

// Main tokenizer function
Token* tokenize(const char* src) {
    Token* tokens = malloc(sizeof(Token) * 100);
    int tokenCount = 0;
    int expectTimeFormat = 0;

    while (*src != '\0') {
        if (isspace(*src)) {
            src++;
        } else if (isalpha(*src)) {
            char word[50];
            int wordLength = 0;
            while (isalpha(*src) || isdigit(*src) || *src == '_') {
                word[wordLength++] = *src++;
            }
            word[wordLength] = '\0';
            Token currentToken = tokenizeWord(word);
            tokens[tokenCount++] = currentToken;

            // Set flag if 'time' keyword is found
            if (strcmp(currentToken.value, "time") == 0) {
                expectTimeFormat = 1;
            }
        } else if (isdigit(*src) || (*src == ':' && expectTimeFormat)) {
            if (expectTimeFormat) {
                tokens[tokenCount++] = tokenizeTime(&src);
                expectTimeFormat = 0;
            } else {
                tokens[tokenCount++] = tokenizeNumber(&src);
            }
        } else if (strchr("+-*/><!&|", *src)) {
            tokens[tokenCount++] = tokenizeOperator(&src);
        } else if (strchr("(){}[],;", *src)) {
            tokens[tokenCount++] = tokenizePunctuation(src);
            src++;
        } else if (*src == '\"') {
            tokens[tokenCount++] = tokenizeString(&src);
        } else if (*src == '=') {
            tokens[tokenCount++] = tokenizeAssignment(src);
            src++;
            // Check if next token should be a time format
            if (expectTimeFormat) {
                while (isspace(*src)) src++; // Skip whitespace after '='
                if (*src == '0' || *src == ':') {
                    tokens[tokenCount++] = tokenizeTime(&src);
                    expectTimeFormat = 0;
                }
            }
        }
    }

    tokens[tokenCount].type = TOKEN_EOF;
    return tokens;
}