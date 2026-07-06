// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkTypes/DkStructs.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DkDialogFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkDialogFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Dialog Function Library")
	static bool HasTaskToCommit(int InDialogId);

	UFUNCTION(BlueprintCallable, Category="Dialog Function Library")
	static FCommitTaskDetails GetDialogCommitTaskDetails(int InDialogId);
};
