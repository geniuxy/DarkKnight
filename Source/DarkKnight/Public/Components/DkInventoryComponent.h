// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="true")
	void TryAddItem(UDkItemComponent* ItemComponent);

	void ConstructInventoryMenu();

	FInventoryItemChange OnItemAdded;
	FInventoryItemChange OnItemRemoved;
	FOnRoomInInventoryDelegate OnNoRoomInInventory;

protected:
	virtual void BeginPlay() override;

private:
	TWeakObjectPtr<ADkCharacterHero> OwningCharacter;

	TWeakObjectPtr<UDkWidgetInventoryMenu> CachedInventoryMenu;
};
