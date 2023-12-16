#include <stdlib.h>
#include "parser.h"
#include "lexer.h"
#include <stdbool.h>

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
        case NODE_SETS:
            free(root->data.exercise.sets);
            break;
        case NODE_REST:
            free(root->data.exercise.rest);
            break;
        case NODE_LITERAL:
            free(root->data.literal.value);
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
    ASTNode* node = malloc(sizeof(ASTNode));
    if (!node) return NULL;

    node->type = type;
    node->children = NULL;
    node->childrenCount = 0;

    // Initialize node-specific data
    switch (type) {
        case NODE_CLIENT_PROFILE:
            node->data.clientProfile.name = strdup(value);
            break;
        case NODE_PLAN:
            node->data.plan.name = strdup(value);
            break;
        case NODE_DAY:
            node->data.day.name = strdup(value);
            break;
        case NODE_EXERCISE:
            node->data.exercise.name = strdup(value);
            break;
        case NODE_SHOW_PLANS:
            node->data.showPlans.client = strdup(value);
            break;
        case NODE_SETS:
            node->data.exercise.sets= strdup(value);
            break;
        case NODE_REST:
            node->data.exercise.rest = strdup(value);
            break;
        case NODE_LITERAL:
            node->data.literal.value = strdup(value);
            break;
        case NODE_ASSIGNMENT:
            node->data.assignment.client = strdup(value);
            break;
        // Add cases for other types as needed
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

ASTNode* parsePlan(Token** tokens, int* position) {
    if (tokens[*position]->type != TOKEN_ASSIGN) {
        // Error handling: Expected 'assign' token
        return NULL;
    }
    (*position)++;

    // Assuming the next token is the plan name
    if (tokens[*position]->type != TOKEN_IDENTIFIER) {
        // Error handling: Expected plan name as identifier
        return NULL;
    }
    char* planName = strdup(tokens[*position]->value);
    (*position)++;

    // Next token should be the client identifier
    if (tokens[*position]->type != TOKEN_IDENTIFIER) {
        // Error handling: Expected client identifier
        free(planName);
        return NULL;
    }
    char* clientName = strdup(tokens[*position]->value);
    (*position)++;

    // Create plan node
    ASTNode* planNode = createASTNode(NODE_PLAN, planName);
    free(planName);

    // Optionally, you could link the plan to a client node here

    // Parse each day within the plan
    while (tokens[*position]->type == TOKEN_MONDAY || /* ... other days ... */) {
        ASTNode* dayNode = parseDay(tokens, position);
        if (dayNode == NULL) {
            // Error handling
            free(clientName);
            freeAST(planNode);
            return NULL;
        }
        addASTChildNode(planNode, dayNode);
    }

    // Optionally, handle linking the plan to other parts of the AST

    return planNode;
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


ASTNode* parseShowPlans(Token** tokens, int* position) {
    if (tokens[*position]->type != TOKEN_SHOW_PLANS) {
        // Error handling: Expected 'showPlans' token
        return NULL;
    }
    (*position)++;

    // Parse client identifier for showPlans
    // ...

    ASTNode* showPlansNode = createASTNode(NODE_SHOW_PLANS, "clientName"); // Replace with actual client name

    return showPlansNode;
}

