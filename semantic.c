#include "semantic.h"
#include <stdlib.h>
#include <string.h>

// Function to create a new symbol table
struct SymbolTable* createSymbolTable() {
    struct SymbolTable* table = (struct SymbolTable*)malloc(sizeof(struct SymbolTable));
    if (table != NULL) {
        table->symbols = NULL;
        table->size = 0;
        table->capacity = 0;
    }
    return table;
}

// Function to free the symbol table
void freeSymbolTable(struct SymbolTable* table) {
    if (table != NULL) {
        for (int i = 0; i < table->size; i++) {
            free(table->symbols[i].name);
        }
        free(table->symbols);
        free(table);
    }
}

// Function to add a symbol to the table
int addSymbol(struct SymbolTable* table, const char* name, int type) {
    if (table->size == table->capacity) {
        int newCapacity = (table->capacity == 0) ? 1 : table->capacity * 2;
        struct Symbol* resizedArray = (struct Symbol*)realloc(table->symbols, newCapacity * sizeof(struct Symbol));
        if (resizedArray == NULL) {
            return 0; // Memory allocation error
        }
        table->symbols = resizedArray;
        table->capacity = newCapacity;
    }

    table->symbols[table->size].name = strdup(name);
    table->symbols[table->size].type = type;
    table->size++;
    return 1; // Success
}

// Function to find a symbol in the table
struct Symbol* findSymbol(const struct SymbolTable* table, const char* name) {
    if (table != NULL) {
        for (int i = 0; i < table->size; i++) {
            if (strcmp(table->symbols[i].name, name) == 0) {
                return &table->symbols[i];
            }
        }
    }
    return NULL; // Not found
}

int performSemanticAnalysis(struct ASTNode* node, struct SymbolTable* table) {
    if (node == NULL) {
        return SEMANTIC_OK;
    }

    switch (node->type) {
        case NODE_CLIENT_PROFILE:
            if (findSymbol(table, node->data.clientProfile.name) != NULL) {
                return REDECLARATION_OF_SYMBOL;
            }
            addSymbol(table, node->data.clientProfile.name, TYPE_CLIENT);
            break;

        case NODE_ASSIGNMENT:
            if (findSymbol(table, node->data.assignment.client->data.clientProfile.name) == NULL) {
                return UNDEFINED_IDENTIFIER;
            }
            // Add assignment symbol (if relevant)
            addSymbol(table, node->data.assignment.client->data.clientProfile.name, TYPE_ASSIGNMENT);
            break;

        case NODE_EXERCISE:
            if (node->data.exercise.sets <= 0 || node->data.exercise.rest <= 0) {
                return INVALID_EXERCISE_DEFINITION;
            }
            // Add exercise symbol
            addSymbol(table, node->data.exercise.name, TYPE_EXERCISE);
            break;

        case NODE_SHOW_PLANS:
            if (findSymbol(table, node->data.showPlans.clientName) == NULL) {
                return UNDEFINED_IDENTIFIER;
            }
            // Add show plans symbol (if relevant)
            addSymbol(table, node->data.showPlans.clientName, TYPE_SHOW_PLAN);
            break;

        case NODE_LITERAL:
            if (node->data.literal.value <= 0) {
                return UNDEFINED_IDENTIFIER;
            }
            // Add literal symbol (if relevant)
            addSymbol(table, node->data.literal.value, TYPE_IDENTIFIER);
            break;
        case NODE_IDENTIFIER:
            // Ensure identifier exists
            if (findSymbol(table, node->data.identifier.name) == NULL) {
                // If identifiers are declared elsewhere, add them to the table
                addSymbol(table, node->data.identifier.name, TYPE_IDENTIFIER);
            break;
        case NODE_PLAN:
            // Ensure plan is not already defined
            if (findSymbol(table, node->data.plan.name) != NULL) {
                return REDECLARATION_OF_SYMBOL;
            }
            addSymbol(table, node->data.plan.name, TYPE_PLAN);
            break;
        case NODE_DAY:
            // Ensure day is not already defined
            if (findSymbol(table, node->data.day.name) != NULL) {
                return REDECLARATION_OF_SYMBOL;
            }
            addSymbol(table, node->data.day.name, TYPE_DAY);
            break;
        case NODE_SETS:
            // Ensure sets is not already defined
            if (findSymbol(table, "sets") != NULL) {
                return REDECLARATION_OF_SYMBOL;
            }
            addSymbol(table, "sets", TYPE_SETS);
            break;
        case NODE_REST:
            // Ensure rest is not already defined
            if (findSymbol(table, "rest") != NULL) {
                return REDECLARATION_OF_SYMBOL;
            }
            addSymbol(table, "rest", TYPE_REST);
            break;
    }

    // Recursively analyze children
    for (int i = 0; i < node->childrenCount; i++) {
        int result = performSemanticAnalysis(node->children[i], table);
        if (result != SEMANTIC_OK) {
            return result;
        }
    }

    return SEMANTIC_OK;
}
