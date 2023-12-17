#include <stdlib.h>
#include "parser.h"
#include "lexer.h"
#include <stdbool.h>
#include <stdio.h>
#include <err.h>
#include "lexer.c"

/***
 * Helper functions
*/
bool isDayToken(TokenType type) {
    switch (type) {
        case TOKEN_MONDAY:
        case TOKEN_TUESDAY:
        case TOKEN_WEDNESDAY:
        case TOKEN_THURSDAY:
        case TOKEN_FRIDAY:
        case TOKEN_SATURDAY:
        case TOKEN_SUNDAY:
            return true;
        default:
            return false;
    }
}

/***
 * AST functions
*/

// Create a new AST node
ASTNode* createASTNode(NodeType type, const char* value, int intValue) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        fprintf(stderr, "Failed to allocate memory for ASTNode.\n");
        exit(EXIT_FAILURE);
    }
    
    // Initialize the node
    node->type = type;
    node->children = NULL;
    node->childrenCount = 0;
    
    // Set the data based on the node type
    switch (type) {
        case NODE_MAIN:
            // Initialize the data for NODE_MAIN (if needed)
            break;
        case NODE_IDENTIFIER:
            // Initialize the data for NODE_IDENTIFIER (if needed)
            if (value != NULL) {
                node->data.identifier.name = strdup(value);
            }
            break;
        case NODE_CLIENT_PROFILE:
            // Initialize the data for NODE_CLIENT_PROFILE (if needed)
            if (value != NULL) {
                node->data.clientProfile.name = strdup(value);
            }
            break;
        case NODE_ASSIGNMENT:
            // Initialize the data for NODE_ASSIGNMENT (if needed)
            break;
        case NODE_DAY:
            // Initialize the data for NODE_DAY (if needed)
            if (value != NULL) {
                node->data.day.name = strdup(value);
            }
            break;
        // Add cases for other node types as needed...
        default:
            break;
    }
    
    // Set the integer value (if applicable)
    if (type == NODE_LITERAL) {
        node->data.literal.value = intValue;
    }
    
    return node;
}

// Add a child node to an existing AST node
void addASTChildNode(ASTNode* parent, ASTNode* child) {
    if (parent == NULL || child == NULL) {
        fprintf(stderr, "Invalid parent or child node.\n");
        return;
    }
    
    // Increment the children count
    parent->childrenCount++;
    
    // Reallocate memory for children array
    parent->children = (ASTNode**)realloc(parent->children, parent->childrenCount * sizeof(ASTNode*));
    if (parent->children == NULL) {
        fprintf(stderr, "Failed to allocate memory for children array.\n");
        exit(EXIT_FAILURE);
    }
    
    // Add the child node to the parent's children array
    parent->children[parent->childrenCount - 1] = child;
}

// Free the entire AST
void freeAST(ASTNode* root) {
    if (root == NULL) {
        return;
    }
    
    // Recursively free child nodes
    for (int i = 0; i < root->childrenCount; i++) {
        freeAST(root->children[i]);
    }
    
    // Free data associated with the node
    switch (root->type) {
        case NODE_IDENTIFIER:
            free(root->data.identifier.name);
            break;
        case NODE_CLIENT_PROFILE:
            free(root->data.clientProfile.name);
            break;
        case NODE_DAY:
            free(root->data.day.name);
            break;
        // Add cases for other node types as needed...
        default:
            break;
    }
    
    // Free the children array and the node itself
    free(root->children);
    free(root);
}



/***
 * All parsing functions for each node type
*/

// Parse a client profile
ASTNode* parseClientProfile(Token*** tokens) {
    printf("Debug: parseClientProfile - Starting\n");
    
    if ((**tokens)->type != TOKEN_CLIENT_PROFILE) {
        fprintf(stderr, "Debug: parseClientProfile - Expected client profile declaration, got %s\n", (**tokens)->value);
        return NULL;
    }
    printf("Debug: parseClientProfile - Consumed TOKEN_CLIENT_PROFILE\n");
    (*tokens)++;

    if ((**tokens)->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Debug: parseClientProfile - Expected identifier for client profile, got %s\n", (**tokens)->value);
        return NULL;
    }
    printf("Debug: parseClientProfile - Consumed TOKEN_IDENTIFIER: %s\n", (**tokens)->value);
    ASTNode* node = createASTNode(NODE_CLIENT_PROFILE, (**tokens)->value, 0);
    (*tokens)++;

    if ((**tokens)->type != TOKEN_SEMICOLON) {
        fprintf(stderr, "Debug: parseClientProfile - Expected semicolon, got %s\n", (**tokens)->value);
        freeAST(node);
        return NULL;
    }
    printf("Debug: parseClientProfile - Consumed TOKEN_SEMICOLON\n");
    (*tokens)++;

    printf("Debug: parseClientProfile - Finished\n");
    return node;
}

