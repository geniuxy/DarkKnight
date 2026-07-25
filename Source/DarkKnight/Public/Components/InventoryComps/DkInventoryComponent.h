// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/LoadingScreenInterface.h"
#include "Inventory/DkInventoryCategoryItems.h"
#include "Inventory/DkInventoryFastArray.h"
#include "DkInventoryComponent.generated.h"


struct FInventoryItemManifest;
class UDkAbilitySystemComponent;
class UDkInventoryItemDescriptionMenu;
class UDkInventoryDraggedItem;
struct FDkInventorySlotAvailabilityResult;
class UDkWidgetInventoryMenu;
class UDkItemComponent;
class UDkInventoryItem;
class ADkCharacterHero;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemChange, UDkInventoryItem*, Item);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAddItemNotice, const FText&, ItemName, int, Count);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoomInInventoryDelegate, const FText&, InText);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStackChange, const FDkInventorySlotAvailabilityResult&, Result);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDraggedItemCreated, UDkInventoryDraggedItem*, DraggedItem);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDraggedItemRemoved);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDraggedItemRecovered, UDkInventoryDraggedItem*, DraggedItem);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnItemDescriptionMenuCreated, UDkInventoryItemDescriptionMenu*, ItemDescription);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemDescriptionMenuRemoved);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemEquipStatusChanged, UDkInventoryItem*, Item);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DARKKNIGHT_API UDkInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDkInventoryComponent();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	void InitializeInventoryComponent();

	/* 道具物品的添加和删除 */
	void TryAddItem(UDkItemComponent* ItemComponent);

	void TryAddItem(UDkInventoryItem* Item);

	UFUNCTION(Server, Reliable)
	void Server_AddNewItem(
		UDkItemComponent* ItemComponent, int32 StackCount, FDkInventorySlotAvailabilityResult Result
	);

	UFUNCTION(Server, Reliable)
	void Server_AddNewItemWithItem(
		UDkInventoryItem* Item, int32 StackCount, FDkInventorySlotAvailabilityResult Result
	);

	UFUNCTION(Server, Reliable)
	void Server_AddStacksToItem(UDkItemComponent* ItemComponent, FDkInventorySlotAvailabilityResult Result);

	UFUNCTION(Server, Reliable)
	void Server_AddStacksToItemWithItem(UDkInventoryItem* InItem, FDkInventorySlotAvailabilityResult Result);

	void AddRepSubObj(UObject* SubObj);

	FInventoryItemChange OnItemAdded; // item从无到有
	FInventoryItemChange OnItemRemoved; // item从有到无
	FStackChange OnStackChange; // item从有到更多
	FOnAddItemNotice OnAddItemNotice; // 获取Item后需要通知
	/********/

	/* 道具物品的丢弃 */
	UFUNCTION(Server, Reliable)
	void ServerDropItem(UDkInventoryItem* Item, int32 DroppedCount);

	void SpawnDroppedItem(UDkInventoryItem* Item, int32 DroppedCount);

	UPROPERTY(EditAnywhere, Category="Inventory")
	float DropSpawnAngleMin = -85.f;

	UPROPERTY(EditAnywhere, Category="Inventory")
	float DropSpawnAngleMax = 85.f;

	UPROPERTY(EditAnywhere, Category="Inventory")
	float DropSpawnDistanceMin = 50.f;

	UPROPERTY(EditAnywhere, Category="Inventory")
	float DropSpawnDistanceMax = 100.f;

	UPROPERTY(EditAnywhere, Category="Inventory")
	float RelativeSpawnElevation = 70.f; // Spawn降低高度
	/********/

	/* 道具物品的使用 */
	UFUNCTION(Server, Reliable)
	void ServerConsumeItem(UDkInventoryItem* Item);
	/********/

	/* 判断是否背包有空间 */
	FOnRoomInInventoryDelegate OnNoRoomInInventory;
	/********/

	/* DraggedItem相关Delegate */
	FOnDraggedItemCreated OnDraggedItemCreated;
	FOnDraggedItemRemoved OnDraggedItemRemoved;
	FOnDraggedItemRecovered OnExitGameMenuRecoverEquippedItem;
	FOnDraggedItemRecovered OnExitGameMenuRecoverGridItem;
	/********/

	/* ItemDescriptionMenu相关Delegate */
	FOnItemDescriptionMenuCreated OnItemDescriptionMenuCreated;
	FOnItemDescriptionMenuRemoved OnItemDescriptionMenuRemoved;
	/********/

	/* 装备Item相关 */
	UFUNCTION(Server, Reliable)
	void ServerUpdateEquippedItem(UDkInventoryItem* EquippedItem, UDkInventoryItem* UnEquippedItem);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastUpdateEquippedItem(UDkInventoryItem* EquippedItem, UDkInventoryItem* UnEquippedItem);

	FItemEquipStatusChanged OnItemEquipped;
	FItemEquipStatusChanged OnItemUnEquipped;
	/********/

protected:
	virtual void BeginPlay() override;

	TWeakObjectPtr<ADkCharacterHero> OwningCharacter;
	TWeakObjectPtr<UDkAbilitySystemComponent> OwnerASC;

	UPROPERTY(Replicated)
	FDkInventoryFastArray InventoryList;

	/**********************************************************************/
	/*                     Inventory Item Brief Info                      */
	/**********************************************************************/
public:
	void InitInventoryCategoryItemsArray();
	void UpdateInventoryCategoryItemsArray(const FDkInventorySlotAvailabilityResult& Result);

	const FInventoryCategoryItemsArray& GetInventoryCategoryItemsArray() const
	{
		return InventoryCategoryItemsArray.Get<FInventoryCategoryItemsArray>();
	}

	FInventoryCategoryItemsArray& GetInventoryCategoryItemsArrayMutable()
	{
		return InventoryCategoryItemsArray.GetMutable<FInventoryCategoryItemsArray>();
	}

	UFUNCTION()
	virtual void OnRep_InventoryCategoryItemsArray();

	const TArray<FInventoryItemBriefInfo>* GetItemBriefInfoByCategory(EInventoryItemCategory InCategory) const;

protected:
	UPROPERTY(VisibleAnywhere, meta=(BaseStruct="/Script/DarkKnight.InventoryCategoryItemsArray"),
		ReplicatedUsing=OnRep_InventoryCategoryItemsArray)
	FInstancedStruct InventoryCategoryItemsArray; // 用于记录背包里的放置情况

	UPROPERTY(EditAnywhere, Category="Inventory Item Category")
	TArray<FInventoryItemCategoryInfo> CategoryInfoList;

public:
	FORCEINLINE TArray<FInventoryItemCategoryInfo> GetAllInventoryCategoryInfo() const { return CategoryInfoList; }
	/**********************************************************************/
	/*                           Check Has Room                           */
	/**********************************************************************/
public:
	FDkInventorySlotAvailabilityResult HasRoomForItem(const UDkItemComponent* ItemComponent);
	FDkInventorySlotAvailabilityResult HasRoomForItem(const UDkInventoryItem* Item);

private:
	FDkInventorySlotAvailabilityResult HasRoomForItem(const FInventoryItemManifest& Manifest);

	int32 CalculateFillAmountForSlot(
		const bool bStackable,
		const int32 MaxStackSize,
		const int32 AmountToFill,
		const FInventoryItemBriefInfo& ItemBriefInfo
	) const;
};
