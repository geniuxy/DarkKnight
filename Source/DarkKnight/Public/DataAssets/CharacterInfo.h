// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterInfo.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UCharacterInfo : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Character Basic Info")
	float MaxWalkSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category="Character Basic Info")
	float MaxRunSpeed;

	UPROPERTY(EditDefaultsOnly, Category="Character Basic Info")
	float MaxSprintSpeed;
};
