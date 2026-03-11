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

	void SetEquipmentSkeletalMesh(USkeletalMesh* InEquipmentItemMesh) const;

private:
	UPROPERTY(EditAnywhere, Category="Equipment")
	FGameplayTag EquipmentTag;

	/* Actor Components */
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* EquipmentItemSkeletalMesh;
	/*********/

public:
	LIST_DATA_ACCESSOR(FGameplayTag, EquipmentTag)
};
