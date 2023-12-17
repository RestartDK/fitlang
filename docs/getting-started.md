# Getting Started

Based on the structure of the provided FitLang program, we'll create a getting started guide for a similar but distinct program. Our example will define a client profile for a different client and assign them a weight loss workout plan.

## Getting Started with Fitlang: Weight Loss Workout Plan



### Step 1: Setting up the Environment

Make sure the FitLang interpreter is installed and choose a text editor for writing FitLang code.



### Step 2: Writing a Program

In your text editor, create a new FitLang program following these steps:

1.  **Define a Client Profile**: Create a profile for a new client.

    ```fitlang
    ClientProfile Emily;
    ```
2.  **Assign a Workout Plan**: Define a weight loss workout plan for the client. Use the FitLang structure to specify workouts for each day.

    ```fitlang
    assign weightLossPlan Emily {
        Monday {
            exercise: "cardio" | duration: 30 | intensity: "high"
        }
        Wednesday {
            exercise: "cycling" | duration: 20 | intensity: "medium"
        }
        Friday {
            exercise: "yoga" | duration: 40 | intensity: "low"
        }
    };
    ```

    This code creates a workout plan named `weightLossPlan` for the client Emily. The plan includes different exercises for Monday, Wednesday, and Friday, each with specified duration and intensity.
3.  **Display the Workout Plans**: Add a command to show the assigned workout plans.

    ```fitlang
    showPlans(Emily);
    ```

    The `showPlans` function displays Emily's workout schedule as defined in the `weightLossPlan`.
4. **Save the Program**: Name your file something descriptive, like `weightLossWorkout.fitlang`.



### Step 3: Running the Program

To execute your FitLang program:

1. **Open the Command Line**: Access your computer's command line or terminal.
2. **Navigate to Your File**: Change to the directory where your `weightLossWorkout.fitlang` file is located.
3.  **Run the Program**: Use the FitLang interpreter to run your program:

    ```
    weightLossWorkout.fl
    ```
4. **View the Output**: The output in the terminal will display Emily's weight loss workout plan for the week.



### Conclusion

You've now created and run a FitLang program for a weight loss workout plan. This exercise introduces you to defining client profiles, assigning workout plans with specific exercises and parameters, and using functions to display information. As you get more comfortable with these concepts, you can explore creating more tailored and complex workout plans in FitLang.
