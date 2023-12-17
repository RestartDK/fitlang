# Language Design

The design of FitLang is rooted in simplicity, user-friendliness, and the specific needs of fitness trainers and gym enthusiasts. This section delves into the key design choices and the rationale behind them.



### Design Goals

FitLang's language design aligns with the following goals:

* **Simplicity**: Keep the language syntax straightforward and easy to learn for non-programmers.
* **Expressiveness**: Allow users to express fitness-related concepts and tasks using natural language constructs.
* **Versatility**: Make FitLang versatile enough to accommodate a variety of fitness goals and training scenarios.
* **Error Handling**: Implement user-friendly error messages and graceful error handling to assist users in correcting issues.
* **Efficiency**: Enable fitness trainers to create workout plans efficiently and facilitate the tracking of client progress.



### Key Language Constructs

The previous goals were achieved through the following language constructs:\


* **Client Profiles**: Users can define client profiles using the `ClientProfile` keyword. This allows fitness trainers to create individualized workout plans for clients with specific fitness goals and requirements.

```
ClientProfile clientName;
```

* **Workout Plans**: FitLang allows the creation of workout plans using the `WorkoutPlan` keyword. These plans specify the days of the week and the exercises to be performed on each day.

```
WorkoutPlan planName;
planName.days = ["Monday", "Wednesday", "Friday"];
```

* **Basic Data Types**: FitLang keeps data types simple with `name`, `set`, `rep`, `time`, `int`, `string`, and `float`. These data types cover essential fitness-related information such as exercise names, sets, repetitions, and time.



* **Arrays/Lists and Dictionaries/Maps**: Arrays are used to hold series of exercises or sets, while dictionaries are employed to map exercises to specific details. This simplifies the organization of workout-related data.

```
Exercise[] legExercises = [squat, legPress, calfRaise];
Map<Exercise, Details> workoutMap;
```

* **Control Structures**: FitLang offers support for control structures such as loops and conditionals. For example, a `ForEach` loop allows iteration through exercises, and `If` conditionals enable customization of workout plans based on client conditions.

```
ForEach (exercise in legExercises) {
    Perform(exercise);
}

If (clientName.goal == "WeightLoss") {
    AssignCardioRoutine();
}
```

* **Functions and Procedures**: The language enables the definition of routines or common tasks using functions. Fitness trainers can create custom functions to streamline workout plan creation.

```
Function CreateStrengthRoutine(ClientProfile client) {
    /* Define routine */
}
```

* **Special Commands**: FitLang includes special commands like `UpdateProfile` for modifying client profiles, `LogWorkout` for recording workout sessions, `ShowProgress` for visualizing client progress, and `SaveExcel` for exporting workout plans to Excel files.

```
UpdateProfile(clientName, newDetails);
LogWorkout(clientName, workoutDetails);
ShowProgress(clientName);
SaveExcel(planName);
```

* **Input/Output**: The language provides simple input and output commands, making it possible to interact with users, read client data, or generate reports.

```
Input("Enter client weight: ");
Output(clientName.progressReport);
```

####

### Future Enhancements

While FitLang's initial design focuses on simplicity and essential fitness-related features, future enhancements may include:

* Support for more complex data structures and algorithms.
* Integration with fitness tracking devices and apps.
* Enhanced reporting and visualization capabilities.

In conclusion, FitLang's language design reflects a commitment to simplicity, usability, and effectiveness in addressing the needs of fitness trainers and enthusiasts. The language's minimalistic syntax and fitness-oriented constructs make it a valuable tool for creating and managing workout plans.
