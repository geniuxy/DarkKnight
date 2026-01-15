// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/DkInventoryFastArray.h"
#include "DkInventoryComponent.generated.h"


class UDkWidgetInventoryMenu;
class UDkItemComponent;
class UDkInventoryItem;
class ADkCharacterHero;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemChange, UDkInventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoomInInventoryDelegate);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DARKKNIGHT_API UDkInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDkInventoryComponent();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

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
	
	FInventoryItemChange OnItemAdded;
	FInventoryItemChange OnItemRemoved;
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
