// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkInventoryCompositeBase.h"
#include "DkInventoryComposite.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkInventoryComposite : public UDkInventoryCompositeBase
{
	GENERATED_BODY()
public:
	//~Begin UUserWidget
	virtual void NativeOnInitialized() override;
	//~End UUserWidget

	virtual void ApplyFunction(FuncType Function) override;
	virtual void Collapse() override;

private:
	UPROPERTY()
	TArray<TObjectPtr<UDkInventoryCompositeBase>> Children;
};