// Parse a showPlans statement
ASTNode* parseShowPlans(Token*** tokens) {
    if ((**tokens)->type != TOKEN_SHOW_PLANS) {
        fprintf(stderr, "Expected 'showPlans', got %s\n", (**tokens)->value);
        return NULL;
    }
    (*tokens)++;  // Consume TOKEN_SHOW_PLANS

    if ((**tokens)->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Expected an identifier for client name, got %s\n", (**tokens)->value);
        return NULL;
    }

    ASTNode* node = createASTNode(NODE_SHOW_PLANS, (**tokens)->value, 0);
    (*tokens)++;  // Consume TOKEN_IDENTIFIER

    if ((**tokens)->type != TOKEN_SEMICOLON) {
        fprintf(stderr, "Expected semicolon, got %s\n", (**tokens)->value);
        freeAST(node);
        return NULL;
    }
    (*tokens)++;  // Consume TOKEN_SEMICOLON

    return node;
}

// Parse an exercise
ASTNode* parseExercise(Token** tokens) {
    // Expecting a TOKEN_IDENTIFIER token (the exercise name)
    if (tokens[0]->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Expected an identifier for exercise name, got %s\n", tokens[0]->value);
        return NULL;
    }

    // Create a new node for exercise
    ASTNode* node = createASTNode(NODE_EXERCISE, tokens[0]->value, 0);
    if (!node) return NULL;

    // Consume the exercise name token
    free(tokens[0]);
    tokens++;

    // Initialize exercise attributes
    int sets = 0;
    int rest = 0;

    // Parse exercise attributes
    while (tokens[0]->type == TOKEN_PIPE) {
        // Consume the pipe token
        free(tokens[0]);
        tokens++;

        if (tokens[0]->type == TOKEN_IDENTIFIER) {
            // Parse exercise attribute
            if (strcmp(tokens[0]->value, "sets") == 0) {
                // Consume the "sets" token
                free(tokens[0]);
                tokens++;

                // Expecting a colon
                if (tokens[0]->type == TOKEN_COLON) {
                    // Consume the colon
                    free(tokens[0]);
                    tokens++;

                    // Expecting an integer literal
                    if (tokens[0]->type == TOKEN_INT_LITERAL) {
                        sets = atoi(tokens[0]->value);

                        // Consume the integer literal token
                        free(tokens[0]);
                        tokens++;
                    } else {
                        fprintf(stderr, "Expected an integer literal for 'sets', got %s\n", tokens[0]->value);
                        freeAST(node);
                        return NULL;
                    }
                } else {
                    fprintf(stderr, "Expected a colon after 'sets', got %s\n", tokens[0]->value);
                    freeAST(node);
                    return NULL;
                }
            } else if (strcmp(tokens[0]->value, "rest") == 0) {
                // Consume the "rest" token
                free(tokens[0]);
                tokens++;

                // Expecting a colon
                if (tokens[0]->type == TOKEN_COLON) {
                    // Consume the colon
                    free(tokens[0]);
                    tokens++;

                    // Expecting an integer literal
                    if (tokens[0]->type == TOKEN_INT_LITERAL) {
                        rest = atoi(tokens[0]->value);

                        // Consume the integer literal token
                        free(tokens[0]);
                        tokens++;
                    } else {
                        fprintf(stderr, "Expected an integer literal for 'rest', got %s\n", tokens[0]->value);
                        freeAST(node);
                        return NULL;
                    }
                } else {
                    fprintf(stderr, "Expected a colon after 'rest', got %s\n", tokens[0]->value);
                    freeAST(node);
                    return NULL;
                }
            } else {
                fprintf(stderr, "Unexpected token after exercise name: %s\n", tokens[0]->value);
                freeAST(node);
                return NULL;
            }
        } else {
            fprintf(stderr, "Expected an identifier after pipe in exercise, got %s\n", tokens[0]->value);
            freeAST(node);
            return NULL;
        }
    }

    // Set exercise attributes in the node
    node->data.exercise.sets = sets;
    node->data.exercise.rest = rest;

    return node;
}

