// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "DkPlayerDialogComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARKKNIGHT_API UDkPlayerDialogComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDkPlayerDialogComponent();

	bool FindDialogGameplayTag(FGameplayTag InTag);

private:
	FGameplayTagContainer CachedDialogTags;
};
