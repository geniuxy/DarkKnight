// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkInventoryCompositeBase.h"
#include "DkInventoryLeaf.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkInventoryLeaf : public UDkInventoryCompositeBase
{
	GENERATED_BODY()

public:
	virtual void ApplyFunction(FuncType Function) override;
};
