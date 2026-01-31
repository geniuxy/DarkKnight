// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "DarkKnight/DarkKnight.h"
#include "DkInventoryCompositeBase.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkInventoryCompositeBase : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	virtual void Collapse();
	void Expand();

	using FuncType = TFunction<void(UDkInventoryCompositeBase*)>;
	virtual void ApplyFunction(FuncType Function) {}

protected:
	UPROPERTY(EditAnywhere, meta = (Categories = "Dk.Inventory.Fragment"), Category="Inventory")
	FGameplayTag FragmentTag;

public:
	LIST_DATA_ACCESSOR(FGameplayTag, FragmentTag)
};
