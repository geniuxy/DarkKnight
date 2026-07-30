// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkInventoryComponent.h"
#include "DkNpcInventoryComp.generated.h"


class ADkCharacterMerchant;
class UPA_MerchantInfoGenerics;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARKKNIGHT_API UDkNpcInventoryComp : public UDkInventoryComponent
{
	GENERATED_BODY()

public:
	UDkNpcInventoryComp();

	virtual void InitializeInventoryComponent() override;

	virtual void InitInventorySlotArray() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Merchant Info")
	UPA_MerchantInfoGenerics* MerchantInfoGenerics;

	UPROPERTY()
	ADkCharacterMerchant* OwningMerchant;

private:
	void AddMerchantShopItems();
};
