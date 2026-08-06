// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkInventoryComponent.h"
#include "DkPlayerInventoryComp.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARKKNIGHT_API UDkPlayerInventoryComp : public UDkInventoryComponent
{
	GENERATED_BODY()

public:
	/* 构建背包界面 */
	void ConstructInventoryMenu();
	/**************/

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_TryToBuyItem(int MerchantNpcId, EInventoryItemCategory ItemCategory, int GridIndex, int Count);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_TryToSellItem(int MerchantNpcId, EInventoryItemCategory ItemCategory, int GridIndex, int Count);
	
	UFUNCTION(Client, Reliable)
	void Client_NotifyToUpdateGrid(int MerchantNpcId);
};