// Parse a day
ASTNode* parseDay(Token*** tokens) {
    printf("Debug: parseDay - Starting\n");

    // Expecting a day identifier token
    if ((**tokens)->type != TOKEN_IDENTIFIER || !isDayToken((**tokens)->type)) {
        fprintf(stderr, "Debug: parseDay - Expected day identifier, got %s\n", (**tokens)->value);
        return NULL;
    }
    printf("Debug: parseDay - Consumed day identifier: %s\n", (**tokens)->value);
    ASTNode* node = createASTNode(NODE_DAY, (**tokens)->value, 0);
    (*tokens)++; // Consume day identifier

    // Expecting a left brace
    if ((**tokens)->type != TOKEN_LEFT_BRACE) {
        fprintf(stderr, "Debug: parseDay - Expected left brace, got %s\n", (**tokens)->value);
        freeAST(node);
        return NULL;
    }
    printf("Debug: parseDay - Consumed left brace\n");
    (*tokens)++; // Consume left brace

    // Parse exercises for the day
    while ((**tokens)->type != TOKEN_RIGHT_BRACE) {
        if ((**tokens)->type == TOKEN_IDENTIFIER) {
            printf("Debug: parseDay - Parsing exercise: %s\n", (**tokens)->value);
            ASTNode* exerciseNode = parseExercise(*tokens);
            if (exerciseNode) {
                addASTChildNode(node, exerciseNode);
                printf("Debug: parseDay - Added exercise node\n");
            } else {
                fprintf(stderr, "Debug: parseDay - Error parsing exercise\n");
                freeAST(node);
                return NULL;
            }
        } else {
            fprintf(stderr, "Debug: parseDay - Expected exercise identifier, got %s\n", (**tokens)->value);
            freeAST(node);
            return NULL;
        }
    }

    // Expecting a right brace
    if ((**tokens)->type != TOKEN_RIGHT_BRACE) {
        fprintf(stderr, "Debug: parseDay - Expected right brace, got %s\n", (**tokens)->value);
        freeAST(node);
        return NULL;
    }
    printf("Debug: parseDay - Consumed right brace\n");
    (*tokens)++; // Consume right brace

    printf("Debug: parseDay - Finished\n");
    return node;
}


ASTNode* parseAssignment(Token*** tokens) {
    printf("Debug: Entering parseAssignment\n"); // Debug start of function

    // Check for 'assign' token
    if ((**tokens)->type != TOKEN_ASSIGN) {
        fprintf(stderr, "Debug: Error - Expected 'assign', got %s\n", (**tokens)->value); // Debug error
        return NULL;
    }
    printf("Debug: Consumed 'assign' token. Current token: %s\n", (**tokens)->value); // Debug token consumption
    (*tokens)++;  // Move to the next token

    // Check for plan identifier
    if ((**tokens)->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Debug: Error - Expected plan identifier, got %s\n", (**tokens)->value); // Debug error
        return NULL;
    }
    char* planName = strdup((**tokens)->value);
    printf("Debug: Consumed plan identifier. Plan Name: %s\n", planName); // Debug token consumption
    (*tokens)++;  // Move to the next token

    // Check for 'to' keyword
    if ((**tokens)->type != TOKEN_TO) {
        free(planName);
        fprintf(stderr, "Debug: Error - Expected 'to', got %s\n", (**tokens)->value); // Debug error
        return NULL;
    }
    printf("Debug: Consumed 'to' token.\n"); // Debug token consumption
    (*tokens)++;  // Move to the next token

    // Check for client identifier
    if ((**tokens)->type != TOKEN_IDENTIFIER) {
        free(planName);
        fprintf(stderr, "Debug: Error - Expected client identifier, got %s\n", (**tokens)->value); // Debug error
        return NULL;
    }
    char* clientName = strdup((**tokens)->value);
    printf("Debug: Consumed client identifier. Client Name: %s\n", clientName); // Debug token consumption
    (*tokens)++;  // Move to the next token

    // Create nodes for client and plan
    ASTNode* planNode = createASTNode(NODE_PLAN, planName, 0);
    ASTNode* clientNode = createASTNode(NODE_CLIENT_PROFILE, clientName, 0);
    free(planName);
    free(clientName);

    // Debug node creation
    printf("Debug: Created plan and client nodes.\n");

    // Create assignment node and link plan and client
    ASTNode* assignmentNode = createASTNode(NODE_ASSIGNMENT, NULL, 0);
    assignmentNode->data.assignment.plan = planNode;
    assignmentNode->data.assignment.client = clientNode;

    printf("Debug: Exiting parseAssignment\n"); // Debug end of function
    return assignmentNode;
}


