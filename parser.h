#ifndef AST_H
#define AST_H

#include <stdlib.h>

// Define the types of nodes that can appear in the AST
typedef enum {
    NODE_CLIENT_PROFILE, // Represents a client profile
    NODE_PLAN,           // Represents a training plan
    NODE_EXERCISE,       // Represents an exercise within a plan
    NODE_DAY,            // Represents a day within a plan (e.g., Monday)
    NODE_SHOW_PLANS,     // Represents a showPlans command
    NODE_SETS,           // Represents the sets in an exercise
    NODE_REST,           // Represents the rest time in an exercise
    NODE_LITERAL,        // Represents a literal value (e.g., "squats", number of sets)
    NODE_ASSIGNMENT,     // Represents an assignment of a plan to a client
} NodeType;

// Define the structure of an AST node
typedef struct ASTNode ASTNode;

// Enumeration for different types of AST nodes
typedef enum {
    AST_CLIENT_PROFILE,
    AST_ASSIGNMENT,
    AST_DAY,
    AST_EXERCISE,
    AST_SHOW_PLANS,
    // ... other node types
} ASTNodeType;

// Structures for different node types
typedef struct {
    char* name; // For storing names like client names, exercise names, etc.
} ASTClientProfile;

typedef struct {
    ASTNode* client;
    ASTNode* plan;
} ASTAssignment;

typedef struct {
    ASTNode** exercises;
    int exercisesCount;
} ASTDay;

typedef struct {
    char* name; // Name of the exercise
    int sets;
    int rest;
} ASTExercise;

typedef struct {
    ASTNode* client;
} ASTShowPlans;

typedef struct {
    char* name;
    ASTNode* exercises;
} ASTDayName;

typedef struct {
    char* name;
    ASTNode* days;
} ASTPlan;

// Tagged union to represent node data
typedef union {
    ASTClientProfile clientProfile;
    ASTAssignment assignment;
    ASTDay day;
    ASTExercise exercise;
    ASTShowPlans showPlans;
    ASTDayName dayName;
    ASTPlan plan;

} ASTNodeData;

// Definition of AST node
struct ASTNode {
    ASTNodeType type;
    ASTNodeData data;
    ASTNode** children;
    int childrenCount;
};


// Function declarations for creating and manipulating AST nodes
ASTNode* createASTNode(NodeType type, const char* value);
void addASTChildNode(ASTNode* parent, ASTNode* child);
void freeAST(ASTNode* root);

#endif
