# Semantic Analysis

Semantic analysis in FitLang is a crucial step that ensures the source code is meaningful and adheres to the logical rules of the language. This phase involves inspecting the Abstract Syntax Tree (AST) to enforce type compatibility, scope rules, and other semantic constraints.

### Type Checking

*   **Compatibility and Coercion**: FitLang enforces type compatibility in operations and assignments. Implicit type coercion is handled where appropriate.



    ```c
    void checkTypeCompatibility(Node* node) {
        if (node->type == NodeType_Assignment) {
            DataType leftType = getVariableType(node->left);
            DataType rightType = evaluateExpressionType(node->right);

            if (!areTypesCompatible(leftType, rightType)) {
                reportError("Type mismatch in assignment", node->lineNumber);
            }
        }
    }
    ```

### Scope Checking

*   **Variable Declaration and Usage**: Ensures variables are declared within their scope and used appropriately.



    ```c
    void validateVariableScope(Node* node) {
        if (node->type == NodeType_Variable) {
            if (!isVariableDeclaredInScope(node->variableName)) {
                reportError("Variable used before declaration", node->lineNumber);
            }
        }
    }
    ```

### Function and Procedure Semantics

*   **Function Calls**: Validates that functions are called with the correct number and types of arguments.



    ```c
    void validateFunctionCall(Node* functionCallNode) {
        FunctionDefinition* functionDef = getFunctionDefinition(functionCallNode->functionName);
        
        if (functionDef->paramCount != functionCallNode->arguments->length) {
            reportError("Incorrect number of arguments in function call", functionCallNode->lineNumber);
        }

        // Additional code to check argument types
    }
    ```

### Error Reporting

*   **Descriptive Errors**: Provides clear and informative error messages.



    ```c
    void reportError(const char* message, int lineNumber) {
        printf("Semantic Error at line %d: %s\n", lineNumber, message);
    }
    ```

### Integration with AST

*   **Annotations and Modifications**: Additional information, such as data types and scope, is annotated on the AST nodes during semantic analysis.



    ```c
    void annotateAstWithSemanticInfo(Node* ast) {
        // Traverse and annotate the AST
        foreach (Node* node in ast) {
            if (node->type == NodeType_VariableDeclaration) {
                annotateVariableNode(node);
            }
            // Other annotations based on node type
        }
    }
    ```
