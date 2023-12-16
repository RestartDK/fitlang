#include "parser.h"
#include "lexer.h"

ASTNode* parseClientProfile(Token* token) {
    // Expect and consume the "ClientProfile" keyword
    // Expect and consume an identifier, which is the client's name
    // Construct and return a ClientProfile node
}

ASTNode* parseExercise(Token* token) {
    // Expect and consume the "exercise" keyword
    // Expect and consume a string literal, which is the exercise name
    // Expect and consume the "sets" keyword
    // Expect and consume an integer literal, which is the number of sets
    // Expect and consume the "rest" keyword
    // Expect and consume an integer literal, which is the rest time
    // Construct and return an Exercise node
}

ASTNode* parseDay(Token* token) {
    // Expect and consume an identifier, which is the day of the week
    // Expect and consume a left brace
    // Parse exercises until a right brace is encountered
    // Expect and consume a right brace
    // Construct and return a Day node
}

ASTNode* parseAssignment(Token* token) {
    // Expect and consume the "assign" keyword
    // Expect and consume an identifier, which is the plan name
    // Expect and consume an identifier, which is the client's name
    // Expect and consume a left brace
    // Parse days until a right brace is encountered
    // Expect and consume a right brace
    // Construct and return an Assignment node
}

ASTNode* parseShowPlans(Token* token) {
    // Expect and consume the "showPlans" keyword
    // Expect and consume a left parenthesis
    // Expect and consume an identifier, which is the client's name
    // Expect and consume a right parenthesis
    // Construct and return a ShowPlans node
}

ASTNode* parseStatement(Token** tokens) {
    Token token = lexerNextToken(tokens);
    switch (token.type) {
        case TOKEN_CLIENT_PROFILE:
            return parseClientProfile(lexer);
        case TOKEN_ASSIGN:
            return parseAssignment(lexer);
        case TOKEN_SHOW_PLANS:
            return parseShowPlans(lexer);
        // ... handle other statement types
    }
}

// Main parsing function
ASTNode* parse(Token* Token) {
    // Create a new AST node for the root of the tree
    ASTNode* root = createASTNode(AST_ROOT);

    // Parse the program...
    while (lexer_hasNextToken(lexer)) {
        Token token = lexer_nextToken(lexer);

        switch (token.type) {
            case TOKEN_CLIENT_PROFILE:
                addASTChildNode(root, parseClientProfile(lexer));
                break;
            case TOKEN_PLAN:
                addASTChildNode(root, parsePlan(lexer));
                break;
            // ... handle other token types
        }
    }

    return root;
}