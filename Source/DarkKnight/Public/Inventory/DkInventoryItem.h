// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkInventoryStruct.h"
#include "StructUtils/InstancedStruct.h"
#include "UObject/Object.h"
#include "DkInventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkInventoryItem : public UObject
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void SetItemManifest(const FInventoryItemManifest& Manifest);

private:
	UPROPERTY(VisibleAnywhere, meta=(BaseStruct="/Script/DarkKnight.InventoryItemManifest"), Replicated)
	FInstancedStruct ItemManifest;
};
