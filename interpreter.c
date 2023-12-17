#include "interpreter.h"
#include "lexer.h"    // Your lexer header
#include "parser.h"   // Your parser header
#include "semantic.h" // Your semantic analyzer header
#include <stdio.h>
#include <stdlib.h>

char* readFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = malloc(fileSize + 1);
    if (buffer == NULL) {
        perror("Unable to allocate memory for file content");
        exit(EXIT_FAILURE);
    }

    fread(buffer, 1, fileSize, file);
    buffer[fileSize] = '\0';

    fclose(file);
    return buffer;
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename.fl>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char* sourceCode = readFile(argv[1]);

    // Lexer
    Token** tokens = lexer(sourceCode);
    if (tokens == NULL) {
        fprintf(stderr, "Lexical analysis failed.\n");
        return EXIT_FAILURE;
    }

    // Parser
    ASTNode* root = parseProgram(tokens);
    if (root == NULL) {
        fprintf(stderr, "Parsing failed.\n");
        return EXIT_FAILURE;
    }

    // Semantic Analysis (if implemented)
    struct SymbolTable* table = createSymbolTable();
    int semanticResult = performSemanticAnalysis(root, table);
    if (semanticResult != SEMANTIC_OK) {
        fprintf(stderr, "Semantic analysis failed.\n");
        return EXIT_FAILURE;
    }

    // Interpretation
    int result = evaluate(root, /* environment, if needed */);

    // Clean up
    freeAST(root);
    freeSymbolTable(table);
    free(sourceCode);

    return result;
}