// Parse the entire program
ASTNode* parseProgram(Token** tokens) {
    printf("Debug: parseProgram - Starting\n");
    ASTNode* root = createASTNode(NODE_MAIN, NULL, 0);
    if (!root) {
        fprintf(stderr, "Debug: Error - Failed to create root node\n");
        return NULL;
    }

    Token** currentToken = tokens;  // Use a separate pointer to advance through tokens

    while (*currentToken) {
        printf("Debug: Current token type: %d, value: %s\n", (*currentToken)->type, (*currentToken)->value);
        ASTNode* child = NULL;

        switch ((*currentToken)->type) {
            case TOKEN_CLIENT_PROFILE:
                printf("Debug: Found TOKEN_CLIENT_PROFILE\n");
                child = parseClientProfile(&currentToken);
                break;
            case TOKEN_ASSIGN:
                printf("Debug: Found TOKEN_ASSIGN\n");
                child = parseAssignment(&currentToken);
                break;
            case TOKEN_SHOW_PLANS:
                printf("Debug: Found TOKEN_SHOW_PLANS\n");
                child = parseShowPlans(&currentToken);
                break;
            case TOKEN_MONDAY:
            case TOKEN_TUESDAY:
            case TOKEN_WEDNESDAY:
            case TOKEN_THURSDAY:
            case TOKEN_FRIDAY:
            case TOKEN_SATURDAY:
            case TOKEN_SUNDAY:
                printf("Debug: Found a day token\n");
                child = parseDay(&currentToken);
                break;
            // Add other cases as needed
            default:
                fprintf(stderr, "Debug: Unexpected token: %s\n", (*currentToken)->value);
                freeAST(root);
                return NULL;
        }

        if (child) {
            printf("Debug: Adding child node to root\n");
            addASTChildNode(root, child);
        } else {
            fprintf(stderr, "Debug: Error occurred during parsing, no child node created\n");
            // Error occurred during parsing, cleanup and return NULL
            freeAST(root);
            return NULL;
        }
    }

    printf("Debug: Exiting parseProgram\n");
    return root;
}



void printAST(ASTNode *node, int depth) {
    if (!node) return;

    // Indentation for better readability
    for (int i = 0; i < depth; i++) printf("  ");

    switch (node->type) {
        case NODE_MAIN:
            printf("Program\n");
            break;
        case NODE_IDENTIFIER:
            printf("Identifier: %s\n", node->data.identifier.name);
            break;
        case NODE_CLIENT_PROFILE:
            printf("ClientProfile: %s\n", node->data.clientProfile.name);
            break;
        case NODE_PLAN:
            printf("Plan: %s\n", node->data.plan.name);
            break;
        case NODE_DAY:
            printf("Day: %s\n", node->data.day.name);
            break;
        case NODE_EXERCISE:
            printf("Exercise: %s, Sets: %d, Rest: %d\n", node->data.exercise.name, node->data.exercise.sets, node->data.exercise.rest);
            break;
        case NODE_SHOW_PLANS:
            printf("ShowPlans for: %s\n", node->data.clientProfile.name);
            break;
        case NODE_ASSIGNMENT:
            printf("Assignment: Client - %s, Plan - %s\n", 
                node->data.assignment.client->data.clientProfile.name, 
                node->data.assignment.plan->data.plan.name);
            break;

        case NODE_SETS:
            printf("Sets: %d\n", node->data.exercise.sets);
            break;
        case NODE_REST:
            printf("Rest: %d\n", node->data.exercise.rest);
            break;
        case NODE_LITERAL:
            printf("Literal: %d\n", node->data.literal.value);
            break;
    }

    // Recursively print child nodes
    for (int i = 0; i < node->childrenCount; i++) {
        printAST(node->children[i], depth + 1);
    }
}

int main() {
    // Test cases
    char* sourceCode = 
    "ClientProfile Daniel;\n"
    "assign muscleBuildingPlan to Daniel {\n"
    "    Monday {\n"
    "        exercise: \"squats\" | sets: 3 | rest: 1;\n"
    "        exercise: \"leg press\" | sets: 3 | rest: 1;\n"
    "    }\n"
    "    Tuesday {\n"
    "        exercise: \"bench press\" | sets: 3 | rest: 1;\n"
    "    }\n"
    "};";


    // Assuming readSourceCode and lexer are defined and implemented
    Token** tokens;

    // Test case 1
    tokens = lexer(sourceCode);
    printf("Tokens generated by lexer for Test Case 1:\n");
    for (int i = 0; tokens[i] != NULL; i++) {
        printf("Token %d: type = %d, value = %s\n", i, tokens[i]->type, tokens[i]->value);
    }
    ASTNode* ast1 = parseProgram(tokens);
    printf("AST generated by parser for Test Case 1:\n");
    if (ast1 == NULL) {
        fprintf(stderr, "Error in parsing Test Case 1.\n");
    } else {
        printAST(ast1, 0);
    }
    freeAST(ast1);

    return EXIT_SUCCESS;
}