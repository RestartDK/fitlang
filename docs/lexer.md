# Lexer

The lexer in FitLang serves as the first crucial component in the process of translating source code into a format that can be easily processed by the interpreter. It takes raw FitLang source code as input and breaks it down into smaller, meaningful units known as "tokens." These tokens are the fundamental building blocks of the language and represent keywords, identifiers, literals, and symbols.



### Tokenization Process

The tokenization process involves scanning the input source code character by character and recognizing patterns that match specific token types. Here's an overview of how the lexer performs tokenization:

1. **Character-by-Character Scanning**: The lexer reads the source code character by character, starting from the beginning.
2. **Skipping Whitespace**: Whitespace characters (such as spaces, tabs, and newline characters) are skipped and not considered tokens. This step ensures that spaces and formatting do not affect the tokenization process.
3. **Identifying Keywords and Identifiers**: The lexer identifies keywords and identifiers, which are essential elements in FitLang. Keywords are predefined words in the language, while identifiers are user-defined names for variables, functions, or other elements. FitLang distinguishes keywords and identifiers using rules like:
   * Keywords must start with an alphabetical character or an underscore.
   * Identifiers can consist of letters, digits, and underscores.
4. **Recognizing Numeric and String Literals**: FitLang supports numeric literals (integers and floats) and string literals (enclosed in double quotes). The lexer recognizes and extracts these literals from the source code.
5. **Handling Symbols**: Symbols, such as punctuation marks and operators, are recognized as individual tokens. These symbols include braces, colons, pipes, and other characters used for structuring FitLang code.
6. **Error Handling**: The lexer is equipped to handle unexpected characters or patterns gracefully. It can report errors when it encounters characters or combinations of characters that do not conform to the FitLang syntax.



### Token Types

FitLang defines a range of token types to represent different elements within the language. Each token type has a specific purpose and is identified by the lexer during tokenization. Here are some of the core token types used in FitLang:

* **TOKEN\_CLIENT\_PROFILE**: Represents the declaration of a client profile.
* **TOKEN\_ASSIGN**: Indicates assignment operations.
* **TOKEN\_LEFT\_BRACE** and **TOKEN\_RIGHT\_BRACE**: Represent opening and closing braces for defining blocks.
* **TOKEN\_COLON**: Denotes the separation between identifiers and their values.
* **TOKEN\_PIPE**: Used to separate different properties within FitLang commands.
* **TOKEN\_IDENTIFIER**: Represents user-defined names, such as variable names or function names.
* **TOKEN\_STRING\_LITERAL**: Represents string literals enclosed in double quotes.
* **TOKEN\_INT\_LITERAL**: Represents integer literals (e.g., for specifying repetitions, sets, or other numerical values).
* **TOKEN\_SHOW\_PLANS**: Represents the command to display workout plans.
* **TOKEN\_DAYS**: Represents the days of the week (e.g., Monday, Tuesday) used in workout plans.



```c
#include <string.h>
#include <stdlib.h>

// Token types
typedef enum {
    TOKEN_CLIENT_PROFILE,
    TOKEN_ASSIGN,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_COLON,
    TOKEN_PIPE,
    TOKEN_IDENTIFIER,
    TOKEN_STRING_LITERAL,
    TOKEN_EXERCISE,
    TOKEN_SETS,
    TOKEN_REST,
    TOKEN_INT_LITERAL,
    TOKEN_SHOW_PLANS,
    TOKEN_MONDAY,
    TOKEN_TUESDAY,
    TOKEN_WEDNESDAY,
    TOKEN_THURSDAY,
    TOKEN_FRIDAY,
    TOKEN_SATURDAY,
    TOKEN_SUNDAY
} TokenType;

// Token structure
typedef struct {
    TokenType type;  
    char* value;     
} Token;

#endif
```

_File lexer.h showing the various token types_



### Regular Expressions

The lexer employs regular expressions to match and recognize different token types efficiently. These regular expressions define the patterns that tokens must adhere to, making it possible to identify tokens accurately within the source code.

For example, the regular expression for identifying keywords like "assign" or "Monday" would be defined to match the exact word and distinguish it from other text in the source code.



### Tokenization Output

```bash
Tokens generated by lexer:
Token 0: type = 1, value = assign
Token 1: type = 6, value = muscleBuildingPlan
Token 2: type = 6, value = Daniel
Token 3: type = 2, value = {
Token 4: type = 13, value = Monday
Token 5: type = 2, value = {
Token 6: type = 8, value = exercise
Token 7: type = 4, value = :
Token 8: type = 7, value = squats
Token 9: type = 5, value = |
Token 10: type = 9, value = sets
Token 11: type = 4, value = :
Token 12: type = 11, value = 3
Token 13: type = 5, value = |
Token 14: type = 10, value = rest
Token 15: type = 4, value = :
Token 16: type = 11, value = 1
Token 17: type = 3, value = }
Token 18: type = 3, value = }
```
