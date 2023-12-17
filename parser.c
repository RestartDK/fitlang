#include <stdlib.h>
#include "parser.h"
#include "lexer.h"
#include <stdbool.h>
#include <stdio.h>
#include <err.h>
#include "lexer.c"

void freeAST(ASTNode* root) {
    if (!root) return;

    // Free children recursively
    for (int i = 0; i < root->childrenCount; i++) {
        freeAST(root->children[i]);
    }

    // Free the children array
    free(root->children);

    // Free node-specific data
    switch (root->type) {
        case NODE_PROGRAM:
            break;
        case NODE_CLIENT_PROFILE:
            free(root->data.clientProfile.name);
            break;
        case NODE_PLAN:
            free(root->data.plan.name);
            break;
        case NODE_DAY:
            free(root->data.day.name);
            break;
        case NODE_EXERCISE:
            free(root->data.exercise.name);
            break;
        case NODE_SHOW_PLANS:
            free(root->data.showPlans.client);
            break;
        case NODE_LITERAL:
            free(root->data.literal.value);
            break;
        case NODE_IDENTIFIER:
            free(root->data.identifier.name);
            break;
        case NODE_ASSIGNMENT:
            free(root->data.assignment.client);
            break;
        // Add cases for other types as needed
    }

    // Finally, free the node itself
    free(root);
}

void addASTChildNode(ASTNode* parent, ASTNode* child) {
    if (!parent || !child) return;

    // Resize children array to accommodate new child
    parent->childrenCount++;
    parent->children = realloc(parent->children, sizeof(ASTNode*) * parent->childrenCount);
    parent->children[parent->childrenCount - 1] = child;
}

ASTNode* createASTNode(NodeType type, const char* value) {
    printf("Creating node with type: %d, value: %s\n", type, value);

    ASTNode* node = malloc(sizeof(ASTNode));
    if (!node) {
        printf("Failed to allocate memory for node\n");
        return NULL;
    }

    node->type = type;
    node->children = NULL;
    node->childrenCount = 0;

    // Initialize node-specific data
    switch (type) {
        case NODE_PROGRAM:
            printf("Creating NODE_PROGRAM\n");
            break;
        case NODE_CLIENT_PROFILE:
            node->data.clientProfile.name = strdup(value);
            printf("Creating NODE_CLIENT_PROFILE with name: %s\n", node->data.clientProfile.name);
            break;
        case NODE_IDENTIFIER:
            node->data.identifier.name = strdup(value);
            printf("Creating NODE_IDENTIFIER with name: %s\n", node->data.identifier.name);
            break;
        case NODE_PLAN:
            node->data.plan.name = strdup(value);
            printf("Creating NODE_PLAN with name: %s  For client: %s\n", node->data.plan.name, node->data.clientProfile.name);
            break;
        case NODE_DAY:
            node->data.day.name = strdup(value);
            printf("Creating NODE_DAY with name: %s\n", node->data.day.name);
            break;
        case NODE_EXERCISE:
            node->data.exercise.name = strdup(value);
            printf("Creating NODE_EXERCISE with name: %s\n", node->data.exercise.name);
            break;
        case NODE_SHOW_PLANS:
            // Allocate memory for the client node within showPlans
            node->data.showPlans.client = malloc(sizeof(ASTClientProfile));
            if (node->data.showPlans.client == NULL) {
                printf("Failed to allocate memory for showPlans client\n");
                free(node); // Don't forget to free the allocated node
                return NULL;
            }
            // Now it's safe to duplicate the value
            node->data.showPlans.client = strdup(value);
            printf("Creating NODE_SHOW_PLANS with client name: %s\n", node->data.showPlans.client);
            break;
        case NODE_SETS:
            node->data.exercise.sets = atoi(value);
            printf("Creating NODE_SETS with sets: %d\n", node->data.exercise.sets);
            break;
        case NODE_REST:
            node->data.exercise.rest = atoi(value);
            printf("Creating NODE_REST with rest: %d\n", node->data.exercise.rest);
            break;
        case NODE_LITERAL:
            node->data.literal.value = strdup(value);
            printf("Creating NODE_LITERAL with value: %s\n", node->data.literal.value);
            break;
        case NODE_ASSIGNMENT:
            node->data.assignment.client = strdup(value);
            node->data.assignment.plan = strdup(value);
            printf("Creating NODE_ASSIGNMENT with client name: %s\n", node->data.assignment.client);
            break;
        // Add similar print statements for other cases
    }

    return node;
}

