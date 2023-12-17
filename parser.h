#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>

// Define the types of nodes that can appear in the AST
typedef enum {
    NODE_MAIN,        // Represents the entire program
    NODE_CLIENT_PROFILE, // Represents a client profile
    NODE_PLAN,           // Represents a training plan
    NODE_EXERCISE,       // Represents an exercise within a plan
    NODE_DAY,            // Represents a day within a plan (e.g., Monday)
    NODE_SHOW_PLANS,     // Represents a showPlans command
    NODE_SETS,           // Represents the sets in an exercise
    NODE_REST,           // Represents the rest time in an exercise
    NODE_LITERAL,        // Represents a literal value (e.g., "squats", number of sets)
    NODE_ASSIGNMENT,     // Represents an assignment of a plan to a client
    NODE_IDENTIFIER,     // Represents an identifier (e.g., "John")
} NodeType;

// Define the structure of an AST node
typedef struct ASTNode ASTNode;

// Define the structure of the AST
typedef struct {
    // For NODE_MAIN, might not need additional data if it's just a container for the top-level elements
} ASTMain;

typedef struct {
    ASTNode* client; // Points to the client node for an assignment
    ASTNode* plan;   // Points to the plan node for an assignment
} ASTAssignment;

typedef struct {
    char* name; // Client name for NODE_CLIENT_PROFILE
} ASTClientProfile;

typedef struct {
    char* name; // Plan name for NODE_PLAN
} ASTPlan;

typedef struct {
    char* name; // Exercise name for NODE_EXERCISE
    int sets;
    int rest;
} ASTExercise;

typedef struct {
    char* name; // Day name for NODE_DAY
} ASTDay;

typedef struct {
    char* clientName; // Client name for NODE_SHOW_PLANS
} ASTShowPlans;

typedef struct {
    int value; // Numeric value for NODE_LITERAL
} ASTLiteral;

typedef struct {
    char* name; // Identifier name for NODE_IDENTIFIER
} ASTIdentifier;


// Tagged union to represent node data
typedef union {
    ASTMain main;
    ASTIdentifier identifier;
    ASTClientProfile clientProfile;
    ASTAssignment assignment;
    ASTDay day;
    ASTExercise exercise;
    ASTShowPlans showPlans;
    ASTPlan plan;
    ASTLiteral literal;
} ASTNodeData;

// Definition of AST node
struct ASTNode {
    NodeType type;
    ASTNodeData data;
    ASTNode** children;
    int childrenCount;
};

// Function declarations for creating and manipulating AST nodes
ASTNode* createASTNode(NodeType type, const char* value, int intValue);
void addASTChildNode(ASTNode* parent, ASTNode* child);
void freeAST(ASTNode* root);

#endif