// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DarkKnight/DarkKnight.h"
#include "GameFramework/Actor.h"
#include "DkEquippedActorBase.generated.h"


class UDkHighlightableSkeletalMesh;

UCLASS()
class DARKKNIGHT_API ADkEquippedActorBase : public AActor
{
	GENERATED_BODY()

public:
	ADkEquippedActorBase();

protected:
	UPROPERTY(EditAnywhere, Category="Equipment")
	FGameplayTag EquipmentTag;

public:
	LIST_DATA_ACCESSOR(FGameplayTag, EquipmentTag)
};
