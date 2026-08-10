// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DkCraftListData.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkCraftListData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FText CraftItemName;
	UPROPERTY()
	bool IsReady;
	UPROPERTY()
	UTexture2D* CraftItemIcon;
};
