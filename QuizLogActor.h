// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuizLogActor.generated.h" // Must be the last include

// Define a structure to hold a single quiz question
// USTRUCT allows this structure to be recognized by Unreal's reflection system
// BlueprintType makes it potentially usable in Blueprints later
USTRUCT(BlueprintType)
struct FQuizQuestion
{
    GENERATED_BODY()

public:
    // The text of the question
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quiz Question")
    FString QuestionText;

    // An array of possible answers
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quiz Question")
    TArray<FString> Answers;

    // The index (in the Answers array) of the correct answer
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quiz Question")
    int32 CorrectAnswerIndex;

    // Default constructor
    FQuizQuestion()
        : QuestionText(TEXT("")), CorrectAnswerIndex(-1)
    {
    }

    // Convenience constructor
    FQuizQuestion(const FString& InQuestion, const TArray<FString>& InAnswers, int32 InCorrectIndex)
        : QuestionText(InQuestion), Answers(InAnswers), CorrectAnswerIndex(InCorrectIndex)
    {
    }
};


// Define the Actor class
UCLASS()
class R_QUIZGAME_API AQuizLogActor : public AActor // Replace YOURPROJECTNAME_API with your project's API macro (e.g., MYPROJECT_API)
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AQuizLogActor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Array to hold all the quiz questions
    // EditAnywhere allows editing this data directly in the Unreal Editor details panel
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quiz Data")
    TArray<FQuizQuestion> QuizQuestions;

    // Function to initialize the quiz data with sample questions
    void InitializeQuizData();

    // Function to log the current quiz data to the Output Log
    UFUNCTION(BlueprintCallable, Category = "Quiz") // Make it callable from Blueprints if needed
        void LogQuizData();

public:
    // Called every frame (we likely won't need this for just logging)
    // virtual void Tick(float DeltaTime) override;

};