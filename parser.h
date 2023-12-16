#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

// AST node types
typedef enum {
    NODE_PROGRAM,
    NODE_BINARY_OP,
    NODE_DECLARATION,
    NODE_ASSIGNMENT,
    NODE_FUNCTION_CALL,
    NODE_IF_STATEMENT,
    NODE_FOREACH_STATEMENT,
    NODE_TRY_CATCH_BLOCK,
    NODE_BLOCK,
    NODE_EXPRESSION,
    NODE_IDENTIFIER,
    NODE_LITERAL_STRING,
    NODE_LITERAL_INT,
    NODE_LITERAL_FLOAT
} NodeType;

typedef struct ASTNode ASTNode;

struct ASTNode {
    NodeType type;
    union {
        struct {
            ASTNode* left;
            char* op;
            ASTNode* right;
        } binary_op;
        struct {
            char* identifier;
        } declaration;
        struct {
            ASTNode* target;
            ASTNode* value;
        } assignment;
        struct {
            char* identifier;
            ASTNode** arguments;
            int num_arguments;
        } function_call;
        struct {
            ASTNode* condition;
            ASTNode* true_branch;
            ASTNode* false_branch;
        } if_statement;
        struct {
            ASTNode* identifier;
            ASTNode* iterable;
            ASTNode* block;
        } foreach_statement;
        struct {
            ASTNode* try_block;
            ASTNode* catch_identifier;
            ASTNode* catch_block;
        } try_catch_block;
        struct {
            ASTNode* expression;
        } expression;
        struct {
            char* value;
        } identifier;
        struct {
            char* value;
        } literal_string;
        struct {
            int value;
        } literal_int;
        struct {
            float value;
        } literal_float;
        struct {
            ASTNode* statements;
        } block;
    } data;
    struct ASTNode** children;  // Array of child nodes
    int num_children; // for linking statements in a block
};

// Function prototypes
ASTNode* createFunctionCallNode(const char* identifier, ASTNode** arguments, int num_arguments);
ASTNode* createIfStatementNode(ASTNode* condition, ASTNode* true_branch, ASTNode* false_branch);
ASTNode* addASTChild(ASTNode* parent, ASTNode* child);
void freeASTNode(ASTNode* node);

#endif // PARSER_H