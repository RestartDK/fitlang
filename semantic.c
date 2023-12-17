#include "semantic.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Function to create a new symbol table
struct SymbolTable *createSymbolTable()
{
    struct SymbolTable *table = (struct SymbolTable *)malloc(sizeof(struct SymbolTable));
    if (table != NULL)
    {
        table->symbols = NULL;
        table->size = 0;
        table->capacity = 0;
    }
    return table;
}

// Function to free the symbol table
void freeSymbolTable(struct SymbolTable *table)
{
    if (table != NULL)
    {
        for (int i = 0; i < table->size; i++)
        {
            free(table->symbols[i].name);
        }
        free(table->symbols);
        free(table);
    }
}

// Function to add a symbol to the table
int addSymbol(struct SymbolTable *table, const char *name, int type, int intValue) {
    if (table == NULL) {
        return 0; // Error if table is NULL
    }

    // Resize the symbol table array if necessary
    if (table->size == table->capacity) {
        int newCapacity = (table->capacity == 0) ? 1 : table->capacity * 2;
        struct Symbol *resizedArray = (struct Symbol *)realloc(table->symbols, newCapacity * sizeof(struct Symbol));
        if (resizedArray == NULL) {
            return 0; // Memory allocation error
        }
        table->symbols = resizedArray;
        table->capacity = newCapacity;
    }

    // Allocate memory for the symbol's name and set its value
    table->symbols[table->size].name = strdup(name);
    table->symbols[table->size].type = type;
    
    // Set the value of the symbol based on its type
    if (type == TYPE_INTEGER) {
        // If it's an integer (literal or otherwise), store the integer value
        table->symbols[table->size].value.intValue = intValue;
    } else {
        // For other types, the intValue is not used, set to 0 or some default
        table->symbols[table->size].value.intValue = 0;
    }

    table->size++;
    return 1; // Success
}


// Function to find a symbol in the table
struct Symbol *findSymbol(const struct SymbolTable *table, const char *name)
{
    if (table != NULL)
    {
        for (int i = 0; i < table->size; i++)
        {
            if (strcmp(table->symbols[i].name, name) == 0)
            {
                return &table->symbols[i];
            }
        }
    }
    return NULL; // Not found
}

int performSemanticAnalysis(struct ASTNode *node, struct SymbolTable *table) {
    if (node == NULL) {
        return SEMANTIC_OK;
    }

    int result = SEMANTIC_OK;

    switch (node->type) {
        case NODE_CLIENT_PROFILE:
            if (findSymbol(table, node->data.clientProfile.name) != NULL) {
                return REDECLARATION_OF_SYMBOL;
            }
            addSymbol(table, node->data.clientProfile.name, TYPE_CLIENT, 0);
            break;

        case NODE_ASSIGNMENT:
            if (findSymbol(table, node->data.assignment.client->data.clientProfile.name) == NULL) {
                return UNDEFINED_IDENTIFIER;
            }
            // Perform additional checks or actions if necessary
            break;

        case NODE_EXERCISE:
            if (node->data.exercise.sets <= 0 || node->data.exercise.rest <= 0) {
                return INVALID_EXERCISE_DEFINITION;
            }
            // Perform additional checks or actions if necessary
            break;

        case NODE_SHOW_PLANS:
            if (findSymbol(table, node->data.showPlans.clientName) == NULL) {
                return UNDEFINED_IDENTIFIER;
            }
            // Perform additional checks or actions if necessary
            break;

        case NODE_LITERAL:
            if (node->data.literal.value <= 0) {
                return INVALID_EXERCISE_DEFINITION;
            }
            addSymbol(table, NULL, TYPE_INTEGER, node->data.literal.value);
            break;

        case NODE_IDENTIFIER:
            if (findSymbol(table, node->data.identifier.name) == NULL) {
                // Add the identifier to the symbol table
                addSymbol(table, node->data.identifier.name, TYPE_IDENTIFIER, 0);
            }
            break;

        case NODE_PLAN:
        case NODE_DAY:
        case NODE_SETS:
        case NODE_REST:
            // Handle other node types similarly
            // Each node type might have its own set of checks and validations
            break;

        case NODE_MAIN:
            // For the main node, simply continue with the analysis of its children
            break;

        default:
            // Handle any other cases or unknown node types
            break;
    }

    // Recursively analyze children
    for (int i = 0; i < node->childrenCount; i++) {
        result = performSemanticAnalysis(node->children[i], table);
        if (result != SEMANTIC_OK) {
            return result;
        }
    }

    return SEMANTIC_OK;
}
