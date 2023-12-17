#include <stdlib.h>
#include "parser.h"
#include "lexer.h"
#include <stdbool.h>
#include <stdio.h>
#include <err.h>
#include "semantic.h"


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

// Helper function to parse an integer attribute (sets or rest)
int parseAttribute(Token*** tokens, int* attributeValue) {
    // Expecting a colon
    if ((**tokens)->type == TOKEN_COLON) {
        (*tokens)++; // Consume colon token

        // Expecting an integer literal
        if ((**tokens)->type == TOKEN_INT_LITERAL) {
            *attributeValue = atoi((**tokens)->value);
            printf("Debug: Parsed attribute value: %d\n", *attributeValue);
            (*tokens)++; // Consume integer literal token
            return 0; // Success
        } else {
            fprintf(stderr, "Debug: Expected TOKEN_INT_LITERAL, got %d\n", (**tokens)->type);
            return -1; // Error
        }
    } else {
        fprintf(stderr, "Debug: Expected TOKEN_COLON, got %d\n", (**tokens)->type);
        return -1; // Error
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

    printf("Debug: Creating ASTNode. Type: %d, ", type);  // Debug statement
    
    // Set the data based on the node type
    switch (type) {
        case NODE_MAIN:
            printf("NODE_MAIN\n");
            break;
        case NODE_IDENTIFIER:
            if (value != NULL) {
                node->data.identifier.name = strdup(value);
                printf("NODE_IDENTIFIER, Name: %s\n", node->data.identifier.name);
            }
            break;
        case NODE_CLIENT_PROFILE:
            if (value != NULL) {
                node->data.clientProfile.name = strdup(value);
                printf("NODE_CLIENT_PROFILE, Name: %s\n", node->data.clientProfile.name);
            }
            break;
        case NODE_PLAN:
            if (value != NULL) {
                node->data.plan.name = strdup(value);
                printf("NODE_PLAN, Name: %s\n", node->data.plan.name);
            }
            break;
        case NODE_DAY:
            if (value != NULL) {
                node->data.day.name = strdup(value);
                printf("NODE_DAY, Name: %s\n", node->data.day.name);
            }
            break;
        case NODE_EXERCISE:
            if (value != NULL) {
                node->data.exercise.name = strdup(value);
                printf("NODE_EXERCISE, Name: %s\n", node->data.exercise.name);
            }
            break;
        case NODE_ASSIGNMENT:
            printf("NODE_ASSIGNMENT\n");
            break;
        case NODE_LITERAL:
            node->data.literal.value = intValue;
            printf("NODE_LITERAL, Value: %d\n", node->data.literal.value);
            break;
        default:
            printf("Unknown NodeType\n");
            break;
    }
    
    return node;
}


// Revised addASTChildNode function
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

    printf("Debug: Added child node. Parent type: %d, Child type: %d\n", parent->type, child->type); // Debug statement
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

ASTNode* parseExercise(Token*** tokens) {
    printf("Debug: parseExercise - Starting\n");

    // Expecting TOKEN_EXERCISE
    if ((**tokens)->type != TOKEN_EXERCISE) {
        fprintf(stderr, "Debug: parseExercise - Expected TOKEN_EXERCISE, got %d\n", (**tokens)->type);
        return NULL;
    }
    (*tokens)++; // Consume TOKEN_EXERCISE

    // Expecting a colon after "exercise"
    if ((**tokens)->type != TOKEN_COLON) {
        fprintf(stderr, "Debug: parseExercise - Expected TOKEN_COLON after 'exercise', got %d\n", (**tokens)->type);
        return NULL;
    }
    (*tokens)++; // Consume TOKEN_COLON

    // Expecting a string literal for the exercise name
    if ((**tokens)->type != TOKEN_STRING_LITERAL) {
        fprintf(stderr, "Debug: parseExercise - Expected TOKEN_STRING_LITERAL, got %d\n", (**tokens)->type);
        return NULL;
    }
    char* exerciseName = strdup((**tokens)->value);
    printf("Debug: parseExercise - Exercise Name: %s\n", exerciseName);
    (*tokens)++; // Consume TOKEN_STRING_LITERAL

    // Initialize exercise attributes
    int sets = 0;
    int rest = 0;

    // Parse exercise attributes
    while ((**tokens)->type == TOKEN_PIPE) {
        printf("Debug: parseExercise - Found TOKEN_PIPE\n");
        (*tokens)++; // Consume TOKEN_PIPE

        // Parse sets
        if ((**tokens)->type == TOKEN_SETS) {
            (*tokens)++; // Consume TOKEN_SETS
            if ((**tokens)->type == TOKEN_COLON) {
                (*tokens)++; // Consume TOKEN_COLON
                if ((**tokens)->type == TOKEN_INT_LITERAL) {
                    sets = atoi((**tokens)->value);
                    printf("Debug: parseExercise - Sets: %d\n", sets);
                    (*tokens)++; // Consume TOKEN_INT_LITERAL
                }
            }
        }
        // Parse rest
        else if ((**tokens)->type == TOKEN_REST) {
            (*tokens)++; // Consume TOKEN_REST
            if ((**tokens)->type == TOKEN_COLON) {
                (*tokens)++; // Consume TOKEN_COLON
                if ((**tokens)->type == TOKEN_INT_LITERAL) {
                    rest = atoi((**tokens)->value);
                    printf("Debug: parseExercise - Rest: %d\n", rest);
                    (*tokens)++; // Consume TOKEN_INT_LITERAL
                }
            }
        } else {
            fprintf(stderr, "Debug: parseExercise - Expected TOKEN_SETS or TOKEN_REST, got %d\n", (**tokens)->type);
            free(exerciseName);
            return NULL;
        }
    }

    // Create the exercise node with the exercise name
    ASTNode* node = createASTNode(NODE_EXERCISE, exerciseName, 0);
    node->data.exercise.sets = sets;
    node->data.exercise.rest = rest;
    free(exerciseName);

    printf("Debug: parseExercise - Exiting\n");
    return node;
}



// Parse a day
ASTNode* parseDay(Token*** tokens) {
    printf("Debug: parseDay - Starting\n");

    // Check if the current token is a day token
    const char* dayName;
    switch ((**tokens)->type) {
        case TOKEN_MONDAY:
            dayName = "Monday";
            break;
        case TOKEN_TUESDAY:
            dayName = "Tuesday";
            break;
        case TOKEN_WEDNESDAY:
            dayName = "Wednesday";
            break;
        case TOKEN_THURSDAY:
            dayName = "Thursday";
            break;
        case TOKEN_FRIDAY:
            dayName = "Friday";
            break;
        case TOKEN_SATURDAY:
            dayName = "Saturday";
            break;
        case TOKEN_SUNDAY:
            dayName = "Sunday";
            break;
        default:
            fprintf(stderr, "Debug: parseDay - Expected day token, got %d\n", (**tokens)->type);
            return NULL;
    }

    printf("Debug: parseDay - Consumed day token: %s\n", dayName);
    ASTNode* dayNode = createASTNode(NODE_DAY, dayName, 0);
    (*tokens)++; // Consume day token

    // Expecting a left brace to start the day's exercises
    if ((**tokens)->type != TOKEN_LEFT_BRACE) {
        fprintf(stderr, "Debug: parseDay - Expected left brace, got %d\n", (**tokens)->type);
        freeAST(dayNode);
        return NULL;
    }
    printf("Debug: parseDay - Consumed left brace\n");
    (*tokens)++; // Consume left brace

    while ((**tokens)->type != TOKEN_RIGHT_BRACE) {
        if ((**tokens)->type == TOKEN_EXERCISE) {
            printf("Debug: parseDay - Parsing exercise\n");
            ASTNode* exerciseNode = parseExercise(tokens);
            if (exerciseNode) {
                printf("Debug: Adding exercise node: %s to day node: %s\n", exerciseNode->data.exercise.name, dayNode->data.day.name);
                addASTChildNode(dayNode, exerciseNode);
            } else {
                fprintf(stderr, "Debug: parseDay - Error parsing exercise\n");
                freeAST(dayNode);
                return NULL;
            }
        } else {
            fprintf(stderr, "Debug: parseDay - Expected TOKEN_EXERCISE or TOKEN_RIGHT_BRACE, got %d\n", (**tokens)->type);
            (*tokens)++; // Skip unexpected tokens
        }
    }

    // Expecting a right brace to end the day's exercises
    if ((**tokens)->type != TOKEN_RIGHT_BRACE) {
        fprintf(stderr, "Debug: parseDay - Expected right brace, got %d\n", (**tokens)->type);
        freeAST(dayNode);
        return NULL;
    }
    printf("Debug: parseDay - Consumed right brace\n");
    (*tokens)++; // Consume right brace

    printf("Debug: parseDay - Finished\n");
    return dayNode;
}


// Parse an assignment
ASTNode* parseAssignment(Token*** tokens) {
    printf("Debug: Entering parseAssignment\n");

    // Check for 'assign' token
    if ((**tokens)->type != TOKEN_ASSIGN) {
        fprintf(stderr, "Debug: Error - Expected 'assign', got %s\n", (**tokens)->value);
        return NULL;
    }
    printf("Debug: Consumed 'assign' token\n");
    (*tokens)++;

    // Check for plan identifier
    if ((**tokens)->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Debug: Error - Expected plan identifier, got %s\n", (**tokens)->value);
        return NULL;
    }
    char* planName = strdup((**tokens)->value);
    printf("Debug: Plan identifier: %s\n", planName);
    (*tokens)++;

    // Check for 'to' keyword
    if ((**tokens)->type != TOKEN_TO) {
        free(planName);
        fprintf(stderr, "Debug: Error - Expected 'to', got %s\n", (**tokens)->value);
        return NULL;
    }
    printf("Debug: Consumed 'to' token\n");
    (*tokens)++;

    // Check for client identifier
    if ((**tokens)->type != TOKEN_IDENTIFIER) {
        free(planName);
        fprintf(stderr, "Debug: Error - Expected client identifier, got %s\n", (**tokens)->value);
        return NULL;
    }
    char* clientName = strdup((**tokens)->value);
    printf("Debug: Client identifier: %s\n", clientName);
    (*tokens)++;

    // Expecting an opening brace '{'
    if ((**tokens)->type != TOKEN_LEFT_BRACE) {
        free(planName);
        free(clientName);
        fprintf(stderr, "Debug: Error - Expected '{', got %s\n", (**tokens)->value);
        return NULL;
    }
    printf("Debug: Consumed opening brace '{'\n");
    (*tokens)++; // Consume opening brace '{'

    // Create and initialize plan node
    ASTNode* planNode = createASTNode(NODE_PLAN, planName, 0);
    printf("Debug: Created plan node with name: %s\n", planName);
    free(planName);

    // Parse days inside the plan
    while ((**tokens)->type != TOKEN_RIGHT_BRACE) {
        if (isDayToken((**tokens)->type)) {
            ASTNode* dayNode = parseDay(tokens);
            if (dayNode) {
                printf("Debug: Adding day node: %s to plan node: %s\n", dayNode->data.day.name, planNode->data.plan.name);
                addASTChildNode(planNode, dayNode);
            } else {
                free(clientName);
                freeAST(planNode);
                fprintf(stderr, "Debug: Error in parsing day node\n");
                return NULL;
            }
        } else {
            printf("Debug: Skipping unexpected token: %d\n", (**tokens)->type);
            (*tokens)++;
        }
    }
    printf("Debug: Consumed closing brace '}'\n");
    (*tokens)++; // Consume closing brace '}'

    // Expecting a semicolon at the end of the assignment
    if ((**tokens)->type != TOKEN_SEMICOLON) {
        fprintf(stderr, "Debug: Error - Expected semicolon, got %s\n", (**tokens)->value);
        freeAST(planNode);
        free(clientName);
        return NULL;
    }
    printf("Debug: Consumed semicolon\n");
    (*tokens)++; // Consume semicolon

    // Create client node
    ASTNode* clientNode = createASTNode(NODE_CLIENT_PROFILE, clientName, 0);
    printf("Debug: Created client node with name: %s\n", clientName);
    free(clientName);

    // Create assignment node and link client and plan
    ASTNode* assignmentNode = createASTNode(NODE_ASSIGNMENT, NULL, 0);
    assignmentNode->data.assignment.plan = planNode;
    assignmentNode->data.assignment.client = clientNode;
    printf("Debug: Linking plan node and client node to assignment node\n");

    printf("Debug: Exiting parseAssignment with plan - %s and client - %s\n",
        planNode->data.plan.name, clientNode->data.clientProfile.name);
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


/***
 * AST printing functions
*/

// Print the AST
void printAST(ASTNode* node, int depth) {
    if (!node) return;

    // Indentation for better readability
    for (int i = 0; i < depth; i++) {
        printf("|   ");
    }

    // Print node information based on its type
    switch (node->type) {
        case NODE_MAIN:
            printf("Main Program\n");
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
        case NODE_ASSIGNMENT:
            printf("Assignment: Client - %s, Plan - %s\n", 
                node->data.assignment.client->data.clientProfile.name, 
                node->data.assignment.plan->data.plan.name);
            break;
        case NODE_SHOW_PLANS:
            printf("ShowPlans for: %s\n", node->data.showPlans.clientName);
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
        // Add other cases as necessary
        default:
            printf("Unknown Node\n");
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
    "        exercise: \"squats\" | sets: 3 | rest: 1\n"
    "        exercise: \"leg press\" | sets: 3 | rest: 1\n"
    "    }\n"
    "    Tuesday {\n"
    "        exercise: \"bench press\" | sets: 3 | rest: 1\n"
    "    }\n"
    "};";

    char* sourceCode2 = 
    "blah blah";



    // Assuming readSourceCode and lexer are defined and implemented
    Token** tokens;

    // Create symbol table
    struct SymbolTable* table = createSymbolTable();

    // Test case 1
    tokens = lexer(sourceCode2);
    //printf("Tokens generated by lexer for Test Case 1:\n");
    //for (int i = 0; tokens[i] != NULL; i++) {
    //    printf("Token %d: type = %d, value = %s\n", i, tokens[i]->type, tokens[i]->value);
    //}
    ASTNode* ast1 = parseProgram(tokens);
    printf("AST generated by parser for Test Case 1:\n");
    if (ast1 == NULL) {
        fprintf(stderr, "Error in parsing Test Case 1.\n");
    } else {
        printAST(ast1, 0);
    }

    printf("Performing semantic analysis for Test Case 1...\n");
    int result = performSemanticAnalysis(ast1, table);
    if (result == 100) {
        printf("No errors found in semantic analysis for Test Case 1.\n");
    } else {
        printf("Errors found in semantic analysis for Test Case 1.\n");
    }

    return EXIT_SUCCESS;
}