#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>

// Define the types of nodes that can appear in the AST
typedef enum {
    NODE_PROGRAM,        // Represents the entire program
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

typedef struct {
    ASTNode* program;
} ASTProgram;

// Define identifier node
typedef struct {
    char* name; // Name of the identifier
} ASTIdentifier;

// Structures for different node types
typedef struct {
    char* name; // Name of the client
} ASTClientProfile;

typedef struct {
    ASTNode* client;
} ASTShowPlans;

typedef struct {
    ASTNode* client;
    ASTNode* plan;
} ASTAssignment;

typedef struct {
    char* name; // Name of the day
    ASTNode** exercises;
    int exercisesCount;
} ASTDay;

typedef struct {
    char* name; // Name of the exercise
    int sets;
    int rest;
} ASTExercise;

typedef struct {
    char* name; // Name of the day
    ASTNode* exercises;
} ASTDayName;

typedef struct {
    char* name; // Name of the plan
    ASTNode* client; // Pointer to the corresponding client node
    ASTNode* days; // Points to the days within the plan
} ASTPlan;

typedef struct {
    char* value; // Value of the literal
} ASTLiteral;

// Tagged union to represent node data
typedef union {
    ASTProgram program;
    ASTIdentifier identifier;
    ASTClientProfile clientProfile;
    ASTAssignment assignment;
    ASTDay day;
    ASTExercise exercise;
    ASTShowPlans showPlans;
    ASTDayName dayName;
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
ASTNode* createASTNode(NodeType type, const char* value);
void addASTChildNode(ASTNode* parent, ASTNode* child);
void freeAST(ASTNode* root);

#endif