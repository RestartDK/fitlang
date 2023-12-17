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

ASTNode* parseClientProfile(Token** tokens) {
    if (tokens[0]->type != TOKEN_CLIENT_PROFILE) {
        fprintf(stderr, "Expected client profile declaration, got %s\n", tokens[0]->value);
        return NULL;
    }
    tokens++;  // Consume TOKEN_CLIENT_PROFILE

    if (tokens[0]->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Expected identifier for client profile, got %s\n", tokens[0]->value);
        return NULL;
    }

    ASTNode* node = createASTNode(NODE_CLIENT_PROFILE, tokens[0]->value, 0);
    tokens++;  // Consume TOKEN_IDENTIFIER

    if (tokens[0]->type != TOKEN_SEMICOLON) {
        fprintf(stderr, "Expected semicolon, got %s\n", tokens[0]->value);
        freeAST(node);
        return NULL;
    }
    tokens++;  // Consume TOKEN_SEMICOLON

    return node;
}


// Parse a showPlans statement
ASTNode* parseShowPlans(Token** tokens) {
    // Expecting a TOKEN_IDENTIFIER token (the client name)
    if (tokens[0]->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Expected an identifier for client name in showPlans, got %s\n", tokens[0]->value);
        return NULL;
    }

    // Create a new node for showPlans
    ASTNode* node = createASTNode(NODE_SHOW_PLANS, tokens[0]->value, 0);
    if (!node) return NULL;

    // Consume the client name token
    free(tokens[0]);
    tokens++;

    // Expecting a semicolon
    if (tokens[0]->type != TOKEN_SEMICOLON) {
        fprintf(stderr, "Expected a semicolon after showPlans, got %s\n", tokens[0]->value);
        freeAST(node);
        return NULL;
    }

    // Consume the semicolon
    free(tokens[0]);
    tokens++;

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
ASTNode* parseDay(Token** tokens) {
    // Expecting a TOKEN_IDENTIFIER token (the day name)
    if (tokens[0]->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Expected an identifier for day name, got %s\n", tokens[0]->value);
        return NULL;
    }

    // Create a new node for day
    ASTNode* node = createASTNode(NODE_DAY, tokens[0]->value, 0);
    if (!node) return NULL;

    // Consume the day name token
    free(tokens[0]);
    tokens++;

    // Expecting a left brace
    if (tokens[0]->type != TOKEN_LEFT_BRACE) {
        fprintf(stderr, "Expected a left brace after day name, got %s\n", tokens[0]->value);
        freeAST(node);
        return NULL;
    }

    // Consume the left brace
    free(tokens[0]);
    tokens++;

    // Parse exercises for the day
    while (tokens[0]->type != TOKEN_RIGHT_BRACE) {
        if (tokens[0]->type == TOKEN_IDENTIFIER) {
            // Parse an exercise
            ASTNode* exerciseNode = parseExercise(tokens);
            if (exerciseNode) {
                // Add the exercise node as a child
                addASTChildNode(node, exerciseNode);
            } else {
                // Error parsing exercise, cleanup and return NULL
                freeAST(node);
                return NULL;
            }
        } else {
            fprintf(stderr, "Expected an identifier (exercise name) in day, got %s\n", tokens[0]->value);
            freeAST(node);
            return NULL;
        }
    }

    // Expecting a right brace
    if (tokens[0]->type != TOKEN_RIGHT_BRACE) {
        fprintf(stderr, "Expected a right brace at the end of day, got %s\n", tokens[0]->value);
        freeAST(node);
        return NULL;
    }

    // Consume the right brace
    free(tokens[0]);
    tokens++;

    return node;
}

ASTNode* parseAssignment(Token** tokens) {
    printf("Debug: Entering parseAssignment\n"); // Debug start of function

    // Check for 'assign' token
    if ((*tokens)->type != TOKEN_ASSIGN) {
        fprintf(stderr, "Debug: Error - Expected 'assign', got %s\n", (*tokens)->value); // Debug error
        return NULL;
    }
    printf("Debug: Consumed 'assign' token. Current token: %s\n", (*tokens)->value); // Debug token consumption
    tokens++;  // Move to the next token

    // Check for plan identifier
    if ((*tokens)->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Debug: Error - Expected plan identifier, got %s\n", (*tokens)->value); // Debug error
        return NULL;
    }
    char* planName = strdup((*tokens)->value);
    printf("Debug: Consumed plan identifier. Plan Name: %s\n", planName); // Debug token consumption
    tokens++;  // Move to the next token

    // Check for 'to' keyword
    if ((*tokens)->type != TOKEN_TO) {
        free(planName);
        fprintf(stderr, "Debug: Error - Expected 'to', got %s\n", (*tokens)->value); // Debug error
        return NULL;
    }
    printf("Debug: Consumed 'to' token.\n"); // Debug token consumption
    tokens++;  // Move to the next token

    // Check for client identifier
    if ((*tokens)->type != TOKEN_IDENTIFIER) {
        free(planName);
        fprintf(stderr, "Debug: Error - Expected client identifier, got %s\n", (*tokens)->value); // Debug error
        return NULL;
    }
    char* clientName = strdup((*tokens)->value);
    printf("Debug: Consumed client identifier. Client Name: %s\n", clientName); // Debug token consumption
    tokens++;  // Move to the next token

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

ASTNode* parseShowPlans(Token** tokens, int* position) {
    if (tokens[*position]->type != TOKEN_SHOW_PLANS) {
        // Error handling: Expected 'showPlans' token
        return NULL;
    }

    // Create a new node for plan
    ASTNode* node = createASTNode(NODE_PLAN, tokens[0]->value, 0);
    if (!node) return NULL;

    // Consume the plan name token
    free(tokens[0]);
    tokens++;

    // Expecting a left brace
    if (tokens[0]->type != TOKEN_LEFT_BRACE) {
        fprintf(stderr, "Expected a left brace after plan name, got %s\n", tokens[0]->value);
        freeAST(node);
        return NULL;
    }

    // Consume the left brace
    free(tokens[0]);
    tokens++;

    // Parse assignments for the plan
    while (tokens[0]->type != TOKEN_RIGHT_BRACE) {
        if (tokens[0]->type == TOKEN_IDENTIFIER) {
            // Parse an assignment
            ASTNode* assignmentNode = parseAssignment(tokens);
            if (assignmentNode) {
                // Add the assignment node as a child
                addASTChildNode(node, assignmentNode);
            } else {
                // Error parsing assignment, cleanup and return NULL
                freeAST(node);
                return NULL;
            }
        } else {
            fprintf(stderr, "Expected an identifier (day name) in plan, got %s\n", tokens[0]->value);
            freeAST(node);
            return NULL;
        }
    }

    // Expecting a right brace
    if (tokens[0]->type != TOKEN_RIGHT_BRACE) {
        fprintf(stderr, "Expected a right brace at the end of plan, got %s\n", tokens[0]->value);
        freeAST(node);
        return NULL;
    }

    // Consume the right brace
    free(tokens[0]);
    tokens++;

    // Expecting a semicolon
    if (tokens[0]->type != TOKEN_SEMICOLON) {
        fprintf(stderr, "Expected a semicolon after plan, got %s\n", tokens[0]->value);
        freeAST(node);
        return NULL;
    }

    // Consume the semicolon
    free(tokens[0]);
    tokens++;

    return node;
}


ASTNode* parseProgram(Token** tokens) {
    ASTNode* root = createASTNode(NODE_MAIN, NULL, 0);
    if (!root) return NULL;

    while (*tokens) {
        ASTNode* child = NULL;
        switch ((*tokens)->type) {
            case TOKEN_CLIENT_PROFILE:
                child = parseClientProfile(tokens);
                break;
            case TOKEN_ASSIGN:
                child = parseAssignment(tokens);
                break;
            case TOKEN_SHOW_PLANS:
                child = parseShowPlans(tokens);
                break;
            case TOKEN_MONDAY:
            case TOKEN_TUESDAY:
            case TOKEN_WEDNESDAY:
            case TOKEN_THURSDAY:
            case TOKEN_FRIDAY:
            case TOKEN_SATURDAY:
            case TOKEN_SUNDAY:
                child = parseDay(tokens);
                break;
            default:
                fprintf(stderr, "Unexpected token: %s\n", (*tokens)->value);
                freeAST(root);
                return NULL;
        }

        if (child) {
            addASTChildNode(root, child);
        } else {
            // Error occurred during parsing, cleanup and return NULL
            freeAST(root);
            return NULL;
        }
    }

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
                node->data.assignment.client->name, 
                node->data.assignment.plan->data.plan.name); // Adjusted for the new structure
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
    char* sourceCode1 = "ClientProfile Daniel;";
    char* sourceCode2 = "assign muscleBuildingPlan Daniel {\n"
                        "    Monday {\n"
                        "        exercise: \"squats\" | sets: 3 | rest: 1;\n"
                        "        exercise: \"leg press\" | sets: 3 | rest: 1;\n"
                        "    }\n"
                        "    Tuesday {\n"
                        "        exercise: \"bench press\" | sets: 3 | rest: 1;\n"
                        "    }\n"
                        "};";
    char* sourceCode3 = "showPlans(Daniel);";
    char* sourceCode4 = "ClientProfile Daniel;\n"
                        "assign muscleBuildingPlan Daniel {\n"
                        "    Monday {\n"
                        "        exercise: \"squats\" | sets: 3 | rest: 1;\n"
                        "        exercise: \"leg press\" | sets: 3 | rest: 1;\n"
                        "    }\n"
                        "    Tuesday {\n"
                        "        exercise: \"bench press\" | sets: 3 | rest: 1;\n"
                        "    }\n"
                        "};\n"
                        "showPlans(Daniel);";

    // Assuming readSourceCode and lexer are defined and implemented
    Token** tokens;

    // Test case 1
    tokens = lexer(sourceCode1);
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

    // Test case 2
    tokens = lexer(sourceCode2);
    printf("Tokens generated by lexer for Test Case 2:\n");
    for (int i = 0; tokens[i] != NULL; i++) {
        printf("Token %d: type = %d, value = %s\n", i, tokens[i]->type, tokens[i]->value);
    }
    ASTNode* ast2 = parseProgram(tokens);
    printf("AST generated by parser for Test Case 2:\n");
    if (ast2 == NULL) {
        fprintf(stderr, "Error in parsing Test Case 2.\n");
    } else {
        printAST(ast2, 0);
    }
    freeAST(ast2);

    // Test case 3
    tokens = lexer(sourceCode3);
    printf("Tokens generated by lexer for Test Case 3:\n");
    for (int i = 0; tokens[i] != NULL; i++) {
        printf("Token %d: type = %d, value = %s\n", i, tokens[i]->type, tokens[i]->value);
    }
    ASTNode* ast3 = parseProgram(tokens);
    printf("AST generated by parser for Test Case 3:\n");
    if (ast3 == NULL) {
        fprintf(stderr, "Error in parsing Test Case 3.\n");
    } else {
        printAST(ast3, 0);
    }
    freeAST(ast3);

    // Test case 4
    tokens = lexer(sourceCode4);
    printf("Tokens generated by lexer for Test Case 4:\n");
    for (int i = 0; tokens[i] != NULL; i++) {
        printf("Token %d: type = %d, value = %s\n", i, tokens[i]->type, tokens[i]->value);
    }
    ASTNode* ast4 = parseProgram(tokens);
    printf("AST generated by parser for Test Case 4:\n");
    if (ast4 == NULL) {
        fprintf(stderr, "Error in parsing Test Case 4.\n");
    } else {
        printAST(ast4, 0);
    }
    freeAST(ast4);

    return EXIT_SUCCESS;
}