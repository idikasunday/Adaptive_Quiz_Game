// Fill out your copyright notice in the Description page of Project Settings.

#include "QuizLogActor.h" // Include the header file for our class
#include "Logging/LogMacros.h" // Required for UE_LOG

// Define a custom log category (optional but good practice)
DEFINE_LOG_CATEGORY_STATIC(LogQuiz, Log, All); // Category name, Default verbosity, Compile-time verbosity

// Sets default values
AQuizLogActor::AQuizLogActor()
{
    // Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false; // We don't need Tick for this example

    // Initialize the quiz with default data when the actor is constructed
    InitializeQuizData();
}

// Called when the game starts or when spawned
void AQuizLogActor::BeginPlay()
{
    Super::BeginPlay(); // Call the parent class's BeginPlay

    // Log the quiz data to the output log
    LogQuizData();
}

// Initialize the QuizQuestions array with sample data
void AQuizLogActor::InitializeQuizData()
{
    QuizQuestions.Empty(); // Clear any existing data (e.g., if edited in editor and then constructor runs)

    // --- Question 1 ---
    TArray<FString> Answers1;
    Answers1.Add(TEXT("Paris"));
    Answers1.Add(TEXT("London"));
    Answers1.Add(TEXT("Berlin"));
    Answers1.Add(TEXT("Madrid"));
    QuizQuestions.Add(FQuizQuestion(TEXT("What is the capital of France?"), Answers1, 0)); // Correct answer is index 0 ("Paris")

    // --- Question 2 ---
    TArray<FString> Answers2;
    Answers2.Add(TEXT("int"));
    Answers2.Add(TEXT("string"));
    Answers2.Add(TEXT("FString"));
    Answers2.Add(TEXT("FName"));
    QuizQuestions.Add(FQuizQuestion(TEXT("Which data type is typically used for user-facing text in Unreal Engine C++?"), Answers2, 2)); // Correct answer is index 2 ("FString")

    // --- Question 3 ---
    TArray<FString> Answers3;
    Answers3.Add(TEXT("Blueprints"));
    Answers3.Add(TEXT("C++"));
    Answers3.Add(TEXT("Materials"));
    Answers3.Add(TEXT("Sequencer"));
    QuizQuestions.Add(FQuizQuestion(TEXT("Which Unreal Engine system is primarily used for visual scripting?"), Answers3, 0)); // Correct answer is index 0 ("Blueprints")

    // --- Question 4 ---
    TArray<FString> Answers4;
    Answers4.Add(TEXT("Mercury"));
    Answers4.Add(TEXT("Venus"));
    Answers4.Add(TEXT("Earth"));
    Answers4.Add(TEXT("Mars"));
    QuizQuestions.Add(FQuizQuestion(TEXT("Which is the third planet from the Sun?"), Answers4, 2)); // Correct answer is index 2 ("Earth")
}

// Logs the current quiz data to the Unreal Engine Output Log
void AQuizLogActor::LogQuizData()
{
    UE_LOG(LogQuiz, Log, TEXT("--- Starting Quiz Data Log ---")); // Use our custom category "LogQuiz"

    if (QuizQuestions.Num() == 0)
    {
        UE_LOG(LogQuiz, Warning, TEXT("No quiz questions found in the QuizQuestions array."));
        return;
    }

    // Iterate through each question in the array
    for (int32 i = 0; i < QuizQuestions.Num(); ++i)
    {
        const FQuizQuestion& Question = QuizQuestions[i]; // Get a reference to the current question

        // Log the question number and text
        UE_LOG(LogQuiz, Log, TEXT("Question %d: %s"), i + 1, *Question.QuestionText); // Add 1 for human-readable numbering, use * to get C-style string from FString

        // Log the possible answers
        for (int32 j = 0; j < Question.Answers.Num(); ++j)
        {
            // Optionally indicate the correct answer in the log
            FString CorrectIndicator = (j == Question.CorrectAnswerIndex) ? TEXT(" (Correct)") : TEXT("");
            UE_LOG(LogQuiz, Log, TEXT("  Answer %d: %s%s"), j + 1, *Question.Answers[j], *CorrectIndicator);
        }

        // Log the index of the correct answer (optional, but good for verification)
        if (Question.CorrectAnswerIndex >= 0 && Question.CorrectAnswerIndex < Question.Answers.Num())
        {
            UE_LOG(LogQuiz, Log, TEXT("  Correct Answer Index: %d (%s)"), Question.CorrectAnswerIndex, *Question.Answers[Question.CorrectAnswerIndex]);
        }
        else
        {
            UE_LOG(LogQuiz, Warning, TEXT("  Invalid Correct Answer Index: %d"), Question.CorrectAnswerIndex);
        }
        UE_LOG(LogQuiz, Log, TEXT("--------------------")); // Separator between questions
    }

    UE_LOG(LogQuiz, Log, TEXT("--- Finished Quiz Data Log ---"));
}

// We are not using Tick in this example
// void AQuizLogActor::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
// }