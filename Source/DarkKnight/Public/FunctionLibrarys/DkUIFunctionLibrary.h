// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DkUIFunctionLibrary.generated.h"

class UDkWidgetActivatableBase;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkUIFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="UI Function Library")
	static TSoftClassPtr<UDkWidgetActivatableBase> GetUISoftWidgetClassByTag(
		UPARAM(meta=(Categories="Dk.Widget")) FGameplayTag InWidgetTag
	);

	UFUNCTION(BlueprintPure, Category="UI Function Library")
	static TSoftObjectPtr<UTexture2D> GetSoftImageByTag(UPARAM(meta=(Categories="Dk.Image")) FGameplayTag InImageTag);
};
