// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkEquippedActorBase.h"
#include "DkEquippedActorSkeletal.generated.h"

UCLASS()
class DARKKNIGHT_API ADkEquippedActorSkeletal : public ADkEquippedActorBase
{
	GENERATED_BODY()

public:
	ADkEquippedActorSkeletal();

	void SetEquipmentSkeletalMesh(USkeletalMesh* InEquipmentItemMesh) const;

protected:
	/* Actor Components */
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* EquipmentItemSkeletalMesh;
	/*********/
};
