#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Token types
typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_KEYWORD,
    TOKEN_INTEGER,
    TOKEN_STRING,
    TOKEN_EOF
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    char* value;
} Token;

// List of keywords
const char* keywords[] = {
    "ClientProfile", "WorkoutPlan", "If", "Else", "Function",
    "Input", "Output", "ForEach", "int", "string", "float",
    NULL // Sentinel value
};

// Check if a string is a keyword
int isKeyword(const char* str) {
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) return 1;
    }
    return 0;
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

// Main tokenizer function
Token* tokenize(const char* src) {
    // Allocate memory for tokens - this is a simplified example
    Token* tokens = malloc(sizeof(Token) * 100); // Arbitrary size for demonstration
    int tokenCount = 0;

    while (*src != '\0') {
        if (isspace(*src)) {
            // Skip whitespace
            src++;
        } else if (isalpha(*src)) {
            // Start of a word
            char word[50]; // Arbitrary word size
            int wordLength = 0;

            while (isalpha(*src) || isdigit(*src) || *src == '_') {
                word[wordLength++] = *src++;
            }
            word[wordLength] = '\0';

            tokens[tokenCount++] = tokenizeWord(word);
        } else if (isdigit(*src)) {
            // Start of a number
            // ... (similar to word tokenization)
        }
        // ... (handle other token types)
    }

    tokens[tokenCount].type = TOKEN_EOF; // End-of-file token
    return tokens;
}

// Example usage
int main() {
    const char* code = "ClientProfile johnDoe; If (johnDoe.age > 30) { Output(\"Older than 30\"); }";
    Token* tokens = tokenize(code);

    for (int i = 0; tokens[i].type != TOKEN_EOF; i++) {
        printf("Token: Type = %d, Value = %s\n", tokens[i].type, tokens[i].value);
        free(tokens[i].value); // Free the strdup memory
    }

    free(tokens); // Free the tokens array
    return 0;
}
