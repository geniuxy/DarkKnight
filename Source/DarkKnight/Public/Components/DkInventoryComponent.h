// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/LoadingScreenInterface.h"
#include "Inventory/DkInventoryFastArray.h"
#include "DkInventoryComponent.generated.h"


struct FDkInventorySlotAvailabilityResult;
class UDkWidgetInventoryMenu;
class UDkItemComponent;
class UDkInventoryItem;
class ADkCharacterHero;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemChange, UDkInventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoomInInventoryDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStackChange, const FDkInventorySlotAvailabilityResult&, Result);

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

	bool bHasInitInventoryMenu = false;
	/********/

	/* 道具物品的添加和删除 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="true")
	void TryAddItem(UDkItemComponent* ItemComponent);

	UFUNCTION(Server, Reliable)
	void Server_AddNewItem(UDkItemComponent* ItemComponent, int32 StackCount);
	
	UFUNCTION(Server, Reliable)
	void Server_AddStacksToItem(UDkItemComponent* ItemComponent, int32 StackCount, int32 Remainder);

	void AddRepSubObj(UObject* SubObj);
	
	FInventoryItemChange OnItemAdded; // item从无到有
	FInventoryItemChange OnItemRemoved; // item从有到无
	FStackChange OnStackChange; // item从有到更多
	/********/

	/* 判断是否背包有空间 */
	FOnRoomInInventoryDelegate OnNoRoomInInventory;
	/********/

protected:
	virtual void BeginPlay() override;

private:
	TWeakObjectPtr<ADkCharacterHero> OwningCharacter;

	TWeakObjectPtr<UDkWidgetInventoryMenu> CachedInventoryMenu;

	UPROPERTY(Replicated)
	FDkInventoryFastArray InventoryList;
};
