#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lexer.h"
#include <stdbool.h>
#define INITIAL_SIZE 32

// Helper functions
bool is_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

bool is_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

TokenType identifyKeywordOrIdentifier(const char* word) {
    if (strcmp(word, "ClientProfile") == 0) {
        return TOKEN_CLIENT_PROFILE;
    } else if (strcmp(word, "assign") == 0) {
        return TOKEN_ASSIGN;
    } else if (strcmp(word, "to") == 0) {
        return TOKEN_TO;
    } else if (strcmp(word, "Monday") == 0) {
        return TOKEN_MONDAY;
    } else if (strcmp(word, "Tuesday") == 0) {
        return TOKEN_TUESDAY;
    } else if (strcmp(word, "Wednesday") == 0) {
        return TOKEN_WEDNESDAY;
    } else if (strcmp(word, "Thursday") == 0) {
        return TOKEN_THURSDAY;
    } else if (strcmp(word, "Friday") == 0) {
        return TOKEN_FRIDAY;
    } else if (strcmp(word, "Saturday") == 0) {
        return TOKEN_SATURDAY;
    } else if (strcmp(word, "Sunday") == 0) {
        return TOKEN_SUNDAY;
    } else if (strcmp(word, "exercise") == 0) {
        return TOKEN_EXERCISE;
    } else if (strcmp(word, "sets") == 0) {
        return TOKEN_SETS;
    } else if (strcmp(word, "rest") == 0) {
        return TOKEN_REST;
    } else if (strcmp(word, "showPlans") == 0) {
        return TOKEN_SHOW_PLANS;
    }
    // "plan" and all other unrecognized words are identifiers
    return TOKEN_IDENTIFIER;
}

// Create the Tokens
Token* createToken(TokenType type, const char* value) {
    Token* token = (Token*)malloc(sizeof(Token));
    if (!token) {
        return NULL;
    }

    token->value = strdup(value);
    if (!token->value) {
        free(token);
        return NULL;
    }

    token->type = type;
    return token;
}

Token** lexer(const char* input) {
    Token** tokens = malloc(sizeof(Token*) * INITIAL_SIZE);
    int tokenCount = 0;
    int capacity = INITIAL_SIZE;

    while (*input != '\0') {
        if (isspace(*input)) {
            input++;
        } else if (is_identifier_start(*input)) {
            const char* start = input;
            while (is_identifier_char(*input)) input++;

            char* word = strndup(start, input - start);
            TokenType type = identifyKeywordOrIdentifier(word);
            tokens[tokenCount++] = createToken(type, word);
            free(word);

            if (tokenCount >= capacity) {
                capacity *= 2;
                tokens = realloc(tokens, sizeof(Token*) * capacity);
            }
        } else if (*input == '"') {
            input++;
            const char* start = input;
            while (*input && *input != '"') input++;
            if (*input == '"') {
                size_t length = input - start;
                char* literal = strndup(start, length);
                tokens[tokenCount++] = createToken(TOKEN_STRING_LITERAL, literal);
                free(literal);
                input++;
            } else {
                // Handle unterminated string literal error
            }
        } else if (isdigit(*input)) {
            const char* start = input;
            while (isdigit(*input)) input++;
            size_t length = input - start;
            char* number = strndup(start, length);
            tokens[tokenCount++] = createToken(TOKEN_INT_LITERAL, number);
            free(number);
        } else {
            switch (*input) {
                case '{':
                    tokens[tokenCount++] = createToken(TOKEN_LEFT_BRACE, "{");
                    break;
                case '}':
                    tokens[tokenCount++] = createToken(TOKEN_RIGHT_BRACE, "}");
                    break;
                case ':':
                    tokens[tokenCount++] = createToken(TOKEN_COLON, ":");
                    break;
                case '|':
                    tokens[tokenCount++] = createToken(TOKEN_PIPE, "|");
                    break;
                case ';':
                    tokens[tokenCount++] = createToken(TOKEN_SEMICOLON, ";");
                    break;
                // Add cases for other single-character tokens as needed
            }
            input++;
        }

        if (tokenCount >= capacity) {
            capacity *= 2;
            Token** resized = realloc(tokens, sizeof(Token*) * capacity);
            if (!resized) {
                for (int i = 0; i < tokenCount; ++i) {
                    free(tokens[i]->value);
                    free(tokens[i]);
                }
                free(tokens);
                return NULL;
            }
            tokens = resized;
        }
    }

    Token** resized = realloc(tokens, sizeof(Token*) * (tokenCount + 1));
    if (!resized) {
        for (int i = 0; i < tokenCount; ++i) {
            free(tokens[i]->value);
            free(tokens[i]);
        }
        free(tokens);
        return NULL;
    }
    tokens = resized;
    tokens[tokenCount] = NULL;

    return tokens;
}

void printToken(const Token* token) {
    if (token == NULL) {
        printf("NULL Token\n");
        return;
    }

    printf("Token Type: %d, Token Value: %s\n", token->type, token->value);
}

void testLexer(const char* input) {
    printf("Testing Lexer with input: %s\n", input);
    Token** tokens = lexer(input);

    for (int i = 0; tokens[i] != NULL; i++) {
        printToken(tokens[i]);
        free(tokens[i]->value);
        free(tokens[i]);
    }
    free(tokens);
}
