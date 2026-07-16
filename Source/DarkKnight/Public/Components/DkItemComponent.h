// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/DkInventoryItemManifest.h"
#include "DkItemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DARKKNIGHT_API UDkItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDkItemComponent();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void OnPickedUp();

	void InitializeItemComponent(const FDkItemInfo* ItemInfo, int32 InItemStack);

	void InitItemManifest(FInventoryItemManifest InItemManifest);

	int32 GetItemId() const;

	FText GetItemName() const;

	FInventoryItemManifest GetItemManifest() const { return ItemManifest; }

	FInventoryItemManifest& GetItemManifestMutable() { return ItemManifest; }

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void BP_OnPickedUp();

private:
	UPROPERTY(EditAnywhere, Category="Inventory")
	FString PickUpMessage;

	UPROPERTY(Replicated, EditAnywhere, Category="Inventory")
	FInventoryItemManifest ItemManifest;

public:
	FORCEINLINE FString GetPickUpMessage() const { return PickUpMessage; }
};
