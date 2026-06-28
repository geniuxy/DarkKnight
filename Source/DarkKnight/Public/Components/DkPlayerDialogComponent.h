// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "DkPlayerDialogComponent.generated.h"


struct FDialogNpcDetail;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARKKNIGHT_API UDkPlayerDialogComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDkPlayerDialogComponent();

	bool FindDialogGameplayTag(FGameplayTagContainer InTags);
	void AddDialogTag(FGameplayTag InTag);

protected:
	virtual void BeginPlay() override;

private:
	FGameplayTagContainer CachedDialogTags;
};
