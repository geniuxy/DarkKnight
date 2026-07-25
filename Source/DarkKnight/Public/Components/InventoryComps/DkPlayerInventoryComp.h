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
//
// 	/* 请求将某槽位的物品移动到另一槽位（空位） */
// 	UFUNCTION(BlueprintCallable, Category="Category Items Array")
// 	void RequestMoveItem(EInventoryItemCategory Category, int32 FromSlot, int32 ToSlot, int32 StackCount);
//
// 	UFUNCTION(BlueprintCallable, Category="Category Items Array")
// 	void RequestMoveItemFromEquipment(
// 		EInventoryItemCategory Category, UDkInventoryItem* Item, int32 ToSlot, int32 StackCount
// 	);
//
// 	/* 请求交换两个槽位的物品 */
// 	UFUNCTION(BlueprintCallable, Category="Category Items Array")
// 	void RequestSwapItems(EInventoryItemCategory Category, int32 SlotA, int32 SlotB, int32 StackCountFromA);
//
// protected:
// 	UFUNCTION(Server, Reliable, WithValidation)
// 	void Server_MoveItem(EInventoryItemCategory Category, int32 FromSlot, int32 ToSlot, int32 StackCount);
//
// 	UFUNCTION(Server, Reliable, WithValidation)
// 	void Server_MoveItemFromEquipment(
// 		EInventoryItemCategory Category, UDkInventoryItem* Item, int32 ToSlot, int32 StackCount
// 	);
//
// 	UFUNCTION(Server, Reliable, WithValidation)
// 	void Server_SwapItems(EInventoryItemCategory Category, int32 SlotA, int32 SlotB, int32 StackCountFromA);
//
// 	void ApplyMoveItem(EInventoryItemCategory Category, int32 FromSlot, int32 ToSlot, int32 MoveStackCount);
// 	void ApplyMoveItemFromEquipment(
// 		EInventoryItemCategory Category, UDkInventoryItem* Item, int32 ToSlot, int32 MoveStackCount
// 	);
// 	void ApplySwapItems(EInventoryItemCategory Category, int32 SlotA, int32 SlotB, int32 StackCountFromA);
};
