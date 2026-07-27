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

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void SetItemId(int InItemId);

protected:
	UPROPERTY(EditAnywhere, Category="Equipment")
	FGameplayTag EquipmentTag;

	UPROPERTY(ReplicatedUsing = OnRep_ItemId)
	int32 ItemId = 0;

	UFUNCTION()
	void OnRep_ItemId();

	virtual void UpdateEquipmentItemInfo();

public:
	LIST_DATA_ACCESSOR(FGameplayTag, EquipmentTag)
};
