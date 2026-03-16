// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkEquippedActorBase.h"
#include "DkEquippedActorStatic.generated.h"

UCLASS()
class DARKKNIGHT_API ADkEquippedActorStatic : public ADkEquippedActorBase
{
	GENERATED_BODY()

public:
	ADkEquippedActorStatic();

	void SetEquipmentStaticMesh(UStaticMesh* InEquipmentItemMesh) const;

protected:
	/* Actor Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* EquipmentItemStaticMesh;
	/*********/
};
