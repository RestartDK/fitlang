#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.c"

// AST node types
typedef enum {
    NODE_PROGRAM,
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

ASTNode* createProgramNode();
ASTNode* parseStatement(Token** tokens);
ASTNode* parseIfStatement(Token** tokens);
ASTNode* parseForeachStatement(Token** tokens);
ASTNode* parseTryCatchBlock(Token** tokens);
ASTNode* parseFunctionCall(Token** tokens);
ASTNode* parseAssignment(Token** tokens);
ASTNode* parseExpression(Token** tokens);
Token* peekNextToken(Token** tokens);
ASTNode* parseBlock(Token** tokens);
ASTNode* createDeclarationNode(const char* identifier);
ASTNode* addASTChild(ASTNode* parent, ASTNode* child);
ASTNode* parseTerm(Token** tokens);
ASTNode* createBinaryOpNode(char* op, ASTNode* left, ASTNode* right);
ASTNode* createIntNode(int value);
void freeTokens(Token** tokens);

// Function to create a new AST node for identifier
ASTNode* createIdentifierNode(const char* value) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    node->type = NODE_IDENTIFIER;
    node->data.identifier.value = strdup(value);
    node->children = NULL;
    node->num_children = 0;
    return node;
}

// Function to create a new AST node for string literal
ASTNode* createStringLiteralNode(const char* value) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    node->type = NODE_LITERAL_STRING;
    node->data.literal_string.value = strdup(value);
    node->children = NULL;
    node->num_children = 0;
    return node;
}

// Function to create a new AST node for integer literal
ASTNode* createIntLiteralNode(int value) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    node->type = NODE_LITERAL_INT;
    node->data.literal_int.value = value;
    node->children = NULL;
    node->num_children = 0;
    return node;
}

// Function to create a new AST node for float literal
ASTNode* createFloatLiteralNode(float value) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    node->type = NODE_LITERAL_FLOAT;
    node->data.literal_float.value = value;
    node->children = NULL;
    node->num_children = 0;
    return node;
}

ASTNode* createDeclarationNode(const char* identifier) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    node->type = NODE_DECLARATION;
    node->data.declaration.identifier = strdup(identifier);
    node->children = NULL;
    node->num_children = 0;
    return node;
}

// Function to create a new AST node for an assignment
ASTNode* createAssignmentNode(ASTNode* target, ASTNode* value) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    node->type = NODE_ASSIGNMENT;
    node->data.assignment.target = target;
    node->data.assignment.value = value;
    node->children = NULL;
    node->num_children = 0;
    return node;
}

// Function to create a new AST node for a function call
ASTNode* createFunctionCallNode(const char* identifier, ASTNode** arguments, int num_arguments) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    node->type = NODE_FUNCTION_CALL;
    node->data.function_call.identifier = strdup(identifier);
    node->data.function_call.arguments = arguments;
    node->data.function_call.num_arguments = num_arguments;
    node->children = NULL;
    node->num_children = 0;
    return node;
}

// Function to create a new AST node for an if statement
ASTNode* createIfStatementNode(ASTNode* condition, ASTNode* true_branch, ASTNode* false_branch) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    node->type = NODE_IF_STATEMENT;
    node->data.if_statement.condition = condition;
    node->data.if_statement.true_branch = true_branch;
    node->data.if_statement.false_branch = false_branch;
    node->children = NULL;
    node->num_children = 0;
    return node;
}

// Function to create a new AST node for a foreach statement
ASTNode* createForeachStatementNode(ASTNode* identifier, ASTNode* iterable, ASTNode* block) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    node->type = NODE_FOREACH_STATEMENT;
    node->data.foreach_statement.identifier = identifier;
    node->data.foreach_statement.iterable = iterable;
    node->data.foreach_statement.block = block;
    node->children = NULL;
    node->num_children = 0;
    return node;
}

// Function to create a new AST node for a try-catch block
ASTNode* createTryCatchBlockNode(ASTNode* try_block, ASTNode* catch_identifier, ASTNode* catch_block) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    node->type = NODE_TRY_CATCH_BLOCK;
    node->data.try_catch_block.try_block = try_block;
    node->data.try_catch_block.catch_identifier = catch_identifier;
    node->data.try_catch_block.catch_block = catch_block;
    node->children = NULL;
    node->num_children = 0;
    return node;
}

