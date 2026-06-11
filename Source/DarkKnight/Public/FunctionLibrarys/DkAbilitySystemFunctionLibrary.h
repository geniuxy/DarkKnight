// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "DkAbilitySystemFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkAbilitySystemFunctionLibrary : public UAbilitySystemBlueprintLibrary
{
	GENERATED_BODY()

public:
	static bool IsActorDead(const AActor* ActorToCheck);

	static bool ActorHasTag(const AActor* ActorToCheck, const FGameplayTag& Tag);
};
