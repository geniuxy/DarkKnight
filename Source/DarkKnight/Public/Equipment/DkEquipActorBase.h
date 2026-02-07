// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DarkKnight/DarkKnight.h"
#include "GameFramework/Actor.h"
#include "DkEquipActorBase.generated.h"


UCLASS()
class DARKKNIGHT_API ADkEquipActorBase : public AActor
{
	GENERATED_BODY()

public:
	ADkEquipActorBase();

private:
	UPROPERTY(EditAnywhere, Category="Equipment")
	FGameplayTag EquipmentTag;

public:
	LIST_DATA_ACCESSOR(FGameplayTag, EquipmentTag)
};
