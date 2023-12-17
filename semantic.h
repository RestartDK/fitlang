#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "parser.h"

// Define semantic types for FitLang
#define TYPE_INTEGER 0
#define TYPE_IDENTIFIER 1
#define TYPE_PLAN 2
#define TYPE_CLIENT 3
#define TYPE_EXERCISE 4
#define TYPE_DAY 5
#define TYPE_ASSIGNMENT 6
#define TYPE_SHOW_PLAN 7
#define TYPE_REST 8
#define TYPE_SETS 9

// Define error codes for semantic analysis
#define SEMANTIC_OK 100
#define UNDEFINED_IDENTIFIER -1
#define INVALID_EXERCISE_DEFINITION -2
#define TYPE_MISMATCH -3
#define INVALID_NUMBER_OF_ARGUMENTS -4
#define INVALID_ARGUMENT_TYPE -5
#define REDECLARATION_OF_SYMBOL -6
#define INVALID_OPERATION -7
#define SCOPE_VIOLATION -8
#define MISSING_RETURN_STATEMENT -9
#define INVALID_ASSIGNMENT -10
#define BREAK_OUT_OF_CONTEXT -11
#define CONTINUE_OUT_OF_CONTEXT -12
#define CYCLIC_DEPENDENCY -13
#define INVALID_ARRAY_INDEX -14
#define RUNTIME_ERROR -15

// Symbol structure for semantic analysis
struct Symbol {
    char* name;
    int type;
    union {
        char* strValue;
        int intValue;
    } value;
};


// Symbol Table structure
struct SymbolTable {
    struct Symbol* symbols; // Array of symbols
    int size;               // Number of symbols
    int capacity;           // Capacity of the symbol array
};

// Function prototypes for symbol table management
struct SymbolTable* createSymbolTable();
void freeSymbolTable(struct SymbolTable* table);
int addSymbol(struct SymbolTable* table, const char* name, int type, int intValue);
struct Symbol* findSymbol(const struct SymbolTable* table, const char* name);

// Function prototype for semantic analysis
int performSemanticAnalysis(struct ASTNode* ast, struct SymbolTable* table);

#endif // SEMANTIC_H