ASTNode* parseClientProfile(Token** tokens, int* position) {
    if (tokens[*position]->type != TOKEN_CLIENT_PROFILE) {
        // Error handling: Expected client profile token
        return NULL;
    }
    (*position)++;

    if (tokens[*position]->type != TOKEN_IDENTIFIER) {
        // Error handling: Expected identifier after client profile
        return NULL;
    }

    ASTNode* clientProfileNode = createASTNode(NODE_CLIENT_PROFILE, tokens[*position]->value);
    (*position)++;

    return clientProfileNode;
}

bool isDayToken(Token* token) {
    if (!token) return false;

    switch (token->type) {
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

ASTNode* parseExercise(Token** tokens, int* position) {
    if (tokens[*position]->type != TOKEN_EXERCISE) {
        // Error handling: Expected 'exercise' token
        return NULL;
    }
    (*position)++;

    // Parse the exercise name
    if (tokens[*position]->type != TOKEN_STRING_LITERAL) {
        // Error handling: Expected a string literal for exercise name
        return NULL;
    }
    char* exerciseName = strdup(tokens[*position]->value);
    (*position)++;

    // Create exercise node
    ASTNode* exerciseNode = createASTNode(NODE_EXERCISE, exerciseName);
    free(exerciseName);

    // Initial values for sets and rest
    int sets = 0, rest = 0;

    // Parse sets
    if (tokens[*position]->type == TOKEN_SETS) {
        (*position)++;
        if (tokens[*position]->type != TOKEN_INT_LITERAL) {
            // Error handling: Expected an integer literal for sets
            freeAST(exerciseNode);
            return NULL;
        }
        sets = atoi(tokens[*position]->value); // Convert string to integer
        (*position)++;
    }

    // Parse rest
    if (tokens[*position]->type == TOKEN_REST) {
        (*position)++;
        if (tokens[*position]->type != TOKEN_INT_LITERAL) {
            // Error handling: Expected an integer literal for rest
            freeAST(exerciseNode);
            return NULL;
        }
        rest = atoi(tokens[*position]->value); // Convert string to integer
        (*position)++;
    }

    // Add sets and rest to exerciseNode's data
    exerciseNode->data.exercise.sets = sets;
    exerciseNode->data.exercise.rest = rest;

    return exerciseNode;
}

ASTNode* parseDay(Token** tokens, int* position) {
    // Ensure the current token is a day
    if (!isDayToken(tokens[*position])) {
        // Error handling: Expected a day token (e.g., TOKEN_MONDAY)
        return NULL;
    }

    char* dayName = strdup(tokens[*position]->value); // Get the day's name
    (*position)++;

    ASTNode* dayNode = createASTNode(NODE_DAY, dayName);
    free(dayName);

    // Parse exercises for the day
    while (tokens[*position]->type == TOKEN_EXERCISE) {
        ASTNode* exerciseNode = parseExercise(tokens, position);
        if (exerciseNode == NULL) {
            // Error handling
            freeAST(dayNode);
            return NULL;
        }
        addASTChildNode(dayNode, exerciseNode);
    }

    return dayNode;
}

ASTNode* parsePlan(Token** tokens, int* position, ASTNode* clientsList) {
    if (tokens[*position]->type != TOKEN_ASSIGN) {
        // Error handling: Expected 'assign' token
        return NULL;
    }
    (*position)++;

    // Parsing plan name
    if (tokens[*position]->type != TOKEN_IDENTIFIER) {
        // Error handling: Expected plan name as identifier
        return NULL;
    }
    char* planName = strdup(tokens[*position]->value);
    (*position)++;

    // Parse the client identifier
    if (tokens[*position]->type != TOKEN_IDENTIFIER) {
        // Error handling: Expected client identifier
        free(planName);
        return NULL;
    }
    char* clientName = strdup(tokens[*position]->value);
    (*position)++;

    // Find the client node
    ASTNode* clientNode = findClientNode(clientsList, clientName);
    free(clientName);
    if (!clientNode) {
        // Error handling: Client not found
        free(planName);
        return NULL;
    }

    // Create plan node and link it to the client node
    ASTNode* planNode = createASTNode(NODE_PLAN, planName);
    planNode->data.plan.client = clientNode;
    free(planName);

    // Parse each day within the plan
    while (isDayToken(tokens[*position])) {
        ASTNode* dayNode = parseDay(tokens, position);
        if (dayNode == NULL) {
            // Error handling
            freeAST(planNode);
            return NULL;
        }
        addASTChildNode(planNode, dayNode);
    }

    return planNode;
}


ASTNode* parseShowPlans(Token** tokens, int* position) {
    if (tokens[*position]->type != TOKEN_SHOW_PLANS) {
        // Error handling: Expected 'showPlans' token
        return NULL;
    }
    (*position)++;

    // Parse client identifier
    if (tokens[*position]->type != TOKEN_IDENTIFIER) {
        // Error handling: Expected identifier after 'showPlans'
        return NULL;
    }

    ASTNode* showPlansNode = createASTNode(NODE_SHOW_PLANS, tokens[*position]->value);
    (*position)++;

    return showPlansNode;
}

ASTNode* parseIdentifier(Token** tokens, int* position) {
    if (tokens[*position]->type != TOKEN_IDENTIFIER) {
        // Error handling: Expected identifier
        return NULL;
    }
    ASTNode* identifierNode = createASTNode(NODE_IDENTIFIER, tokens[*position]->value);
    (*position)++;
    return identifierNode;
}

ASTNode* parseAssignment(Token** tokens, int* position) {
    // Parse client identifier
    ASTNode* clientNode = parseIdentifier(tokens, position);
    if (!clientNode) {
        // Error handling: Expected identifier after 'assign'
        return NULL;
    }

    // Parse plan identifier
    ASTNode* planNode = parseIdentifier(tokens, position);
    if (!planNode) {
        // Error handling: Expected identifier after client identifier
        freeAST(clientNode);
        return NULL;
    }

    // Create assignment node
    ASTNode* assignmentNode = createASTNode(NODE_ASSIGNMENT, NULL);
    assignmentNode->data.assignment.client = clientNode;
    assignmentNode->data.assignment.plan = planNode;

    return assignmentNode;
}

ASTNode* parseLiteral(Token** tokens, int* position) {
    if (tokens[*position]->type != TOKEN_STRING_LITERAL && tokens[*position]->type != TOKEN_INT_LITERAL) {
        // Error handling: Expected string or integer literal
        return NULL;
    }

    ASTNode* literalNode = createASTNode(NODE_LITERAL, tokens[*position]->value);
    (*position)++;

    return literalNode;
}

ASTNode* parseProgram(Token** tokens) {
    ASTNode* root = createASTNode(NODE_PROGRAM, NULL);
    int position = 0;

    while (tokens[position] != NULL) {
        ASTNode* node = NULL;

        switch (tokens[position]->type) {
            case TOKEN_CLIENT_PROFILE:
                node = parseClientProfile(tokens, &position);
                break;
            case TOKEN_ASSIGN:
                node = parsePlan(tokens, &position);
                break;
            case TOKEN_SHOW_PLANS:
                node = parseShowPlans(tokens, &position);
                break;
            default:
                // Handle unexpected tokens, potentially logging an error or skipping them
                break;
        }

        if (node != NULL) {
            addASTChildNode(root, node);
        } else {
            // Error handling: Free partial AST, display error, etc.
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
        case NODE_PROGRAM:
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
            printf("Assignment: Client - %s\n", node->data.plan.name);
            break;
        // Add more cases for other node types
        // For example:
        case NODE_SETS:
            printf("Sets: %d\n", node->data.exercise.sets);
            break;
        case NODE_REST:
            printf("Rest: %d\n", node->data.exercise.rest);
            break;
        case NODE_LITERAL:
            printf("Literal: %s\n", node->data.literal.value);
            break;
    }

    // Recursively print child nodes
    for (int i = 0; i < node->childrenCount; i++) {
        printAST(node->children[i], depth + 1);
    }
}

int main() {
    // Assuming readSourceCode and lexer are defined and implemented
    char* sourceCode = "ClientProfile Daniel;\n"
                       "assign muscleBuildingPlan Daniel {\n"
                       "    Monday {\n"
                       "        exercise: \"squats\" | sets: 3 | rest: 1\n"
                       "        exercise: \"leg press\" | sets: 3 | rest: 1\n"
                       "    }\n"
                       "    Tuesday {\n"
                       "        exercise: \"bench press\" | sets: 3 | rest: 1\n"
                       "    }\n"
                       "};\n"
                       "showPlans(Daniel);";

    Token** tokens = lexer(sourceCode);
    printf("Tokens generated by lexer:\n"); // Debug print
    for (int i = 0; tokens[i] != NULL; i++) {
        printf("Token %d: type = %d, value = %s\n", i, tokens[i]->type, tokens[i]->value); // Debug print
    }

    // Parse the tokens to construct the AST
    ASTNode* ast = parseProgram(tokens);
    printf("AST generated by parser:\n"); // Debug print

    if (ast == NULL) {
        fprintf(stderr, "Error in parsing the program.\n");
    } else {
        // Pretty print the AST for debugging
        printAST(ast, 0);
    }

    // Cleanup
    freeAST(ast);

    return ast != NULL ? EXIT_SUCCESS : EXIT_FAILURE;
}