// Function to create a new AST node for a block
ASTNode* createBlockNode(ASTNode* statement_list) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    node->type = NODE_BLOCK;
    // The statement_list is assumed to be a linked list of statements
    node->data.block.statements = statement_list;
    node->children = NULL;
    node->num_children = 0;
    return node;
}

// Function to create a new AST node for an expression
ASTNode* createExpressionNode(ASTNode* sub_expression) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    node->type = NODE_EXPRESSION;
    node->data.expression.expression = sub_expression;
    node->children = NULL;
    node->num_children = 0;
    return node;
}

//to make the freeAST function you gave in the previousanswer shorter don't forget to use the function freeASTNode who already handle the case depending on the type of the node:'

ASTNode* addASTChild(ASTNode* parent, ASTNode* child) {
    parent->children = (ASTNode**)realloc(parent->children, (parent->num_children + 1) * sizeof(ASTNode*));
    if (parent->children == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    parent->children[parent->num_children] = child;
    parent->num_children++;
    return child;
}
void freeASTNode(ASTNode* node) {
    if (node == NULL) {
        return;
    }
    
    // Free any child nodes or resources held by this node
    switch (node->type) {
        case NODE_DECLARATION:
            free(node->data.declaration.identifier);
            break;
        case NODE_ASSIGNMENT:
            freeASTNode(node->data.assignment.target);
            freeASTNode(node->data.assignment.value);
            break;
        case NODE_FUNCTION_CALL:
            free(node->data.function_call.identifier);
            for (int i = 0; i < node->data.function_call.num_arguments; ++i) {
                freeASTNode(node->data.function_call.arguments[i]);
            }
            free(node->data.function_call.arguments);
            break;
        case NODE_IF_STATEMENT:
            freeASTNode(node->data.if_statement.condition);
            freeASTNode(node->data.if_statement.true_branch);
            freeASTNode(node->data.if_statement.false_branch);
            break;
        case NODE_FOREACH_STATEMENT:
            freeASTNode(node->data.foreach_statement.identifier);
            freeASTNode(node->data.foreach_statement.iterable);
            freeASTNode(node->data.foreach_statement.block);
            break;
        case NODE_TRY_CATCH_BLOCK:
            freeASTNode(node->data.try_catch_block.try_block);
            freeASTNode(node->data.try_catch_block.catch_identifier);
            freeASTNode(node->data.try_catch_block.catch_block);
            break;
        case NODE_BLOCK:
            freeASTNode(node->data.block.statements);
            break;
        case NODE_EXPRESSION:
            freeASTNode(node->data.expression.expression);
            break;
        case NODE_IDENTIFIER:
            free(node->data.identifier.value);
            break;
        case NODE_LITERAL_STRING:
            free(node->data.literal_string.value);
            break;
        case NODE_LITERAL_INT:
            // No dynamic memory to free for integers
            break;
        case NODE_LITERAL_FLOAT:
            // No dynamic memory to free for floats
            break;
        // ... (other cases as needed)
    }
    
    // Free the next node in the chain if there is one
    for (int i = 0; i < node->num_children; ++i) {
        freeASTNode(node->children[i]);
    }
    free(node->children);
    
    // Finally, free the node itself
    free(node);
}
/*
Function to parse program 
*/

ASTNode* parseProgram(Token** tokens) {
    ASTNode* root = createProgramNode();
    while ((*tokens)->type != TOKEN_EOF) {
        ASTNode* child = parseStatement(tokens);
        addASTChild(root, child);
    }
    return root;
}


ASTNode* parseDeclaration(Token** tokens) {
    if ((*tokens)->type == TOKEN_KEYWORD) {
        char* type = strdup((*tokens)->value); // Copy the type string
        (*tokens)++; // Move to the next token, which should be the identifier
        if ((*tokens)->type == TOKEN_IDENTIFIER) {
            char* identifier = strdup((*tokens)->value); // Copy the identifier string
            (*tokens)++; // Move past the identifier
            ASTNode* declarationNode = createDeclarationNode(identifier);
            free(type);
            free(identifier);
            return declarationNode;
        }
        free(type);
    }
    // Error handling for unexpected token type
    return NULL;
}

ASTNode* parseStatement(Token** tokens) {
    if ((*tokens)->type == TOKEN_KEYWORD) {
        if (strcmp((*tokens)->value, "ClientProfile") == 0 || strcmp((*tokens)->value, "WorkoutPlan") == 0) {
            return parseDeclaration(tokens);
        } else if (strcmp((*tokens)->value, "If") == 0) {
            return parseIfStatement(tokens);
        } else if (strcmp((*tokens)->value, "ForEach") == 0) {
            return parseForeachStatement(tokens);
        } else if (strcmp((*tokens)->value, "Try") == 0) {
            return parseTryCatchBlock(tokens);
        }
        // Additional keyword-based rules
        else if (strcmp((*tokens)->value, "UpdateProfile") == 0 ||
                 strcmp((*tokens)->value, "LogWorkout") == 0 ||
                 strcmp((*tokens)->value, "ShowProgress") == 0 ||
                 strcmp((*tokens)->value, "SaveExcel") == 0) {
            return parseFunctionCall(tokens); // Assuming these are function calls
        }
        // Add other specific keyword handling as required
    } else if ((*tokens)->type == TOKEN_IDENTIFIER) {
        // Check the next token to decide if it's an assignment or function call
        Token* nextToken = peekNextToken(tokens); // Assumes a function to peek at the next token
        if (nextToken != NULL && nextToken->type == TOKEN_PUNCTUATION && strcmp(nextToken->value, "=") == 0) {
            return parseAssignment(tokens);
        } else if (nextToken != NULL && nextToken->type == TOKEN_PUNCTUATION && strcmp(nextToken->value, "(") == 0) {
            return parseFunctionCall(tokens);
        }
    }
    // Error handling: unknown statement type
    return NULL;
}

ASTNode* parseAssignment(Token** tokens) {
    if ((*tokens)->type == TOKEN_IDENTIFIER) {
        char* identifier = strdup((*tokens)->value); // Copy the identifier string
        (*tokens)++; // Move past the identifier
        if ((*tokens)->type == TOKEN_PUNCTUATION && strcmp((*tokens)->value, "=") == 0) {
            (*tokens)++; // Move past the '='
            ASTNode* target = createIdentifierNode(identifier);
            ASTNode* value = parseExpression(tokens);
            ASTNode* assignmentNode = createAssignmentNode(target, value);
            free(identifier);
            return assignmentNode;
        }
        free(identifier);
    }
    // Error handling for unexpected token type
    return NULL;
}

ASTNode* parseFunctionCall(Token** tokens) {
    if ((*tokens)->type == TOKEN_IDENTIFIER) {
        char* identifier = strdup((*tokens)->value); // Copy the identifier string
        (*tokens)++; // Move past the identifier
        if ((*tokens)->type == TOKEN_PUNCTUATION && strcmp((*tokens)->value, "(") == 0) {
            (*tokens)++; // Move past the '('
            ASTNode** arguments = NULL;
            int num_arguments = 0;
            if ((*tokens)->type != TOKEN_PUNCTUATION || strcmp((*tokens)->value, ")") != 0) {
                arguments = (ASTNode**)realloc(arguments, sizeof(ASTNode*));
                arguments[num_arguments++] = parseExpression(tokens);
                while ((*tokens)->type == TOKEN_PUNCTUATION && strcmp((*tokens)->value, ",") == 0) {
                    (*tokens)++; // Move past the ','
                    arguments = (ASTNode**)realloc(arguments, sizeof(ASTNode*) * num_arguments);
                    arguments[num_arguments++] = parseExpression(tokens);
                }
            }
            if ((*tokens)->type == TOKEN_PUNCTUATION && strcmp((*tokens)->value, ")") == 0) {
                (*tokens)++; // Move past the ')'
                ASTNode* functionCallNode = createFunctionCallNode(identifier, arguments, num_arguments);
                free(identifier);
                return functionCallNode;
            }
            // Error handling for unexpected token type
            free(identifier);
            return NULL;
        }
        free(identifier);
    }
    // Error handling for unexpected token type
    return NULL;
}

ASTNode* parseIfStatement(Token** tokens) {
    if ((*tokens)->type == TOKEN_KEYWORD && strcmp((*tokens)->value, "If") == 0) {
        (*tokens)++; // Move past the 'If'
        ASTNode* condition = parseExpression(tokens);
        ASTNode* true_branch = parseBlock(tokens);
        ASTNode* false_branch = NULL;
        if ((*tokens)->type == TOKEN_KEYWORD && strcmp((*tokens)->value, "Else") == 0) {
            (*tokens)++; // Move past the 'Else'
            false_branch = parseBlock(tokens);
        }
        ASTNode* ifStatementNode = createIfStatementNode(condition, true_branch, false_branch);
        return ifStatementNode;
    }
    // Error handling for unexpected token type
    return NULL;
}

ASTNode* parseForeachStatement(Token** tokens) {
    if ((*tokens)->type == TOKEN_KEYWORD && strcmp((*tokens)->value, "ForEach") == 0) {
        (*tokens)++; // Move past the 'ForEach'
        if ((*tokens)->type == TOKEN_IDENTIFIER) {
            char* identifier = strdup((*tokens)->value); // Copy the identifier string
            (*tokens)++; // Move past the identifier
            if ((*tokens)->type == TOKEN_KEYWORD && strcmp((*tokens)->value, "in") == 0) {
                (*tokens)++; // Move past the 'in'
                ASTNode* iterable = parseExpression(tokens);
                ASTNode* block = parseBlock(tokens);
                ASTNode* foreachStatementNode = createForeachStatementNode(identifier, iterable, block);
                free(identifier);
                return foreachStatementNode;
            }
            free(identifier);
        }
    }
    // Error handling for unexpected token type
    return NULL;
}

ASTNode* parseTryCatchBlock(Token** tokens) {
    if ((*tokens)->type == TOKEN_KEYWORD && strcmp((*tokens)->value, "Try") == 0) {
        (*tokens)++; // Move past the 'Try'
        ASTNode* try_block = parseBlock(tokens);
        if ((*tokens)->type == TOKEN_KEYWORD && strcmp((*tokens)->value, "Catch") == 0) {
            (*tokens)++; // Move past the 'Catch'
            if ((*tokens)->type == TOKEN_IDENTIFIER) {
                char* identifier = strdup((*tokens)->value); // Copy the identifier string
                (*tokens)++; // Move past the identifier
                ASTNode* catch_block = parseBlock(tokens);
                ASTNode* tryCatchBlockNode = createTryCatchBlockNode(try_block, identifier, catch_block);
                free(identifier);
                return tryCatchBlockNode;
            }
        }
    }
    // Error handling for unexpected token type
    return NULL;
}


ASTNode* parseBlock(Token** tokens) {
    if ((*tokens)->type == TOKEN_PUNCTUATION && strcmp((*tokens)->value, "{") == 0) {
        (*tokens)++; // Move past the '{'
        ASTNode* statement_list = NULL;
        while ((*tokens)->type != TOKEN_PUNCTUATION || strcmp((*tokens)->value, "}") != 0) {
            ASTNode* statement = parseStatement(tokens);
            if (statement != NULL) {
                statement_list = addASTChild(statement_list, statement);
            }
        }
        (*tokens)++; // Move past the '}'
        ASTNode* blockNode = createBlockNode(statement_list);
        return blockNode;
    }
    // Error handling for unexpected token type
    return NULL;
}

ASTNode* parseExpression(Token** tokens) {
    ASTNode* left = parseTerm(tokens);
    while ((*tokens)->type == TOKEN_OPERATOR) {
        char* op = strdup((*tokens)->value);
        (*tokens)++; // Move past the operator
        ASTNode* right = parseTerm(tokens);
        if (right == NULL) {
            printf("Error: Expected a term after operator\n");
            exit(1);
        }
        left = createBinaryOpNode(op, left, right);
    }
    return left;
}

ASTNode* parseTerm(Token** tokens) {
    if ((*tokens)->type == TOKEN_INTEGER) {
        int value = atoi((*tokens)->value);
        (*tokens)++; // Move past the integer
        return createIntNode(value);
    }
    // Error handling for unexpected token type
    return NULL;
}

Token* peekNextToken(Token** tokens) {
    return *(tokens + 1);
}

int main() {
    // A simple test program
    char* program = "ForEach item in list { Try { print(item); } Catch error { print(error); } }";

    // Tokenize the program
    Token** tokens = tokenize(program);
    if (tokens == NULL) {
        printf("Lexing failed\n");
        return 1;
    }

    // Parse the token stream
    ASTNode* ast = parseProgram(&tokens);
    if (ast == NULL) {
        printf("Parsing failed\n");
        return 1;
    }

    // Print the AST


    // Clean up
    freeAST(ast);
    freeTokens(tokens);

    return 0;
}