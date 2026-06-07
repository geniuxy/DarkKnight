// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkEquippedActorStatic.h"
#include "DkEquippedActorStaticWithAdditional.generated.h"

UCLASS()
class DARKKNIGHT_API ADkEquippedActorStaticWithAdditional : public ADkEquippedActorStatic
{
	GENERATED_BODY()
	
public:
	ADkEquippedActorStaticWithAdditional();

protected:
	/* Actor Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* AdditionalMesh;
	/*********/

public:
	FORCEINLINE UStaticMeshComponent* GetAdditionalMesh() const { return AdditionalMesh; }
};
