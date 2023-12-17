#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h" // Include the header file where your AST structure is defined

// If you have a specific environment structure for storing variables, function definitions, etc.
typedef struct {
    // ... environment fields ...
} Environment;

// Function to create a new environment (if needed)
Environment* createEnvironment();

// Function to free the environment (if needed)
void freeEnvironment(Environment* env);

// Function to evaluate/execute an AST node
int evaluate(ASTNode* node, Environment* env);

#endif // INTERPRETER_H
