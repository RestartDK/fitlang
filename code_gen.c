#include "code_gen.h"
#include <stdio.h>
#include <stdlib.h>

// Initialize the code generation context
void initCodeGenContext(CodeGenContext* context, TargetCodeType targetType) {
    context->code = (char*)malloc(MAX_CODE_LENGTH * sizeof(char));
    context->length = 0;
    context->targetType = targetType;
}

// Free the resources used by the code generation context
void freeCodeGenContext(CodeGenContext* context) {
    if (context != NULL) {
        if (context->code != NULL) {
            free(context->code);
            context->code = NULL;
        }
        context->length = 0;
        context->targetType = TARGET_BYTECODE; // or any default value
    }
}
// Append generated code to the context

void appendCode(CodeGenContext* context, const char* code) {
    int codeLength = strlen(code);
    if (context->length + codeLength >= MAX_CODE_LENGTH) {
        // The new code won't fit in the current buffer, so we need to resize it
        int newLength = context->length + codeLength + 1; // +1 for the null terminator
        char* newCode = realloc(context->code, newLength * sizeof(char));
        if (newCode == NULL) {
            // Handle memory allocation error
            fprintf(stderr, "Error: Unable to allocate memory for code buffer.\n");
            return;
        }
        context->code = newCode;
    }
    // Append the new code to the end of the current code
    strcat(context->code, code);
    context->length += codeLength;
}


void generateNode(struct ASTNode* node, CodeGenContext* context) {
    if (node == NULL) {
        return;
    }

    char buffer[128]; // Buffer to hold generated code snippet

    switch (node->type) {
        case NODE_CLIENT_PROFILE:
            snprintf(buffer, sizeof(buffer), "CreateClientProfile('%s');\n", node->data.clientProfile.name);
            appendCode(context, buffer);
            break;

        case NODE_EXERCISE:
            snprintf(buffer, sizeof(buffer), "AddExercise('%s', %d, %d);\n", node->data.exercise.name, node->data.exercise.sets, node->data.exercise.rest);
            appendCode(context, buffer);
            break;

        case NODE_PLAN:
            snprintf(buffer, sizeof(buffer), "CreatePlan('%s');\n", node->data.plan.name);
            appendCode(context, buffer);
            break;

        case NODE_ASSIGNMENT:
            snprintf(buffer, sizeof(buffer), "AssignPlan('%s', '%s');\n", node->data.assignment.client->data.clientProfile.name, node->data.assignment.plan->data.plan.name);
            appendCode(context, buffer);
            break;
        case NODE_SHOW_PLANS:
            snprintf(buffer, sizeof(buffer), "ShowPlans('%s');\n", node->data.showPlans.clientName);
            appendCode(context, buffer);
            break;
        case NODE_DAY:
            snprintf(buffer, sizeof(buffer), "AddDay('%s');\n", node->data.day.name);
            appendCode(context, buffer);
            break;
        case NODE_LITERAL:
            snprintf(buffer, sizeof(buffer), "AddLiteral(%d);\n", node->data.literal.value);
            appendCode(context, buffer);
            break;

        default:
            // Handle default or unknown cases
            break;
    }

    // Recursively generate code for child nodes
    for (int i = 0; i < node->childrenCount; i++) {
        generateNode(node->children[i], context);
    }
}

// High-level function to start code generation
void generateCode(struct ASTNode* root) {
    if (root == NULL) {
        return;
    }

    CodeGenContext context;
    initCodeGenContext(&context, TARGET_C_CODE); // Example: generating C-like code

    generateNode(root, &context);

    printf("Generated Code:\n%s", context.code);

    freeCodeGenContext(&context);
}
