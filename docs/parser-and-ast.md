# Parser and AST

The FitLang parser transforms a sequence of tokens, provided by the lexer, into an Abstract Syntax Tree (AST). This AST represents the hierarchical structure of FitLang's source code, showing how elements like expressions, statements, and constructs are related.

### AST Structure

* AST nodes in FitLang correspond to language constructs: variables, control structures, functions, and expressions.
* Each node represents a specific construct with pointers to its child nodes. For instance, a `Function` node will have child nodes for its parameters and body.

### Parsing Process

* The parser identifies the token type and calls the appropriate parsing function.
* Functions like `parseExpression`, `parseStatement`, and `parseFunction` are recursive, allowing nested structures.

### Error Handling

* FitLang's parser has error handling for syntax issues, providing descriptive messages and location information.

### Creating the AST

* Specific functions are used to construct AST nodes based on the GymLang constructs.

### Example code snippet: Parsing a function

```c
Node* parseFunction() {
    // Assuming currentToken() gives the current token and advanceToken() moves to the next
    consume(TokenType_Function, "Expected 'function'");
    char* functionName = currentToken().value;
    advanceToken();

    // Parse parameters and function body
    Node* parameters = parseParameters();
    Node* body = parseFunctionBody();

    return createFunctionNode(functionName, parameters, body);
}
```

### Example code snippet: Parsing an IF statement

```c
Node* parseIfStatement() {
    consume(TokenType_If, "Expected 'if'");
    Node* condition = parseExpression();

    Node* thenBlock = parseBlock();
    Node* elseBlock = NULL;
    if (currentToken().type == TokenType_Else) {
        advanceToken();
        elseBlock = parseBlock();
    }

    return createIfNode(condition, thenBlock, elseBlock);
}
```

### Lexer Integration

* The parser requests tokens from the lexer, ensuring a seamless transition from lexical to syntax analysis.

### Output

* The parser produces a well-structured AST, ready for interpretation.

```bash
Debug: parseProgram - Starting
Debug: Creating ASTNode. Type: 0, NODE_MAIN
Debug: Current token type: 0, value: ClientProfile
Debug: Found TOKEN_CLIENT_PROFILE
Debug: parseClientProfile - Starting
Debug: parseClientProfile - Consumed TOKEN_CLIENT_PROFILE
Debug: parseClientProfile - Consumed TOKEN_IDENTIFIER: Daniel
Debug: Creating ASTNode. Type: 1, NODE_CLIENT_PROFILE, Name: Daniel
Debug: parseClientProfile - Consumed TOKEN_SEMICOLON
Debug: parseClientProfile - Finished
Debug: Adding child node to root
Debug: Current token type: 1, value: assign
Debug: Found TOKEN_ASSIGN
Debug: Entering parseAssignment
Debug: Consumed 'assign' token
Debug: Plan identifier: muscleBuildingPlan
Debug: Consumed 'to' token
Debug: Client identifier: Daniel
Debug: Consumed opening brace '{'
Debug: Creating ASTNode. Type: 2, NODE_PLAN, Name: muscleBuildingPlan
Debug: Created plan node with name: muscleBuildingPlan
Debug: parseDay - Starting
Debug: parseDay - Consumed day token: Monday
Debug: Creating ASTNode. Type: 4, NODE_DAY, Name: Monday
Debug: parseDay - Consumed left brace
Debug: parseDay - Parsing exercise
Debug: parseExercise - Starting
Debug: parseExercise - Exercise Name: squats
Debug: parseExercise - Found TOKEN_PIPE
Debug: parseExercise - Sets: 3
Debug: parseExercise - Found TOKEN_PIPE
Debug: parseExercise - Rest: 1
Debug: Creating ASTNode. Type: 3, NODE_EXERCISE, Name: squats
Debug: parseExercise - Exiting
Debug: Adding exercise node: squats to day node: Monday
Debug: parseDay - Parsing exercise
Debug: parseExercise - Starting
Debug: parseExercise - Exercise Name: leg press
Debug: parseExercise - Found TOKEN_PIPE
Debug: parseExercise - Sets: 3
Debug: parseExercise - Found TOKEN_PIPE
Debug: parseExercise - Rest: 1
Debug: Creating ASTNode. Type: 3, NODE_EXERCISE, Name: leg press
Debug: parseExercise - Exiting
Debug: Adding exercise node: leg press to day node: Monday
Debug: parseDay - Consumed right brace
Debug: parseDay - Finished
Debug: Adding day node: Monday to plan node: muscleBuildingPlan
Debug: parseDay - Starting
Debug: parseDay - Consumed day token: Tuesday
Debug: Creating ASTNode. Type: 4, NODE_DAY, Name: Tuesday
Debug: parseDay - Consumed left brace
Debug: parseDay - Parsing exercise
Debug: parseExercise - Starting
Debug: parseExercise - Exercise Name: bench press
Debug: parseExercise - Found TOKEN_PIPE
Debug: parseExercise - Sets: 3
Debug: parseExercise - Found TOKEN_PIPE
Debug: parseExercise - Rest: 1
Debug: Creating ASTNode. Type: 3, NODE_EXERCISE, Name: bench press
Debug: parseExercise - Exiting
Debug: Adding exercise node: bench press to day node: Tuesday
Debug: parseDay - Consumed right brace
Debug: parseDay - Finished
Debug: Adding day node: Tuesday to plan node: muscleBuildingPlan
Debug: Consumed closing brace '}'
Debug: Consumed semicolon
Debug: Creating ASTNode. Type: 1, NODE_CLIENT_PROFILE, Name: Daniel
Debug: Created client node with name: Daniel
Debug: Creating ASTNode. Type: 9, NODE_ASSIGNMENT
Debug: Linking plan node and client node to assignment node
Debug: Exiting parseAssignment with plan - muscleBuildingPlan and client - Daniel
Debug: Adding child node to root
Debug: Exiting parseProgram
AST generated by parser for Test Case 1:
Program
Debug: Node at depth 0, type 0 has 2 children
  ClientProfile: Daniel
Debug: Node at depth 1, type 1 has 0 children
  Assignment: Client - Daniel, Plan - muscleBuildingPlan
Debug: Node at depth 1, type 9 has 0 children
```
