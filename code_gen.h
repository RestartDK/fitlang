#ifndef CODE_GEN_H
#define CODE_GEN_H

#include "parser.h"  // Include the parser header which defines the AST structure

// Constants for code generation, if needed
#define MAX_CODE_LENGTH 1024

// Enumerate the types of target code you might generate (e.g., bytecode, assembly, etc.)
typedef enum {
    TARGET_BYTECODE,
    TARGET_ASSEMBLY,
    TARGET_C_CODE
} TargetCodeType;

// Structure to represent the state of the code generator
typedef struct {
    char* code;  // Buffer to store the generated code
    int length;  // Current length of the generated code
    TargetCodeType targetType;  // Type of target code to generate
} CodeGenContext;

// Function prototypes for code generation
void initCodeGenContext(CodeGenContext* context, TargetCodeType targetType);
void freeCodeGenContext(CodeGenContext* context);
void generateCode(struct ASTNode* ast, CodeGenContext* context);

// Helper function prototypes for generating different types of code
void generateFunction(struct ASTNode* node, CodeGenContext* context);
void generateStatement(struct ASTNode* node, CodeGenContext* context);
void generateExpression(struct ASTNode* node, CodeGenContext* context);
// Add more as necessary for your language

#endif // CODE_GEN_H
