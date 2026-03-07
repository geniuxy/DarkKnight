// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/LoadingScreenInterface.h"
#include "Inventory/DkInventoryFastArray.h"
#include "DkInventoryComponent.generated.h"


class UDkAbilitySystemComponent;
class UDkInventoryItemDescriptionMenu;
class UDkInventoryDraggedItem;
struct FDkInventorySlotAvailabilityResult;
class UDkWidgetInventoryMenu;
class UDkItemComponent;
class UDkInventoryItem;
class ADkCharacterHero;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemChange, UDkInventoryItem*, Item);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoomInInventoryDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStackChange, const FDkInventorySlotAvailabilityResult&, Result);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDraggedItemCreated, UDkInventoryDraggedItem*, DraggedItem);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDraggedItemRemoved);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDraggedItemRecovered, UDkInventoryDraggedItem*, DraggedItem);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnItemDescriptionMenuCreated, UDkInventoryItemDescriptionMenu*, ItemDescription);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemDescriptionMenuRemoved);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemEquipStatusChanged, UDkInventoryItem*, Item);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DARKKNIGHT_API UDkInventoryComponent : public UActorComponent, public ILoadingScreenInterface
{
	GENERATED_BODY()

public:
	UDkInventoryComponent();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	//~Begin ILoadingScreenInterface Interface
	virtual void OnLoadingScreenDeactivated_Implementation() override;
	//~End ILoadingScreenInterface Interface

	/* 构建背包界面 */
	void ConstructInventoryMenu();
	void InitializeInventoryComponent();
	
	bool bHasInitInventoryMenu = false;
	/********/

	/* 道具物品的添加和删除 */
	void TryAddItem(UDkItemComponent* ItemComponent);

	void TryAddItem(UDkInventoryItem* Item);
	
	UFUNCTION(Server, Reliable)
	void Server_AddNewItem(UDkItemComponent* ItemComponent, int32 StackCount, int32 Remainder);

	UFUNCTION(Server, Reliable)
	void Server_AddNewItemWithItem(UDkInventoryItem* Item, int32 StackCount, int32 Remainder);

	UFUNCTION(Server, Reliable)
	void Server_AddStacksToItem(UDkItemComponent* ItemComponent, int32 StackCount, int32 Remainder);

	UFUNCTION(Server, Reliable)
	void Server_AddStacksToItemWithItem(UDkInventoryItem* InItem, int32 StackCount, int32 Remainder);

	void AddRepSubObj(UObject* SubObj);

	FInventoryItemChange OnItemAdded; // item从无到有
	FInventoryItemChange OnItemRemoved; // item从有到无
	FStackChange OnStackChange; // item从有到更多
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

private:
	TWeakObjectPtr<ADkCharacterHero> OwningCharacter; // 仅在客户端上存在， 这个在初始化的时候用了一下。直接用GetOwner获取角色即可
	
	TWeakObjectPtr<UDkAbilitySystemComponent> OwnerASC;

	TWeakObjectPtr<UDkWidgetInventoryMenu> CachedInventoryMenu; // 仅在客户端上存在

	UPROPERTY(Replicated)
	FDkInventoryFastArray InventoryList;
};
