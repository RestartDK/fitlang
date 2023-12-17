# Language Specifications

FitLang, designed for fitness trainers and gym management, provides a specialized syntax and set of constructs to facilitate the creation and management of fitness routines and gym operations. The language is built with a focus on readability and ease of use.

### Data Types

* **Basic Types**:
  * `int`: Integer type for numeric values without decimals.
  * `float`: Floating-point type for numeric values with decimals.
  * `string`: Represents textual data.
* **Custom Types**:
  * `ClientProfile`: Represents a gym client, holding data like age, goal, and fitness level.
  * `WorkoutPlan`: Encapsulates a workout plan, including exercises, sets, reps, and days.

### Variables

* Variables are declared with a type followed by an identifier, e.g., `int age;`.
* Assignment is straightforward: `age = 25;`.

### Data Structures

* **Arrays/Lists**: Used to represent a collection of elements, e.g., `string[] days = ["Monday", "Wednesday", "Friday"];`.
* **Dictionaries/Maps**: Key-value pairs for complex data relationships, e.g., `map<string, int> exerciseDuration;`.

### Control Structures

* **`If` Statements**: Standard conditional execution.
  * Syntax: `If (condition) { ... }`
* **`ForEach` Loop**: Iterates over elements of a collection.
  * Syntax: `ForEach (element in collection) { ... }`

### Functions and Procedures

* Functions are defined to perform specific tasks.
* Syntax: `Function <Name>(<Parameters>) { ... }`.
* Example: `Function CreateStrengthRoutine(ClientProfile client) { ... }`.

### Special Commands

* **Profile and Workout Management**:
  * `UpdateProfile`: Updates client information.
  * `LogWorkout`: Logs a workout session.
* **Display Commands**:
  * `ShowProgress`: Displays the progress of a client.
  * `Output`: Generic output command for displaying text.

### Input/Output

* **Input**: Used to receive data from the user.
  * Syntax: `Input("Enter your age: ");`
* **Output**: Displays information on the screen.
  * Syntax: `Output(client.name + " has made great progress!");`

### Syntax Rules

* **Expressions**: FitLang supports basic arithmetic and logical expressions.
* **Comments**: Single-line (`//`) and multi-line (`/* ... */`) comments are supported for annotating code.
* **Error Handling**: FitLang provides clear error messages for syntax and runtime errors.

### Semantics

* **Type Safety**: FitLang enforces type safety, ensuring that operations are performed on compatible types.
* **Scope**: Variables and functions have defined scopes. Variables declared inside a function are not accessible outside of it.

### Examples

*   **Variable Declaration and Assignment**:

    ```gym
    int clientCount = 10;
    float weight = 72.5;
    string name = "John Doe";
    ```
*   **Function Definition and Usage**:

    ```gym
    Function PlanWorkout(WorkoutPlan plan) {
        Output("Workout Plan for " + plan.days.length + " days");
        // Additional logic here
    }
    ```
*   **Control Structure Usage**:

    ```gym
    If (client.goal == "WeightLoss") {
        // Logic for weight loss routine
    }
    ForEach (day in workoutPlan.days) {
        Output("Workout scheduled on " + day);
    }
    ```

This detailed specification provides the foundational understanding of FitLang's syntax and semantics, guiding users in writing and understanding FitLang code effectively.
