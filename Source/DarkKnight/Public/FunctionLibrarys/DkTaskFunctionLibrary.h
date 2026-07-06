// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkTypes/DkStructs.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DkTaskFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkTaskFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Task Function Library")
	static FTaskInfo GetTaskInfoById(int InTaskId);

	UFUNCTION(BlueprintCallable, Category="Task Function Library")
	static TArray<int> GetAllSubTaskId(int InMainTaskId);
	
	UFUNCTION(BlueprintCallable, Category="Task Function Library")
	static int GetSubTaskTarget(int InMainTaskId, int InSubTaskId);

	UFUNCTION(BlueprintCallable, Category="Task Function Library")
	static bool IsNextSubTaskIdZero(int InMainTaskId, int InSubTaskId);
	
	UFUNCTION(BlueprintCallable, Category="Task Function Library")
	static int GetNextSubTaskId(int InMainTaskId, int InSubTaskId);
};
